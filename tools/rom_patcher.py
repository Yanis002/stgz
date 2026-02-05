#!/usr/bin/env python3
# SPDX-FileCopyrightText: © 2026 stgz team
# SPDX-License-Identifier: GPL-3.0-only

import argparse

from pathlib import Path


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--extract", "-e", type=Path, required=True)
    args = parser.parse_args()

    extracted_dir: Path = args.extract
    assert extracted_dir.exists()

    # update overlay hi
    arm9_file = extracted_dir / "arm9" / "arm9.bin"
    arm9 = arm9_file.read_bytes()
    arm9 = arm9.replace(b"\x80\xA3\x18\x02", b"\x80\xA3\x1A\x02")
    arm9_file.write_bytes(arm9)


if __name__ == "__main__":
    main()
