.entry  LIST 
.extern  W
MAIN:  add    r3, LIST 
LOOP:  prn    #48

9labelTest: prn    #48
TooLongOfALabelName1234567890lalalalala: prn    #48
AreTheyAllDigits!: prn    #48
LabelExists: prn    #48
LabelExists: prn    #48

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