OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

MEMORY
{
    ram (rwx) : ORIGIN = 0x0218A380, LENGTH = 0x00020000
}

SECTIONS
{
    .text : {
        . = ALIGN(4);
        KEEP(*(.text))
        *(.text.*)
        KEEP(*(.rodata))
        *(.rodata.*)
        *(.glue_7)
        *(.glue_7t)
        . = ALIGN(4);
    } > ram

    .data : {
        . = ALIGN(4);
        KEEP(*(.data))
        *(.data.*)
        . = ALIGN(4);
    } > ram

    .bss : {
        . = ALIGN(4);
        KEEP(*(.bss))
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
    } > ram

    /DISCARD/ : { *(.*) }
}
