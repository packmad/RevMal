; Build (x64 Native Tools Command Prompt):
;   ml64 /c /Fo testflags.obj flags.asm
;   link /nologo /entry:main /subsystem:console testflags.obj kernel32.lib

EXTERN  ExitProcess:PROC

.code
main PROC
    ; Call MyFunc
    call MyFunc

    mov rax, [r12 + rbx*4 + 12345678h] 
    ; Exit cleanly (no printing)
    xor   ecx, ecx
    call  ExitProcess
main ENDP

MyFunc PROC
    ; Inside the function
    nop

    ; Return to caller
    ret
MyFunc ENDP

END

