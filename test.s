LOAD
STOR 0x7F rtest0
MOVE rtest0 r0
STOR 1 rtest0
ADDI rtest0
SUBI rtest0
MULI rtest0
DIVI rtest0
MODI rtest0
INVI
DNUM rtest0 r0
GKEY
NOPE
:lloop0
TICK
STOR &rtest0 rtest1
TILE 0x00 0b01 2 3 4 5 6 7 8
DTIL *rtest1 0x00
DNUM *rtest1 r0
DTXT *rtest1 "this is number \"string\""
DSEL *rtest1 "test1" "test2" "test3"
WKEY
JUMP lloop0
JPEZ lloop0
JPGZ lloop0
JPLZ lloop0
CALL lloop0
RETN
SAVE
EXIT
