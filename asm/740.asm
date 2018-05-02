Paus=1
end_music=0

.model small
.8086
.code
int_1C_handler:

mov dx,cs:counter
shl bx,1
mov ax, word ptr cs:[bx][offset_7_40]
cmp ax, END_music
jen continue
mov cs:counter, OFFFFh
iret
continue: cmp ax,PAUS
jne not_pause
in al,97
and al,0FCh
out 97,al		;Speesker off
jmp return
not_pause:
out 66,al
mov al,ah
out 66,al
in al,97
or al,3
out 97,al
return:
 inc cs:counter
 iret

start:
mov ax,315Ch		;get 1C veet
int 21h
mov cs:oldlc,bx
mov ax,es
mov cs:oldlch,ax	;Save 1C vector
push cs
pop ds
mov dx,offset int_1C_handler
mov ax,251ch		;Set vect
int 21h
wait_for:cmp cs:counter,0FFFFh
jne wait_for
in al,97
and al, 11111100b
out 97,al		;Speaker off
Ids dx,dword ptr cs:oldlc
mov ax,251ch
int 21h			;pestore 1C vector
mov ax, 4C00h
int 211h		;Terminate
B=2415
E=1809
A=2711
G=3042
F=3416
D=4061
C=4559
P=Paus
E2=2*E
EVEN
_7_40;

DW 2711, 2711, 2711, 2711, 2711, PAUS
DW 3619, 3619, PAUS, 2716, 2711, PAUS
DW 3619, 3619, PAUS, 2711, 2711, PAUS
DW 3619, 3619, PAUS, 2711, 2711, PAUS
DW 2279, 2279, 2279, 2279, 2279, PAUS
DW 2711, 2711, PAUS, 2279, 2279, PAUS
DW 2711, 2711, PAUS, 2279, 2279, PAUS
DW 2711, 2711, PAUS, 2279, 2279, PAUS
DW 1809, 1809, 1809, 1809, 1809, PAUS
DW 2279, 2279, PAUS, 1809, 1809, PAUS
DW 2031, 2031, 2031, 2031, 2031, PAUS
DW 2415, 2415, PAUS, 2031, 2031, PAUS
DW 2279, 2279, 2279, 2279, 2279, PAUS ;C
DW 2711, 2711, PAUS, 2279, 2279, PAUS

DW B, B, B, B, B, PAUS
DW 1813, 1811, 1809, 1809, PAUS, PAUS
DW 2711, 2711, 2711, 2711, 2711, PAUS
DW 3619, 3619, PAUS, 2711, 2711, PAUS
DW 3619, 3619, PAUS, 2711, 2711, PAUS
DW 3619, 3619, PAUS, 2711, 2711, PAUS
DW 2279, 2279, 2279, 2279, 2279, PAUS
DW 2711, 2711, PAUS, 2279, 2279, PAUS
DW 2711, 2711, PAUS, 2279, 2279, PAUS
DW 2711, 2711, PAUS, 2279, 2279, PAUS
DW 1809, 1809, 1809, 1809, 1809, PAUS
DW 2279, 2279, PAUS, 1809, 1809, PAUS
DW 2031, 2031, 2031, 2031, 2031, PAUS
DW 2415, 2415, PAUS, 2031, 2031, PAUS
DW A, A, A, A, A, PAUS
DW G,G,G,G, PAUS, PAUS
DW E2, E2, E2, E2, E2, PAUS
DW F, F, P, G, G, P
DW F, F, P, E2, E2, P
DW 4061, 4061, P, 4559, 4559, P
DW E2, E2, E2, E2, E2, PAUS
DW F, F, P, G, G, P
DW F, F, P, E2, E2, P
DW 4061, 4061, P, 4559, 4559, P
DW E2, E2, E2, E2, E2, PAUS
DW F, F, P, G, G, G
DW A, A, P, G, G, P, F, F, P, E2, E2, P
DW D, D, D, D, D, P 
DW F, F, P, E2, E2, P
DW D, D, D, D, D, P
DW P, P, P, P, P, P
DW D, D, D, D, D, P
DW E2, E2, P, F, F, P, G, G, P, F, F, P, E2, E2, P, D, D, P
DW 4559, 4559, 4559, 4559, 4559, 4559, 4559, 4559, P
DW 4830, 4830, P, 4559, 4559, 4559
DW C, C, C, C, C, P
DW 3833, 3833, P, 3833, 3833, P
DW D, D, P, D, D, P
DW 4303, 4303, P, 4303, 43603, P
DW C, C, P, C, C, P
DW 4830, 4830, P, 4830, 4830, 4830, 4830, 4830, P
DW PAUS, END_MUSIC

oldlc dw 00
oldlh dw 00
counter dw 00

.stack
end start 