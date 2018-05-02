;Лабораторная работа №1 Арифметические операции
MASM
MODEL small
STACK 256
.data
      a       dw   256
      b       dw   150
      f        dw   0
       
     mes1     db   "The program calculates function F=10+a*b+(a+b)(a+b) / (a-2).",'$'
     mes2     db   "The result can be looked in Turbo Debuger. $"  
.code
main:
     ;настройка программы
     mov     ax,@data
     mov     ds,ax
     
     ;вывод сообщений
     mov     ah,09h
     mov     dx,offset mes1
     int        21h
     mov     dx,offset mes2
     mov     ah,09h
     int        21h
     
      
    ;(a-2)
     mov ax,a
     sub  ax,2
     mov si,ax
    
     ;(a+b)^2
     mov   ax,a
     mov   bx,b
     add    ax,bx
     mul    ax
       
    ;(a+b)^2/(a-2)+10   
     div     si    
     add    ax,10
     mov   cx,ax	
     mov   ax,a
     mov   bx,b
     mul    bx
     add    cx,ax	
     mov   f,cx
     	 
     		
;выход из программы
     mov ax,4c00h
     int 21h
end main