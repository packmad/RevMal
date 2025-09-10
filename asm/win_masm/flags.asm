; Build (x64 Native Tools Command Prompt):
;   ml64 /c /Fo testflags.obj flags.asm
;   link /nologo /entry:main /subsystem:console testflags.obj kernel32.lib

EXTERN  ExitProcess:PROC

.code
main PROC
    xor rax, rax
    
    ; 255 + 1  (unsigned carry only → CF=1, OF=0)
    mov   al, 255
    add   al, 1

    ; 1 - 2 (borrow → CF=1, OF=0)
    mov   al, 1
    sub   al, 2

    ; -128 - 1 (signed overflow → CF=0, OF=1)
    mov   al, -128
    sub   al, 1

    ; 127 + 1  (signed overflow only → CF=0, OF=1)
    mov   al, 127
    add   al, 1

    ; Exit cleanly (no printing)
    xor   ecx, ecx
    call  ExitProcess
main ENDP
END
