    lw 0 1 data
    lw 0 2 more
    lw 0 3 extra
    add 1 2 3
    beq 1 3 L1
    beq 0 0 next
L1 add 1 1 2
next halt
data .fill 9
more .fill 8
extra .fill 7