#!/usr/bin/env python3
# SPDX-FileCopyrightText: © 2026 stgz team
# SPDX-License-Identifier: GPL-3.0-only

import argparse
import json
import copy
import subprocess
import struct

from dataclasses import dataclass
from pathlib import Path

INDENT = " " * 4

@dataclass
class Symbol:
    name: str
    addr: int

    def to_asm(self):
        return f".definelabel {self.name}, 0x{self.addr:08X}\n"


class SetupASM:
    def __init__(self, obj_list: list[str], hooks_obj_list: list[str]):
        self.entries: list[Symbol] = []
        self.obj_list = obj_list
        self.hooks_obj_list = hooks_obj_list

    @staticmethod
    def new(json_path: Path, version: str, obj_list: list[str], hooks_obj_list: list[str]):
        assert json_path.exists(), "symbols data not found"
        symbols_json = json.loads(json_path.read_text())

        setupASM = SetupASM(obj_list, hooks_obj_list)
        for sym_name, sym_addr in symbols_json[version].items():
            setupASM.entries.append(Symbol(sym_name, int(sym_addr, base=16)))
        
        return setupASM

    def to_asm(self):
        # not sure yet if it's a good idea to generate this entire file but whatever

        lines = [
            "; This file was created by `tools/rom_patcher.py`\n",

            ".nds",
            ".relativeinclude on",
            ".erroronwarning on\n",

            "".join(sym.to_asm() for sym in self.entries),

            "; create gz overlay data",
            ".open OVLGZ_BIN, OVLGZ_ADDR",
            INDENT + f".org OVLGZ_ADDR",
            INDENT * 2 + f".area OVLGZ_SIZE, 0xEE",
            INDENT * 3 + f"\n{INDENT * 3}".join(f'.importobj "{obj}"' for obj in self.obj_list),
            INDENT * 2 + f".endarea",
            f".close\n",

            ".open ARM9_BIN, ARM9_MOD_BIN, 0x02000000",
            INDENT + "; load the hooks into the main module",
            INDENT + ".org ARM9_NEW_CODE_STORE_ADDR",
            INDENT * 2 + ".area HOOKS_SIZE, 0xFF",
            INDENT * 3 + f"\n{INDENT * 3}".join(f'.importobj "{obj}"' for obj in self.hooks_obj_list),
            INDENT * 2 + ".endarea\n",

            INDENT + "; apply update hook",
            INDENT + ".org HOOK_UPDATE",
            INDENT * 2 + ".arm",
            INDENT * 2 + ".area 0x04",
            INDENT * 3 + "bl GZ_UpdateHook",
            INDENT * 2 + ".endarea",
            ".close\n",

            ".open OVL018_BIN, OVL018_MOD_BIN, OVL018_ADDR",
            INDENT + "; apply init hook",
            INDENT + ".org HOOK_INIT",
            INDENT * 2 + ".arm",
            INDENT * 2 + ".area 0x04",
            INDENT * 3 + "bl GZ_InitHook",
            INDENT * 2 + ".endarea",
            ".close\n",
        ]

        return "\n".join(lines)

    def write(self):
        Path("hooks/setup.asm").resolve().write_text(self.to_asm())
        print("setup.asm is OK!")


def check_code_size(obj_list: list[str], max_size: int, kind: str):
    split = subprocess.check_output(["size", "--totals", *obj_list], text=True).split("\n")

    for elem in split:
        if "TOTALS" in elem:
            code_size = int(elem.split("\t")[-2].strip(), base=16)
            print(f"{kind} code size is OK! (code size: 0x{code_size:X} < max: 0x{max_size:X})")
            assert code_size < max_size, f"{kind} code size exceeds the available space! (code size: 0x{code_size:X} >= max: 0x{max_size:X})"
            break


def patch_arm9(extracted_dir: Path, base_addr: int, offset: int):
    # open and read file
    assert extracted_dir.exists()
    arm9_file = extracted_dir / "arm9" / "arm9.bin"
    arm9 = arm9_file.read_bytes()

    # update overlay hi
    arm9 = arm9.replace(struct.pack("<I", base_addr), struct.pack("<I", base_addr + offset))

    # write and close file
    arm9_file.write_bytes(arm9)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--extract", "-e", type=Path, required=True)
    parser.add_argument("-o", "--obj_list", dest="obj_list", nargs="+", help="list of .o file paths", required=True)
    parser.add_argument("-m", "--main_max_size", required=True)
    parser.add_argument("-j", "--hooks_obj_list", dest="hooks_obj_list", nargs="+", help="list of .o hooks file paths", required=True)
    parser.add_argument("-n", "--hooks_max_size", required=True)
    parser.add_argument("-a", "--address", required=True)
    args = parser.parse_args()

    main_max_size = int(args.main_max_size, base=16)
    extracted_path: Path = args.extract.resolve()

    # make sure the overlay code size is ok
    check_code_size(args.obj_list, main_max_size, "Main")

    # make sure the hooks code size is ok
    check_code_size(args.hooks_obj_list, int(args.hooks_max_size, base=16), "Hooks")

    # patch the arm9 binary
    patch_arm9(extracted_path, int(args.address, base=16), main_max_size)

    # generate setup.asm
    setupASM = SetupASM.new(Path("symbols.json").resolve(), extracted_path.stem, args.obj_list, args.hooks_obj_list)
    setupASM.write()


if __name__ == "__main__":
    main()
