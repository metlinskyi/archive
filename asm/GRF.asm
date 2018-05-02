RED         EQU 04h
GREEN       EQU 02h
LIGHTRED    EQU 0Ch
LIGHTGREEN  EQU 0Ah

DATASG  SEGMENT PARA 'Data'
        STRING  DB      'LUCKY'
DATASG  ENDS

CODESG	SEGMENT  PARA  'Code'
        ASSUME  CS: CODESG, DS: CODESG, SS: CODESG
        ORG     100H

MAIN	PROC	NEAR
	MOV	AH,00	      ; Установка графического режима
	MOV	AL,0DH	      ;
	INT	10H

        MOV     AH,0Bh
        MOV     BH,00
        MOV     BL,GREEN
        INT     10H



      ;установка курсора в позицию 4:4
	MOV	AH,02H
	MOV	BH,00H
	MOV	DH,04H
	MOV	DL,04H
	INT	10H

	;вывод пяти символов
	MOV	AH,09H	      	;
	MOV	AL,66	      	; код символа
	MOV	BH,00H	      	;
        MOV     BL,GREEN        ; цвет
        MOV     CX,01H          ; кол-во символов
	INT	10H


	;подчеркивание пяти символов
	MOV	BX,LIGHTRED	; Начальный  цвет;
	MOV	CX,32       	; Столбец
	MOV	DX,42       	; и строка

       @1:
	MOV	AH,0CH  	; Функция  вывода  точки
	MOV	AL,BL       	; Установить  цвет
	INT	10H		; BX, CX  и  DX  сохраняетс
	INC	CX		; Увеличить  столбец
	CMP	CX,70       	; Столбец=70 ?
	JNA	@1		; Нет - цикл

	; ждать нажатия клавиши
	MOV	AH,00H
	INT	16H
	; установить текстовый режим
	MOV	AH,00	       ; Установка текстового режима
	MOV	AL,03H	 	;
	INT	10H
	; выход из программы
	MOV	AL,0
	MOV	AH,04CH
	INT	21H
	RET

MAIN	ENDP
CODESG	ENDS
END
