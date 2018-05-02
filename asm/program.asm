       title таймер.asm
text segment
assume cs:text,ss:stk,ds:text
	mov ax,stk
	mov ss,ax
	myproc proc
	mov ah,25h
	mov al,1ch
	lea dx,alarm
	push cs
	pop ds	
	int 21h
	mov si,700
hhh:    mov di,0ffffh
pause:  dec di
	cmp di,1
	jne pause
	dec si
	cmp si,1
	jne hhh
	push ds
	mov dx,0ff53h
	mov ax,0f000h
	mov ds,ax
	mov ax,251ch
	int 21h
	pop ds
	mov ax,4c00h
	int 21h

alarm:	push ax
	push bx
	push es
	dec cs:count
	cmp cs:count,0
	je output
	jmp short retret
output: dec cs:nmb
	cmp cs:nmb,0
	je retret
	mov ax,cs:count1
	mov cs:count,ax
	mov ax,0b800h
	mov es,ax
	mov bx,cs:pos
	mov al,cs:sym
	mov byte ptr es:[bx],al
	inc bx
	mov byte ptr es:[bx],1fh
	inc cs:sym
	inc bx
	mov cs:pos,bx
retret:	pop es
	pop bx
	pop ax
	iret

sym	db '0'
nmb	db 10+1
count	dw 18
count1	dw 18
pos	dw 80*2+2*2
text    ends
stk segment
	db 128 dup (?)
stk     ends
	end

	