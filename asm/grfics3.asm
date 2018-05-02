RED             EQU     04h
GREEN           EQU     02h
LIGHTRED        EQU     0Ch
LIGHTGREEN      EQU     0Ah
LIGTHBLUE       EQU     09h


DATASG  SEGMENT PARA 'Data'
        STRING  DB      'LUCKY'
DATASG  ENDS

CODESG	SEGMENT  PARA  'Code'
        ASSUME  CS: CODESG, DS:DATASG, SS: CODESG
        ORG     100H

MAIN	PROC	NEAR

;------------ VIDEO MODE ---------------------------
        MOV     AH,00H
        MOV     AL,0DH
        INT     10H

;---------------------------------------------------
;-------------- OUT TEXT ---------------------------

;-------------- DRAW LINE --------------------------



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
CODESG	ENDS
END
