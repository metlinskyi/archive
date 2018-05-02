masm 
model small
stack 256
.data

xcrnt dw 0
ycrnt dw 0
xtmp dw 0
ytmp dw 0
flagx dw 0
flagy dw 0
flagy1 dw 0
flagy2 dw 0
wave dw 0
iter dw 0	
radtemp dw 0

.code

rectangle macro x, y, xsize, ysize, color
   local loopx, loopy
	mov ax,y
	mov ycrnt,ax
	mov cx,ysize
	loopy:
   push cx
	mov  ax,x
	mov xcrnt,ax
	mov cx,xsize
   loopx:
	push cx
	mov ah,0Ch
	mov al,color
	mov cx,xcrnt
	mov dx,ycrnt
	int 10h
	inc xcrnt
	pop cx
	loop loopx
	inc ycrnt
	pop cx
	loop loopy
endm

circle macro x, x0, y0, rad, color
  local loppx, loopy, ml
	mov ax,y0
	sub ax,rad
	mov ycrnt,ax
	mov cx,rad
	add cx,rad
  loopy:
	push cx
	mov ax,x
	mov xcrnt,ax
	mov cx,rad
	add cx,rad
  loopx:
	push cx
	mov bx,xcrnt
	sub bx,x0
	mov ax,bx
	mul bx
	mov xtmp,ax
	mov bx,ycrnt
	sub bx,y0
	mov ax,bx
	mul bx
	mov ytmp,ax
	add xtmp,ax
	mov ax,rad
	mul ax
	cmp ax,xtmp

	jl ml

	mov ah, 0Ch
	mov al,color
	mov bh,0
	mov cx,xcrnt
	mov dx,ycrnt
	int 10h
    ml:
	inc xcrnt
	pop cx

    	loop loopx

	inc ycrnt
	pop cx

    	loop loopy
endm

delay macro time
  local   ext,iter
	push	cx
	mov	cx,time
  ext:
	push	cx
	mov	cx,5000
  iter:
	loop	iter
	pop	cx
	loop	ext
      	pop     cx
endm


hline  macro x1, y1, x2, color
local  @1
        	mov     BX,color
        	mov     CX,x1
        	mov     DX,y1
@1:
        	mov     AH,0CH
        	mov     AL,BL
       	int    10H
       	inc     CX
        	cmp     CX,x2
       	jna     @1
endm

vline macro x1, y1, y2, color
local @1
        	mov     BX,color
       	mov     CX,x1
        	mov     DX,y1
@1:
        	mov     AH,0CH
        	mov     AL,BL
        	int     10H
        	inc     DX
        	cmp     DX,y2
       	jna     @1
endm


main:
	mov ax,@data
	mov ds,ax

	mov ah,00h
	mov al,0Dh
	int 10h

	;rectangle 70,50,179,50,1
	;rectangle 70,100,179,50,14





	mov cx,50
	mov wave,50
anim:

	mov ax,wave
	add ax,1
	mov wave,ax

	mov flagy,ax
	mov ax,flagy
	add ax,50 
	mov flagy1,ax

	mov ax, flagy1
	add ax,50
	mov flagy2,ax

	vline cx,flagy,flagy1,1
	vline cx,flagy1,flagy2,14
	
	add iter,1

	inc cx 		
	cmp cx,200
	jna anim
	



		


	mov ah,08h
	int 21h

	mov ah,00h
	mov al,03h
	int 10h

	mov  ax,4c00h
	int 21h
end main
