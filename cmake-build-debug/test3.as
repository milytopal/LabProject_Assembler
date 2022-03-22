; file ps.as
    .extern LIST
    .extern W

    .extern BAR
MAIN: add r3,LIST
LOOP: prn BAR
      macro m1
        inc r6
        mov r3, W
      endm
      cmp val1,#-6
      m1
      dec K
mov r3, BAR
mov r3, W
mov r3, W
cmp W,#-9
mov r3, BAR
.entry MAIN
      sub LOOP[r10],r14
END:  stop

.extern val1
