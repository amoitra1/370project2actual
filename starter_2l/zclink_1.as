    lw 0 1 t0
    lw 0 2 d0
    add 1 2 3
    lw 0 4 d1
    add 3 4 3
    beq 0 0 cont
cont lw 0 5 t2
    halt
t0 add 0 0 0
t1 add 0 0 0
t2 add 0 0 0
d0 .fill 5
d1 .fill t1