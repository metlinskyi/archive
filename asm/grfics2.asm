RED             EQU     04h
GREEN           EQU     02h
LIGHTRED        EQU     0Ch
LIGHTGREEN      EQU     0Ah
LIGTHBLUE       EQU     09h

DELAY MACRO time
        local   ext,iter
	push	cx
	mov	cx,time
ext:
	push	cx
	mov	cx,5000
iter:
	loop	iter
	pop	cx
	loop	ext
        pop     cx
ENDM

CODESG  SEGMENT  PARA  'Code'
         ASSUME SS:CODESG, CS:CODESG
         ORG     100H
MAIN    PROC    FAR
;------------ VIDEO MODE ---------------------------
        MOV     AH,00
        MOV     AL,0DH
        INT     10H

        MOV     AH,0BH
        MOV     BH,00
        MOV     BL,00H
        INT     10H
;---------------------------------------------------

;-------------- OUT TEXT ---------------------------
        MOV     AH,00H
        INT     16H

        MOV     BL,0AH
@ANIM1:
        INC     BL
        MOV     DL,05H
;---------------------------------------------------
        CALL    SET_XY
        DELAY   8000
;--------------   'L'  ---------------------------
        MOV     AH,09H
        MOV     AL,4CH
        MOV     BH,00H
        INC     BL
        MOV     CX,01H
        INT     10H
;---------------------------------------------------
        CALL    SET_XY
        DELAY   7000
;--------------   'U'  ---------------------------
        MOV     AH,09H
        MOV     AL,55H
        MOV     BH,00H
        INC     BL
        MOV     CX,01H
        INT     10H
;---------------------------------------------------
        CALL     SET_XY
        DELAY   6000
;--------------   'C'  ---------------------------
        MOV     AH,09H
        MOV     AL,43H
        MOV     BH,00H
        INC     BL
        MOV     CX,01H
        INT     10H
;---------------------------------------------------
        CALL    SET_XY
        DELAY   5000
 ;--------------   'K'  ---------------------------
        MOV     AH,09H
        MOV     AL,4BH
        MOV     BH,00H
        INC     BL
        MOV     CX,01H
        INT     10H
;       JMP     @ANIM1

;-------------- DRAW LINE --------------------------
        MOV     BX,LIGTHBLUE
@ANIM2:
        MOV     CX,40
        MOV     DX,30
@1:
        DELAY   500
        MOV     AH,0CH
        MOV     AL,BL
        INT     10H
        INC     CX
        CMP     CX,94
        JNA     @1

        INC     BX
;       JMP     @ANIM2

;-------------  E X I T ----------------------------
;key press
exit:   MOV     AH,00H
        INT     16H
;init text mode
        MOV     AH,00
        MOV     AL,03H
        INT     10H
;quit programm:
        MOV     AL,0
	MOV	AH,04CH
	INT	21H
	RET
MAIN	ENDP
;-------------  PROCEDURE SET_XY ---------------------
SET_XY  PROC
        MOV     AH,02H
	MOV	BH,00H
        MOV     DH,02H
        INT     10H
        ADD     DL,02H
        RET
SET_XY  ENDP
;------------------------------------------------------

CODESG	ENDS
END
