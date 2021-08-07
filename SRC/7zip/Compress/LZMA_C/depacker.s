# -------------------------------------------------
# NRLPack LZMA Depacker v0.2
# 1460 bytes

                    .set noreorder
                    .global _start

Entry:              .long Source - Entry
                    .long Dest - Entry
                    .long Temp - Entry
                    .long Run_Datas - Entry

# -------------------------------------------------
OutStream           = -0x24
OutSize             = -0x20
State               = -0x1c
var_B4              = -0x18
var_B0              = -0x14
var_AC              = -0x10
Range               = -0xc
Code                = -0x8
Buffer              = -0x4

# $a0 = u16 *probLit

Check_Bound:        lw      $t0, Range($sp)
                    li      $v0, 0xFFFFFF
                    sltu    $v0, $t0
                    daddu   $t1, $a0, $0
                    bnez    $v0, LSM9
                    sll     $v1, $t0, 8
                    lw      $v0, Buffer($sp)
                    daddu   $t0, $v1, $0
                    sw      $v1, Range($sp)
                    lw      $v1, Code($sp)
                    lbu     $a0, 0($v0)
                    addiu   $v0, 1
                    sll     $v1, 8
                    sw      $v0, Buffer($sp)
                    or      $v1, $a0
                    sw      $v1, Code($sp)
LSM9:
                    lhu     $a1, 0($t1)
                    srl     $v0, $t0, 11      # Range << 11
                    lw      $a0, Code($sp)
                    li      $t3, 1
                    andi    $v1, $a1, 0xFFFF
                    mult    $a3, $v0, $v1     # * *(probLit)
                    srl     $a2, $v1, 5
                    li      $v0, 0x800
                    subu    $a2, $a1, $a2
                    subu    $v0, $v1
                    sra     $v0, 5
                    subu    $v1, $a0, $a3
                    addu    $a1, $v0
                    sltu    $a0, $a3
                    beqz    $a0, LSM17
                    subu    $v0, $t0, $a3
                    sw      $a3, Range($sp)
                    sh      $a1, 0($t1)       # *(probLit) += 
LSM16:
                    jr      $ra
                    daddu   $v0, $t3, $0
LSM17:
                    sw      $v0, Range($sp)
                    daddu   $t3, $0, $0
                    sw      $v1, Code($sp)
                    b       LSM16
                    sh      $a2, 0($t1)       # *(probLit) -=

# ----------------------------------------------------------------------
_start:
Source:             la      $a1, 0x12345678     # 0x1500008
Dest:               la      $a2, 0x12345678     # Dst
Temp:               la      $s6, 0x12345678     # Probs (15980 bytes)
                    daddu   $t7, $a0, 0

                    li      $s5, 1
                    daddu   $v1, $s6, $0
                    daddu   $a0, $0, $0

                    sw      $0,  State($sp)
                    daddu   $s7, $0, $0
                    daddu   $s0, $0, $0
                    sw      $a2, OutStream($sp)
                    li      $a2, 0x400
LSM20:              addiu   $a0, 1
                    sh      $a2, 0($v1)
                    sltiu   $v0, $a0, 0x1F36
                    sw      $s5, var_B4($sp)
                    sw      $s5, var_B0($sp)
                    sw      $s5, var_AC($sp)
                    bnez    $v0, LSM20
                    addiu   $v1, 2
                    addiu   $a3, $s7, -1      # $a3 = 0xffffffff
                    sw      $a3, Range($sp)
                    sw      $a1, Buffer($sp)    # Compressed datas
                    lw      $a3, -8($a1) 
                    sw      $a3, OutSize($sp)   # Uncompressed length
                    lw      $a1, -4($a1)
                    sw      $a1, Code($sp)      # Code

                    lw      $v1, State($sp)     # 0
LSM31:              andi    $s1, $s7, 3
                    sll     $a0, $v1, 4
                    addu    $a0, $s1
                    sll     $a0, 1
                    bal     Check_Bound
                    addu    $a0, $s6
                    beqz    $v0, LSM62
                    srl     $v0, $s0, 5
                    li      $v1, 0x600
                    mult    $a0, $v0, $v1
                    lw      $a1, State($sp)
                    li      $v1, 1
                    addu    $v0, $a0, $s6
                    slti    $a0, $a1, 7
                    bnez    $a0, L93
                    addiu   $s2, $v0, 0xE6C
                    lw      $a2, OutStream($sp)
                    subu    $v0, $s7, $s5
                    addu    $v0, $a2, $v0
                    lbu     $s3, 0($v0)
                    sll     $s3, 1
LSM41:              sll     $s0, $v1, 1
                    andi    $s1, $s3, 0x100
                    sll     $a0, $s1, 1
                    addu    $a0, $s2, $a0
                    addu    $a0, $s0
                    bal     Check_Bound
                    addiu   $a0, 0x200
                    beqz    $v0, LSM60
                    daddu   $v1, $s0, $0
                    bnez    $s1, LSM57
                    slti    $v0, $s0, 0x100
