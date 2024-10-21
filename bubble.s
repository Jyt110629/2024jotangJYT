    .data
array:  .word 3,1,4,5,2
size:   .word 5

    .text
    .globl_start
_start:
    la t0, array
    lw t1, size

outer_loop:
    addi t2, zero, 0

outer_cmp:
    bge t2, t1, done_outer

    addi t3, zero, 0

inner_loop:
    add t4,t3,t2
    bge t4,t1,outer_continue

    slli t5,t4,2
    add t6,t0,t5
    lw t7,0(t6)
    lw t8,4(t6)

    blt t7, t8, no_swap

    sw t8,0(t6)
    sw t7,4(t6)

no_swap:
    addi t3, t3, 1
    j inner_loop

outer_continue:
    addi t2,t2,1
    j outer_cmp

done_outer:
    li a0, 10
    ecall