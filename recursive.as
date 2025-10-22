        lw      0       1       n
        lw      0       2       r
        lw      0       4       Faddr
        jalr    4       7
        halt
comb    sw      5       7       Stack
        lw      0       6       one
        add     5       6       5
        sw      5       1       Stack
        add     5       6       5
        sw      5       2       Stack
        add     5       6       5
        beq     2       0       base
        beq     1       2       base
        lw      0       6       none
        add     1       6       1
        lw      0       6       Faddr
        jalr    6       7
        add     3       0       4
        lw      0       6       none
        add     5       6       5
        lw      5       2       Stack
        add     5       6       5
        lw      5       1       Stack
        lw      0       6       none
        add     1       6       1
        add     2       6       2
        lw      0       6       Faddr
        jalr    6       7
        add     4       3       3
        lw      0       6       none
        add     5       6       5
        lw      5       7       Stack
        jalr    7       4
base    lw      0       3       one
        lw      0       6       none
        add     5       6       5
        add     5       6       5
        add     5       6       5
        lw      5       7       Stack
        jalr    7       4
n       .fill   7
r       .fill   3
one    .fill   1
none    .fill   -1
Faddr   .fill   comb
