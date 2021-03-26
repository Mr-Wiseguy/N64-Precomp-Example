#include <macro.inc>
.set noat
.set noreorder
.section .text

function dma_code
    jal create_scheduler # Replacement for the instruction that was overwritten
     nop
    
    addiu $sp, $sp, -0x20
    sw $a2, 0x18($sp)
    sw $a1, 0x14($sp)
    sw $a0, 0x10($sp)

    la $a0, _baseromSegmentRomEnd
    la $a1, _customSegmentStart
    lui $a2, %hi(_customSegmentSize)
    jal do_dma_read
    addiu $a2, $a2, %lo(_customSegmentSize)

    lw $a2, 0x18($sp)
    lw $a1, 0x14($sp)
    lw $a0, 0x10($sp)
    j dma_hook + 2 * 4
     addiu $sp, $sp, -0x20
