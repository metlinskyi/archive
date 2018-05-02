page 60,132
TITLE    Program for input
;------------------------------------------------
STACKSG SEGMENT PARA STACK 'Stack'
        DW      32 DUP(?)
STACKSG ENDS
;------------------------------------------------
DATASG  SEGMENT PARA 'Data'
NAMEPAR    LABEL    BYTE    ;Имя списка параметров:
MAXNLEN    DB    20         ;   максимальная длинна ввода
NAMELEN    DB    ?          ;   число введенных символов
NAMEFLD    DB    20   DUP(' '),'$'
DATASG ENDS
;-------------------------------------------------
CODESG  SEGMENT PARA 'Code'
ASSUME CS:CODESG, DS:DATASG, SS:STACKSG
BEGIN:    MOV    AX, DATASG
          MOV    DS, AX
          MOV    AH, 0AH    ; Функция ввода
          LEA    DX, NAMEPAR
          INT    21H
          MOV AL, 0         ; Возврат в DOS
          MOV AH, 04CH
          INT 21H
CODESG ENDS
       END  BEGIN