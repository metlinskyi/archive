                page 60,132
      TITLE     GRAPH (EXE)

RED             EQU     04h
GREEN           EQU     02h
LIGHTRED        EQU     0Ch
LIGHTGREEN      EQU     0Ah
LIGTHBLUE       EQU     09h

DATASG  SEGMENT PARA 'Data'
        STR_NAME        DB      4Ch
DATASG  ENDS

CODESG	SEGMENT  PARA  'Code'
        ASSUME  CS: CODESG, DS:DATASG, SS: CODESG
        ORG     100H

MAIN	PROC	NEAR

        ; init graphics mode
        MOV     AH,00
        MOV     AL,0DH
        INT     10H

        MOV     AH,0Bh
        MOV     BH,00h
        MOV     BL,08h
        INT     10H
jmp exit

        ;-----------------------

        ;output text
        MOV     DH,0Ah
        MOV     DL,0Ah
@shadow:
        MOV     AH,02h
        MOV     BH,00h
        INT     10H

        MOV     AH,09H
        MOV     AL,41h           ;item char
        MOV     BH,00H
        MOV     BL,LIGTHBLUE    ; color
        MOV     CX,01H         ; repeat char
        INT     10h

        ADD     DL,02h
        CMP     DL,12h
        JNA     @shadow


        ;output text
        MOV     DH,09h
        MOV     DL,0Bh
@text:
        MOV     AH,02h
        MOV     BH,00h
        INT     10H

        MOV     AH,09H
        MOV     AL,41h           ;item char
        MOV     BH,00H
        MOV     BL,RED    ; color
        MOV     CX,01H         ; repeat char
        INT     10h

        ADD     DL,02h
        CMP     DL,13h
        JNA     @text


;key press
exit:   MOV     AH,00H
	INT	16H
;init text mode
        MOV     AH,00
        MOV     AL,03H
        INT     10H

        ;---------------------
 ; quit programm:
        MOV     AL,0
	MOV	AH,04CH
	INT	21H
	RET

MAIN	ENDP
CODESG	ENDS
END
