        lw          0       1       n
        lw          0       2       r
        lw          0       4       Faddr
        jalr        4       7
        halt
comb    lw          0       6       one
        sw          5       7       Stack
        add         5       6       5
        sw          5       1       Stack
        add         5       6       5
        sw          5       2       Stack
        add         5       6       5
        sw          5       4       Stack
        add         5       6       5
        beq         2       0       base
        beq         1       2       base
        lw          0       4       Faddr
        lw          0       6       none
        add         1       6       1
        jalr        4       7
        lw          0       6       one
        sw          5       3       Stack
        add         5       6       5
        lw          0       6       none
        add         5       6       5
        add         5       6       5
        lw          5       2       Stack
        add         5       6       5
        lw          5       1       Stack
        lw          0       6       one
        add         5       6       5
        add         5       6       5
        add         1       6       1
        add         2       6       2
        lw          0       4       Faddr
        jalr        4       7
        lw          0       6       none
        add         5       6       5
        lw          5       4       Stack
        lw          0       6       one
        add         5       6       5
        add         3       4       3
        beq         0       0       retrn
base    lw          0       6       one
        add         0       6       3
retrn  lw          0       6       none
        add         5       6       5
        lw          5       4       Stack
        add         5       6       5
        lw          5       2       Stack
        add         5       6       5
        lw          5       1       Stack
        add         5       6       5
        lw          5       7       Stack
        lw          0       6       one
        add         5       6       5
        jalr        7       4
Faddr   .fill       comb
n       .fill       7
r       .fill       3
one    .fill       1
none    .fill       -1