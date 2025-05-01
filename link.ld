/* 
 * Linker script for my version of the BE6502 
 *
 * https://github.com/llvm-mos/llvm-mos-sdk/blob/main/mos-platform/eater/link.ld
 * https://llvm-mos.org/wiki/Porting
 */

MEMORY {
    zp : ORIGIN = __rc31 + 1, LENGTH = 0x100 - (__rc31 + 1)
    ram (rw) : ORIGIN = 0x200, LENGTH = 0x4000 - 0x200
    rom : ORIGIN = 0x8000, LENGTH = 0x7ffa
}

REGION_ALIAS("c_readonly", rom)
REGION_ALIAS("c_writeable", ram)

SECTIONS { 
    /* .text : { */
    /*     KEEP(*(.text._start)) */
    /* } >c_readonly */

    INCLUDE c.ld 
}

__rc0 = 0x00;
INCLUDE imag-regs.ld
ASSERT(__rc0 == 0x00, "Inconsistent zero page map.")
ASSERT(__rc31 == 0x1f, "Inconsistent zero page map.")

/* Set initial soft stack address to just above last ram address. (It grows down.) */
__stack = ORIGIN(ram) + LENGTH(ram);

OUTPUT_FORMAT {
    FULL(rom)
    SHORT(_irqbrk)
    SHORT(_start)
    SHORT(_irqbrk)
}
