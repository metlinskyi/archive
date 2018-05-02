;***********************************************************************
;
;  Program PG0800 ( Chapter 8 )
;
;  Output 24 lines of text onto the screen
;
;  Author: A.I.Sopin, VSU, Voronezh, 1992
;
;  The BIOS video service (interrupt 10h) is used
;
;***********************************************************************
.model  small

EXTRN   VIDTYP : FAR

LF      EQU     10      ;  Line Feed
CR      EQU     13      ;  Carriage Return

;----------------------------------------------------------
.stack
.data
STRING  DB      80 dup ('E'), 0
Nstr    DB      24
TEXT0   DB      ' Output to Screen (BIOS  INT  10h).  Press any key...'
	DB      CR, LF, '$'
TEXT1   DB      ' Video Type = '
VTypeS  DB      'X'
	DB      '   MODE = '
VModeS  DB      'X'
	DB      '    Press any key...$'
MODE    DB      0
VMODE   DB      0               ; original video mode saved
;----------------------------------------------------------
.code
.startup
	mov     ah,0Fh                  ;  function 0Fh - get video mode
	int     10h                     ;  BIOS video service call
	mov     VMODE,al                ;  save original video mode
	mov     ah,0                    ;  function 00h - set video mode
	mov     al,2                    ;  video mode 2 - 80x25  B & W
	int     10h                     ;  BIOS video service call
	mov     ah,2                    ;  function 02h - set cursor
	xor     dx,dx                   ;  cursor position 0, 0
	xor     bh,bh                   ;  video page 0 is used
	int     10h                     ;  BIOS video service call
	lea     dx,TEXT0                ;  address of text to be output
	mov     ah,9                    ;  function 09h - output text string
	int     21h                     ;  DOS service call
	xor     ah,ah                   ;  function 00h - get key
	int     16h                     ;  BIOS keyboard service call
;  determine and report the video adapter type and video mode
	Call    VIDTYP                  ;
	mov     VTypeS,al               ;  Video Adapter Type
	or      VTypeS,30h              ;  Bin ---> ASCII
	mov     ah,0Fh                  ;  function 0Fh - get video mode
	int     10h                     ;  BIOS video service call
	mov     VModeS,al               ;  Video Mode
	or      VModeS,30h              ;  Bin ---> ASCII
	lea     dx,TEXT1                ;  address of text to be output
	mov     ah,9                    ;  function 09h - output text string
	int     21h                     ;  DOS service call
	xor     ah,ah                   ;  function 00h - get key
	int     16h                     ;  BIOS keyboard service call
	mov     ah,2                    ;  function 02h - set cursor
	xor     dx,dx                   ;  cursor position 0, 0
	xor     bh,bh                   ;  video page 0 is used
	int     10h                     ;  BIOS video service call
;-------------------------------------------------------------------
;  Output  ASCIIZ  string onto the screen
	mov     Nstr,24                 ;  number of lines to be output
M0:     lea     si,STRING               ;
	Call    PUTSTR                  ;
	dec     Nstr                    ;
	jnz     M0                      ;
	mov     ah,0                    ;  function 00h - get key
	int     16h                     ;  BIOS keyboard service call
;-------------------------------------------------------------------
;  Finish the program and return to MS-DOS
Exit:   mov     al,VMODE                ;  AL - original video mode
	xor     ah,ah                   ;  function 00h - set video mode
	int     10h                     ;  BIOS video service call
	mov     ax,4C00h                ;  Return Code =0
	int     21h                     ;  BIOS video service call
;-------------------------------------------------------------------
;
;  Output ASCIIZ - string (80  characters) onto the screen
;
;  DS:SI  - start address of a string to be outpit
;
;-------------------------------------------------------------------
PUTSTR  PROC   NEAR
Cycle:  lodsb                           ;  next character into  AL
	and     al,al                   ;  EOL (NULL character) ?
	jz      EOPUT                   ;  if NULL, stop output and return 
	xor     bx,bx                   ;  video page 0 is used
	mov     ah,0Eh                  ;  function 0Eh - teletype output
	int     10h                     ;  BIOS video service call
	jmp     short Cycle             ;  to next character
EOPUT:  RETN                            ;  return to caller
PUTSTR  ENDP
;-------------------------------------------------------------------
	END
