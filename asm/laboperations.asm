% title 'myprog'
DATASG  SEGMENT PARA 'Data'
massoge db 'pr sum'
var1    DW    90    
var2    DW    27
var3    DW    17
var4    DW     ?
DATASG ENDS
CODESG  SEGMENT PARA 'Code'
ASSUME CS:CODESG, DS:DATASG
BEGIN: 
 MOV    AX, DATASG
          MOV    DS, AX
MOV  DX,VAR1 ;in dx90
ADD  DX,VAR2 ;dx+27

SUB  DX,VAR3 ;dx-17
MOV  VAR4,DX ;invar4 dx
MOV  Ax,VAR3 ;inax var3
MOV  DX,2
MUL  DX
INC  AX
MOV  AX,50
MOV  DL,5
DIV  DL
;MOV  AX.111111111101b
;OR   AX,000000001101b
   MOV AH, 04CH
          INT 21H
CODESG ENDS
       END  BEGIN