sound  proc
       in al,61h
       or al,3
       out 61h,al
       mov al,10110110b
       out 43h,al
       mov dx,12h
       mov ax,3428h
       div word ptr [bx]
       out 42h,al
       mov al,ah
       out 42h,al
pause1: push cx
        mov cx,0ffh
pause2: push cx
        mov cx,offffh
pause3: loop pause3
        pop cx
        loop pause2
        pop cx
        loop pause1
        in al,61h
        and al,0fch
        out 61h,al
        ret
sound  endp