LSM45:              slti    $v0, $v1, 0x100
                    bnezl   $v0, LSM41
                    sll     $s3, 1
                    andi    $s0, $v1, 0xFF
LSM47:              lw      $a0, State($sp)
                    lw      $v1, OutStream($sp)
                    addu    $v0, $v1, $s7
                    addiu   $s7, 1
                    slti    $v1, $a0, 4
                    beqz    $v1, LSM51
                    sb      $s0, 0($v0)
                    lw      $a2, OutSize($sp)
                    sw      $0,  State($sp)
                    sltu    $a1, $s7, $a2
L8:                 bnezl   $a1, LSM31
                    lw      $v1, State($sp)

End_Depack:         daddu   $a0, $t7, 0
Run_Datas:          j       0x02345678          # Entry point 0x7a2700 #

LSM51:              lw      $v0, State($sp)
                    lw      $a2, OutSize($sp)
                    addiu   $a0, $v0, 0xFFFA
                    slti    $v1, $v0, 0xA
                    addiu   $v0, 0xFFFD
                    sltu    $a1, $s7, $a2
                    movz    $v0, $a0, $v1
                    b       L8
                    sw      $v0, State($sp)
LSM57:              beqz    $v0, LSM47
                    andi    $s0, $v1, 0xFF
                    sll     $s0, $v1, 1
L102:               bal     Check_Bound
                    addu    $a0, $s2, $s0
                    daddu   $v1, $s0, $0
                    addiu   $s0, 1
                    movz    $v1, $s0, $v0
LSM59:              slti    $v0, $v1, 0x100
                    bnezl   $v0, L102
                    sll     $s0, $v1, 1
                    b       LSM47
                    andi    $s0, $v1, 0xFF
LSM60:              bnez    $s1, LSM45
                    addiu   $v1, $s0, 1
                    b       LSM57
                    slti    $v0, $v1, 0x100
L93:                b       LSM57
                    li      $v0, 1
LSM62:              lw      $v1, State($sp)
                    sll     $v0, $v1, 1
                    addu    $s0, $s6, $v0
                    bal     Check_Bound
                    addiu   $a0, $s0, 0x180
                    beqz    $v0, LSM154
                    lw      $a0, State($sp)
                    li      $a2, 3
                    lw      $a1, var_B0($sp)
                    addiu   $s2, $s6, 0x664
                    lw      $v1, var_B4($sp)
                    slti    $v0, $a0, 7
                    movn    $a2, $0, $v0
                    sw      $a1, var_AC($sp)
                    sw      $v1, var_B0($sp)
                    sw      $a2, State($sp)
                    sw      $s5, var_B4($sp)
LSM73:              bal     Check_Bound
                    daddu   $a0, $s2, $0
                    beqz    $v0, LSM144
                    sll     $v0, $s1, 4
                    daddu   $fp, $0, $0
                    addu    $v0, $s2, $v0
                    li      $s4, 3
                    addiu   $s2, $v0, 4
LSM78:              daddu   $s1, $s4, $0
                    li      $s3, 1
LSM80:              sll     $s0, $s3, 1
                    addiu   $s1, 0xFFFF
                    addu    $a0, $s2, $s0
                    bal     Check_Bound
                    daddu   $s3, $s0, $0
                    addiu   $s0, 1
                    bnez    $s1, LSM80
                    movz    $s3, $s0, $v0
                    lw      $v0, State($sp)
                    slti    $v1, $v0, 4
                    li      $v0, 1
                    sllv    $v0, $v0, $s4
                    subu    $s3, $v0
                    beqz    $v1, LSM119
                    addu    $s3, $fp
                    slti    $v1, $s3, 4
                    li      $v0, 3
                    movn    $v0, $s3, $v1
                    li      $s1, 6
                    lw      $v1, State($sp)
                    sll     $v0, 7
                    addu    $v0, $s6, $v0
                    li      $a1, 1
                    addiu   $v1, 7
                    addiu   $s2, $v0, 0x360
                    sw      $v1, State($sp)
LSM97:              sll     $s0, $a1, 1
                    addiu   $s1, 0xFFFF
                    bal     Check_Bound
                    addu    $a0, $s2, $s0
                    daddu   $a1, $s0, $0
                    addiu   $s0, 1
                    bnez    $s1, LSM97
                    movz    $a1, $s0, $v0
                    addiu   $a1, 0xFFC0
                    slti    $v0, $a1, 4
                    bnez    $v0, LBE24
                    daddu   $s5, $a1, $0
                    sra     $a0, $a1, 1
                    andi    $v0, $a1, 1
                    slti    $v1, $a1, 0xE
                    ori     $s5, $v0, 2
                    beqz    $v1, LSM128
                    addiu   $s1, $a0, 0xFFFF
                    sllv    $s5, $s5, $s1
                    sll     $v1, $a1, 1
                    sll     $v0, $s5, 1
                    addu    $v0, $s6, $v0
                    subu    $v0, $v1
                    addiu   $s2, $v0, 0x55E
LBB24:              li      $s4, 1
                    li      $v1, 1
