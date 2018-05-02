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

HLINE MACRO x1,y1,x2,color
        MOV     BX,color
        MOV     CX,x1
        MOV     DX,y1
@1:
        MOV     AH,0CH
        MOV     AL,BL
        INT     10H
        INC     CX
        CMP     CX,x2
        JNA     @1
ENDM

VLINE MACRO x1,y1,y2,color
        MOV     BX,color
        MOV     CX,x1
        MOV     DX,y1
@2:
        MOV     AH,0CH
        MOV     AL,BL
        INT     10H
        INC     DX
        CMP     DX,y2
        JNA     @2
ENDM

FILL MACRO x1,y1,color

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


;-------------- DRAW LINE --------------------------
     HLINE 100,50,200,LIGTHBLUE
     VLINE 100,50,100,LIGTHBLUE

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

CODESG  ENDS
END
