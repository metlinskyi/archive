	;
	cod segment
	assume es:cod, ds:dan
	mov ax,dan
	mov ds,ax
	;
	mov ax,351bh
	int 21h
	mov wordptr vec_1bh,bx
	mov wordptr vec_1bh+2,es
	;
	mov ax,251bh
	push ds
	push cs
	pop ds
	lea dx,break
	int 21h
	;
	mov ax,2523h
	lea dx,ctrlc
	int 21h
	pop ds
	;
   ring:mov ah,02h
	mov dl,sym
	int 21h
	;

	jmp ring
	;
  break:push ax
	push es
	push cx
	push si
	push di
	mov ax,0b800h ;
	mov es,ax
	mov di,offs;
	lea si,break_mes
	mov cx,len_break
	rep movsb
	add offs,170 ;
	inc sym      ;
	pop di
	pop si
	pop cx
	pop es
	pop ax
	iret
	;
	ctrl:
	;
	lds dx,vec_1bh
	mov ax,251bh
	int 21h
	;
	mov ax,4c00h
	int 21h
	cod ends
	dan segment
	vec_1bh dd 0
	sym db 'a'
	offs dw 800
	break_mes db 'b',4,'r',2,'e',1,'a',4,'k',2
	len_break equ $-break_mes
	dan ends
end
	