# stgz

WIP practice tools for The Legend of Zelda: Spirit Tracks.

Currently stuck on the following issues:
- displaying text on-screen

## Planned Features

I'm not sure yet what are the limitations but here's what I'd like to have:
- warp menu
- add commands, like save the position, moon jump, make Link walk faster, reload area
- frame advance (if possible)
- inventory editor
- equips editor (item, fairies, boat parts?)
- savestates (if possible)
- debug tools, like a collision/hitbox viewer, watches
- cheats (health, rupees, ammo, etc)

Basically, if you're familiar with the [practice tools from OoT](https://github.com/glankk/gz), I'd like to add similar features.

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

## Credits

This project is using [ds-rom](https://github.com/AetiasHax/ds-rom), made by Aetias.

Made with ♥ by me.
