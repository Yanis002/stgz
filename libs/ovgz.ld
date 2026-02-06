OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

SECTIONS
{
    .text 0x0218A380 : {
        KEEP(*(.text))
        *(.text.*)
        *(.glue_7)
        *(.glue_7t)
        . = ALIGN(4);
    }

    .rodata 0x021A2380 : {
        KEEP(*(.rodata))
        *(.rodata.*)
        . = ALIGN(4);
    }

    .init 0x021A3380 : {
        KEEP(*(.init))
        . = ALIGN(4);
    }

    .ctor 0x021A4380 : {
        KEEP(*(.ctor))
        KEEP(*(.ctors))
        KEEP(*(.init_array*))
        . = ALIGN(4);
    }

    .data 0x021A4480 : {
        KEEP(*(.data))
        *(.data.*)
        . = ALIGN(4);
    }

    .bss 0x021A5480 : {
        KEEP(*(.bss))
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
    }

    /DISCARD/ : { 
        *(.comment) 
        *(.ARM.attributes)
    }
}
