ppix	macro	color
	pop cx
	pop dx
	push bx
	mov bh, 0
	mov al, color
	mov ah, 0Ch
	int 10h
	pop bx
	push dx
	push cx
endm

.model small

prg	segment para 'code'
	assume cs:prg, ds:dat
	mov ax, dat
	mov ds, ax
	
	mov ah, 0
	mov al, 12h
	int 10h

	mov bx, 0
row:	push bx
	mov ax, 0
pixel:	push ax
	
	sub ax, a
	jnc noneg1
	neg ax
noneg1:	mul ax
	mov aalo, ax
	mov aahi, dx
	
	mov ax, bx
	sub ax, b
	jnc noneg2
	neg ax
noneg2:	mul ax
	add ax, aalo
	adc dx, aahi
	sub ax, word ptr rr
	sbb dx, word ptr rr+2
	jnc nopix
	ppix 1
nopix:	pop ax
	inc ax
	cmp ax, 639
	ja nexty
	jmp pixel
nexty:	pop bx
	inc bx
	cmp bx, 479
	ja exit
	jmp row

exit:	mov ah, 8
	int 21h
	mov ah, 0
	mov al, 3
	int 10h
	mov ax, 4C00h
	int 21h
prg	ends

dat	segment para 'data'
a	dw	320
b	dw	240
rr	dd	67600 ;r^2
aalo	dw	?
aahi	dw	?
dat 	ends

stk	segment para stack 'stack'
	db 10 dup (?)
stk	ends
	end