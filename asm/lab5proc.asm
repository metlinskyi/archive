
DATASG  SEGMENT PARA 'Data'
mas1	db	6,5,4,3,2,1,0,6
mas2	db	1,3,2,2,1,8,7,3
mas3	db	8 dup (?)
DATASG ENDS

CODESG  SEGMENT PARA 'Code'
ASSUME CS:CODESG, DS:DATASG
BEGIN: 

first    PROC   FAR
	 mov	ax,DATASG
	 mov	ds,ax
	 ;xor	ax,ax	
         call   second
         mov	si,0
         mov     ah,02h
	 mov	cx,8
    m1:  mov     dl,mas3[si]
	 add	dl,30h
         int     21h
         inc     si
         loop    m1  
         MOV AH, 04CH
         INT 21H 
    ret
first  ENDP 

second        PROC    NEAR
             
         mov si,0
         mov cx,8
         m3:  mov     ah,mas1[si]          	
         cmp   	ah,mas2[si]        
         jbe   m0                    		
	 cmp	mas1[si],2
         jb    m0                     
         cmp  	mas1[si],5
         ja    m0                    
         mov     mas3[si],ah
         jmp   m2 
      m0:      mov     mas3[si],0
      m2:      inc     si	
	loop	m3	

        RET
second     ENDP

CODESG ENDS
       END  BEGIN
