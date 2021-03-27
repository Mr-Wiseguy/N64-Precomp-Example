# Assembly Macros

.set gp=64

.set K0BASE, 0x80000000
.set K1BASE, 0xA0000000
.set K2BASE, 0xC0000000

.macro glabel label
    .global \label
    .balign 4
    \label:
.endm

.macro function label
    .global \label
    .type \label @function 
    .balign 4
    \label:
.endm

.macro .helper_start
    addiu  $sp, $sp, -0x28
    sw     $ra, 0x10($sp)
    sw     $a0, 0x14($sp)
    sw     $a1, 0x18($sp)
    sw     $a2, 0x1C($sp)
    sw     $a3, 0x20($sp)
.endm

.macro .helper_end old_func
    lw     $ra, 0x10($sp)
    lw     $a0, 0x14($sp)
    lw     $a1, 0x18($sp)
    lw     $a2, 0x1C($sp)
    lw     $a3, 0x20($sp)
    j      \old_func
     addiu $sp, $sp, 0x28
.endm

.macro .helper_call_no_args new_func old_func
    addiu  $sp, $sp, -0x28
    sw     $ra, 0x10($sp)
    sw     $a0, 0x14($sp)
    sw     $a1, 0x18($sp)
    sw     $a2, 0x1C($sp)
    jal    \new_func
     sw     $a3, 0x20($sp)
    lw     $ra, 0x10($sp)
    lw     $a0, 0x14($sp)
    lw     $a1, 0x18($sp)
    lw     $a2, 0x1C($sp)
    lw     $a3, 0x20($sp)
    j      \old_func
     addiu $sp, $sp, 0x28
.endm

# TODO does not work?
.macro .helper_call_no_args_no_saves new_func old_func
    addiu  $sp, $sp, -0x18
    jal    \new_func
     sw     $ra, 0x10($sp)
    lw     $ra, 0x10($sp)
    j      \old_func
     addiu $sp, $sp, 0x18
.endm