LSM113:             sll     $s0, $v1, 1
                    bal     Check_Bound
                    addu    $a0, $s2, $s0
                    bnez    $v0, LSM116
                    daddu   $v1, $s0, $0
                    addiu   $v1, $s0, 1
                    or      $s5, $s4
LSM116:             addiu   $s1, 0xFFFF
                    bnez    $s1, LSM113
                    sll     $s4, 1
LBE24:              addiu   $s5, 1
LSM119:             lw      $a0, OutStream($sp)
                    addiu   $s3, 2
                    subu    $v0, $a0, $s5
                    addu    $v1, $s7, $v0
                    lw      $a1, OutStream($sp)
LSM124:             addiu   $s3, 0xFFFF
                    lbu     $s0, 0($v1)
                    addiu   $v1, 1
                    addu    $v0, $a1, $s7
                    addiu   $s7, 1
                    sb      $s0, 0($v0)
                    lw      $a2, OutSize($sp)
                    beqz    $s3, L95
                    sltu    $a1, $s7, $a2
                    bnezl   $a1, LSM124
                    lw      $a1, OutStream($sp)
                    b       End_Depack
L95:                lw      $v0, OutSize($sp)
                    b       L8
                    sltu    $a1, $s7, $v0
LSM128:             lui     $a3, 0xFF
                    addiu   $s1, $a0, 0xFFFB
                    li      $a3, 0xFFFFFF
                    lw      $a1, Range($sp)
L104:               addiu   $s1, 0xFFFF
                    sll     $s5, 1
                    sltu    $v0, $a3, $a1
                    bnez    $v0, LSM135
                    sll     $v1, $a1, 8
                    lw      $v0, Buffer($sp)
                    daddu   $a1, $v1, $0
                    sw      $v1, Range($sp)
                    lw      $v1, Code($sp)
                    lbu     $a0, 0($v0)
                    addiu   $v0, 1
                    sll     $v1, 8
                    sw      $v0, Buffer($sp)
                    or      $v1, $a0
                    sw      $v1, Code($sp)
LSM135:             lw      $v1, Code($sp)
                    srl     $v0, $a1, 1
                    subu    $a0, $v1, $v0
                    sltu    $v1, $v0
                    bnez    $v1, LSM140
                    sw      $v0, Range($sp)
                    sw      $a0, Code($sp)
                    ori     $s5, 1
LSM140:             bnez    $s1, L104
                    lw      $a1, Range($sp)
                    addiu   $s2, $s6, 0x644
                    sll     $s5, 4
                    b       LBB24
                    li      $s1, 4
LSM144:             bal     Check_Bound
                    addiu   $a0, $s2, 2
                    beqzl   $v0, LSM152
                    addiu   $s2, 0x204
                    sll     $v0, $s1, 4
                    li      $fp, 8
                    addu    $v0, $s2, $v0
                    li      $s4, 3
                    b       LSM78
                    addiu   $s2, $v0, 0x104
LSM152:             li      $fp, 0x10
                    b       LSM78
                    li      $s4, 8
LSM154:             bal     Check_Bound
                    addiu   $a0, $s0, 0x198
                    beqz    $v0, LBB29
                    lw      $a1, State($sp)
                    sll     $v0, $s1, 1
                    sll     $a0, $a1, 5
                    addu    $a0, $s6, $a0
                    addu    $a0, $v0
                    bal     Check_Bound
                    addiu   $a0, 0x1E0
                    beqzl   $v0, L105
                    lw      $a1, State($sp)
                    lw      $a0, OutStream($sp)
                    subu    $v0, $s7, $s5
                    lw      $a2, State($sp)
                    li      $a1, 9
                    addu    $v0, $a0, $v0
                    addu    $a0, $s7
                    lbu     $s0, 0($v0)
                    slti    $v1, $a2, 7
                    li      $v0, 0xB
                    lw      $a2, OutSize($sp)
                    movz    $a1, $v0, $v1
                    addiu   $s7, 1
                    sw      $a1, State($sp)
                    sltu    $a1, $s7, $a2
                    b       L8
                    sb      $s0, 0($a0)
LSM168:             lw      $a1, State($sp)
L105:               li      $a2, 8
                    li      $v0, 0xB
                    addiu   $s2, $s6, 0xA68
                    slti    $v1, $a1, 7
                    movz    $a2, $v0, $v1
                    b       LSM73
                    sw      $a2, State($sp)

LBB29:              bal     Check_Bound
                    addiu   $a0, $s0, 0x1B0
                    beqz    $v0, LSM174
                    lw      $v1, var_B4($sp)
LSM172:             sw      $s5, var_B4($sp)
                    b       LSM168
                    daddu   $s5, $v1, $0
LSM174:             bal     Check_Bound
                    addiu   $a0, $s0, 0x1C8
                    bnez    $v0, LSM179
                    lw      $v1, var_B0($sp)
                    lw      $v0, var_B0($sp)
                    lw      $v1, var_AC($sp)
                    sw      $v0, var_AC($sp)
LSM179:             lw      $a0, var_B4($sp)
                    b       LSM172
                    sw      $a0, var_B0($sp)
Fin:
