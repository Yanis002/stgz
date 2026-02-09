# stgz

WIP practice tools for The Legend of Zelda: Spirit Tracks.

Currently stuck on the following issues:
- displaying text on-screen

**IMPORTANT**: this project doesn't provide any protected IPs, users are required to provide the necessary files by their own means and we won't help anyone in this regard.

## Planned Features

I'm not sure yet what are the limitations but here's what I'd like to have:
- warp menu
- add commands, like save the position, moon jump, make Link walk faster, reload area
- inventory editor
- equips editor (item, fairies, boat parts?)
- savestates (if possible)
- debug tools, like a collision/hitbox viewer, watches
- cheats (health, rupees, ammo, etc)

Basically, if you're familiar with the [practice tools from OoT](https://github.com/glankk/gz), I'd like to add similar features.

## Project TODO List

- Add rolling releases (low priority)
- Implement each planned features, if possible

## Building

You will need the following packages/tools:
- `git`
- `make`
- `cmake`
- `gcc-arm-none-eabi` ([arch-based users](https://aur.archlinux.org/packages/gcc-arm-none-eabi-bin), [debian-based users](https://launchpad.net/ubuntu/+source/gcc-arm-none-eabi))

Steps:
- Place a baserom in the `extract` folder and name it `baserom_st_REGION.nds`, `REGION` being `eur`, `us` or `jp` (Note: only the european version is supported currently).
- `git submodule update --init --recursive`
- `make init`
- `make setup`
- `make`

## Contributing

Any help is welcome!

If you wish to help on this project, clone the repo then follow the build instructions.

**Warning**: if you're altering decomp symbols in any way, make sure to run `make libs` to generate the new symbol libraries and the asm files inside `src/thumb`.

## Project Structure

- `.github/workflows/`: hosts the actions workflows
- `extract/`: the folder where the rom will be extracted to
- `hooks/`: root directory for hooks source code
- `include/`: root directory for source code headers
- `libs/`: hosts libraries and symbol libraries generated from `make libs`, for use by the linker
- `resources/`: for general resources and also external projects that aren't tools
- `src/`: root directory for stgz source code
- `src/thumb/`: hosts the generated thumb assembly files for thumb compatibility when calling such functions from the game
- `tools/`: various programs, python scripts and misc files

## Credits

Concept heavily inspired by [gz](https://github.com/glankk/gz), made by glank and many contributors.

Referenced projects:
- [ds-rom](https://github.com/AetiasHax/ds-rom), made by Aetias.
- [armips](https://github.com/Kingcom/armips), made by Kingcom and many contributors.

Made with ♥ by me.
