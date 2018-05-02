	title Ctrl_C.asm
	prog segment
	assume es:prog, ds:dan, ss:stack
	mov ax,dan
	mov ds,ax
	;
	push ds
	push cs
	pop ds
	mov ah,25h
	mov al,23h
	mov dx,offset intr
	int 21h
	pop ds
	;
     go:mov ah,40h
	mov bx,1
	mov cx,len_mes
	mov dx,offset mes
	int 21h
	;
	mov si,40
 pause1:mov di,0feeeh
 pause2:dec di
	cmp di,1
	jne pause2
	dec si
	cmp si,1
	jne pause1
	dec count
	test count,0ffffh
	jne go
	;
	mov ax,4c00h
	int 21h
	;
   intr:push ax
	push bx
	push cx
	push dx
	;
	mov ah,40h
	mov bx,1
	mov cx,len_mes1
	mov dx,offset mes1
	int 21h
	;
	pop dx
	pop cx
	pop bx
	pop ax
	ei
	iret
   prog ends
	;
	dan segment
	nes db 'работает основная программа',0a,0d
	len_mes equ $-mes
	mes1 db 'получил управление обработчик Ctrl-C',0ah,0dh
	len_mes1 equ $-mes1
	count dw 10
	dan ends
	stack segment
	         db 10 dup('stack')
	stack ends
	end