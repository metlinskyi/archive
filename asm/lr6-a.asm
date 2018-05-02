;28.11.2003 21:04
MASM
MODEL small
STACK 512

show_mes   macro    message
	mov	ah,09h
        lea     dx,message
        int     21h
endm
	;структура для ввода
	SBufer  struc
	  len_buf  db  40
	  len_in   db  0
	  buf_in   db  42 dup(20h)
        ends

.data
        namedir       db   "c:\dir2\a",0
	ptrnamedir    dd   namedir
	namedir1      db   "c:\lr6\new",0
	ptrnamedir1   dd   namedir1
        namefile      db   'b.txt',0
	ptrnamefile   dd   namefile
	mess1	      db   'Каталог удален',0ah,0dh,'$'
	mess2         db   'Создан новый каталог',0ah,0dh,'$'
	mess3         db   'Введите строку: ',0ah,0dh,'$'
	mess4         db   'Установлен атрибут скрытый',0ah,0dh,'$'
	messerr       db   '!!Ошибка!!',0ah,0dh,'$'
	bufer         SBufer <>
	handle	      dw   0
	messinfile    db   'Assembler Forever!!',0ah,0dh,'$'
	ptrmessinfile dd   messinfile
        d_cur_name    db 64 dup(20h),0
        point_d_cur_name dd d_cur_name
.code

main:
	mov ax,@data
	mov ds,ax

	;удаление каталога "а"
	lds dx,ptrnamedir
	mov ah,3ah
	int 21h
	;jc exit
	show_mes  mess1

	;создание каталога
	lds dx,ptrnamedir1
	mov ah,39h
	int 21h
	;jc  exit
	show_mes mess2

	;задание текущего каталога
	lds dx,ptrnamedir1
	mov ah,3bh
	int 21h
	;jc  exit

	;создание файла
	xor cx,cx
	mov bx,2
	mov dx,12h
	lds si,ptrnamefile
	mov ah,6ch
	int 21h
	;jc  exit

	mov handle,ax
	mov ah,42h
	xor al,al
	xor cx,cx
	xor dx,dx
	mov bx,handle
	int 21h

	show_mes mess3
	lea dx,bufer
	mov ah,0ah
	int 21h

	mov dl,0ah
	mov ah,2
	int 21h
	lea si,bufer.buf_in
	mov al,bufer.len_in
	cbw
	push si
	add si,ax
	inc si
	mov byte ptr[si],0ah
	pop dx
	mov bx,handle
	add ax,2
	mov cx,ax
	mov ah,40h
	int 21h

	;запись строки в файл
	lea dx,messinfile
	mov cx,18
	mov ah,40h
	int 21h

	;установка атрибута скрытый
	show_mes mess4
	xor cx,cx
	mov cx,2
	lds dx,ptrnamefile
	mov ax,4301h
	int 21h
	jmp exit_OK
exit:
	show_mes  messerr

exit_OK:
	mov ax,4c00h
 	int 21h
end main
