.entry  LIST 
.extern  W
MAIN:  add    r3, LIST 
LOOP:  prn    #48

add    r3, LIST            ;Doesn't have an error
nope #1, #2
add #0, r0
bne END[r10]
bne END[r10
bne END[r9]
bne END[r16]
bne END[13]
add r4, 1000000000


  cmp val1, #-6
  bne    END[r15]
.entry  MAIN
  sub    LOOP[r10] ,r14   
END:   stop 
STR:               .string  
LIST:              .data   6, -9
                  .data   -100 
.entry  K 
K:                 .data   31 
.extern  val1