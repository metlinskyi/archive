masm
model small
.stack 100h
.data
     len_vv            equ      10 
     stroka_vv       db        10 dup(?) 
     message1       db        0ah,0dh, 'Input: ',0ah,0dh,' $ '
     message2       db        0ah,0dh, 'HEX: ',0ah,0dh,' $ '
     adr_str_vv      dd        stroka_vv
     mas                db        4 dup (?)
     mas16            db        4 dup(?)  
     tabl                 db        48 dup(?) ,0,1,2,3,4,5,6,7,8,9, 7 dup (?)
                            db        0Ah,0Bh,0Ch,0Dh,0Eh,0Fh, 184 dup(?) 
.code
         assume ds:@data,es:@data
    start:
          mov         ax,@data
          mov         ds,ax
          mov         es,ax
          xor          ax,ax
          mov         ah,09h
          lea           dx,message1
          int            21h
          mov         cx,len_vv
          lea           di,stroka_vv
met1:
          mov         ah,01h
          int            21h
          stos         stroka_vv
          loop         met1
          mov         cx,len_vv
          lea           bx,stroka_vv
          mov         bp,4
          mov         si,0 
met2:
          mov         al,[bx]
          cmp         al,30h
          jb             next
          cmp         al,46h
          ja             next
          cmp         al,39h
          jbe           rec
          cmp         al,41h
          jl              next           
rec:
          mov         mas[si],al
          inc           si
          dec          bp
          cmp         bp,0
          je             met3
next:
          inc           bx
          dec          cx
          cmp         cx,0
          jne           met2
met3:
           mov        ah,09h
           lea          dx,message2
           int           21h
           mov        cx,4
           mov        si,0
show_Hex:
           mov        ah,02h
           mov        dl,mas[si]
           int           21h
           inc          si
           loop        show_Hex 
           mov        si,0
           mov        cx,3
           mov        di,0
           xor         ax,ax
           lea          bx,tabl
           mov        al,mas[si]
           xlat
           mov         mas16[di],al
           mov        dl,al
           shl          dl,4
           inc          si
met4:      
           mov        al,mas[si] 
           xlat
           add         dl,al
           shl          dl,4
           inc          si
           loop        met4  
                     
           mov       ax,4c00h
           int          21h
      end  start 