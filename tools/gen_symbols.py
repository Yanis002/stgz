#!/usr/bin/env python3
# SPDX-FileCopyrightText: © 2026 stgz team
# SPDX-License-Identifier: GPL-3.0-only

import argparse
import json
import re
import os

from pathlib import Path


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-d", "--decomp", type=Path, required=True)
    args = parser.parse_args()

    symbols_path = Path("build/symbols.json").resolve()

    decomp_path: Path = args.decomp.resolve()
    config_path = decomp_path / "config"

    versions = [Path(path).stem for path, _, _ in os.walk(str(config_path)) if Path(path).parent.name == config_path.name]

    out_json = {}
    for version in versions:
        out_json[version] = {}

    for version in versions:
        ver_cfg_path = config_path / version

        for sym_path in ver_cfg_path.rglob("symbols.txt"):
            filelines = sym_path.read_text().splitlines()

            for line in filelines:
                split = line.split(" ")
                sym_name = split.pop(0)

                is_local = False
                for elem in split:
                    if "local" in elem:
                        is_local = True
                        break

                # ignore static initializers, local and anonymous symbols (TODO: solve sqrt oddity)
                if "sinit" in sym_name or sym_name.startswith("@") or is_local or sym_name == "Sqrt":
                    continue

                addr_match = re.search(r"addr:0x([a-fA-F0-9]*)", line)
                assert addr_match is not None, f"address of {sym_name} not found"
                sym_addr = f"0x{addr_match.group(1).upper()}"

                out_json[version][sym_name] = sym_addr

    with symbols_path.open("w") as f:
        json.dump(out_json, f, indent=2)

if __name__ == "__main__":
    main()
