                page 60,132
      TITLE     GRAPH (EXE)

RED             EQU     04h
GREEN           EQU     02h
LIGHTRED        EQU     0Ch
LIGHTGREEN      EQU     0Ah
LIGTHBLUE       EQU     09h


DATASG  SEGMENT PARA 'Data'
        STRING  DB      'LUCKY'
        FULL    DD      STRING
        XY      DW      0F0CH
DATASG  ENDS

CODESG	SEGMENT  PARA  'Code'
        ASSUME  CS: CODESG, DS:DATASG, SS: CODESG
        ORG     100H

MAIN	PROC	NEAR

;------------ VIDEO MODE ---------------------------
        MOV     AH,00
        MOV     AL,0DH
        INT     10H

        MOV     AH,0Bh
        MOV     BH,00h
        MOV     BL,GREEN
        INT     10H

;-------------- OUT TEXT --------------------------
@text:
        MOV     DX,XY

        MOV     AH,02h
        MOV     BH,00h
        INT     10H
        MOV     XY,DX


        LEA     SI,STRING

        MOV     AH,09H
        MOV     AL,[SI]    ;item char
        MOV     BH,00H
        MOV     BL,RED         ; color
        MOV     CX,01H         ; repeat char
        INT     10h

        MOV     DX,XY
        INC     SI
        ADD     DL,02h
        CMP     DL,17h
        MOV     XY,DX
        JNA     @text

;-------------- DRAW LINE --------------------------



;-------------  E X I T ---------------------------
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
