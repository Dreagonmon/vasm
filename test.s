STOR 0x7F rc0
CALL ldisp
STOR 0x80 rc0
CALL ldisp
STOR 0x81 rc0
CALL ldisp
JUMP lreach_end

; display number and wait for key A
:ldisp
STOR 0 rc3
DNUM rc3 rc0
:ldisp_wait_key
GKEY
MOVE r0 rc0
; check is key down
STOR 64 rc3
DIVI rc3
STOR 0b01 rc3
SUBI rc3
JPEZ ldisp_check_key
JUMP ldisp_wait_key
:ldisp_check_key
; check is key A
MOVE rc0 r0
STOR 64 rc3
MODI rc3
STOR 0b000001 rc3
SUBI rc3
JPEZ ldisp_return
JUMP ldisp_wait_key
:ldisp_return
NOPE
RETN

; end of program
:lreach_end
NOPE
:lend
JUMP lend
