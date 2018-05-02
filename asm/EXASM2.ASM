                page 60,132
      TITLE     EXASM2 (EXE)
      ;------------------------------------------------------------
      STACKSG   SEGMENT PARA SACK 'Stack'
                DW      32 DUP(?)
                STACKSG ENDS
      ;------------------------------------------------------------
      DATASG    SEGMENT PARA 'Data'
      FLDA      DW      250
      FLOB      DW      125
      FLOC      DW      ?
      DATASG    ENDS
      ;------------------------------------------------------------
      CODESG    SEGMENT PARA 'Code'
      BEGIN     PROC    FAR
                ASSUME  CS:CODESG,DS:DATASG,SS:STACKSG,ES:NOTHING
                PUSH    DS
                SUB     AX,AX
                PUSH    AX
                MOV     AX,DATASG
                MOV     DS,AX
                MOV     AX,FLDA
                ADD     AX,FLOB
                MOV     FLOC,AX
                RET
      BEGIN     ENDP
      CODESG    ENDS
                END     BEGIN