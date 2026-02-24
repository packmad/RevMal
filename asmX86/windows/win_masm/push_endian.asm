; Build (x64 Native Tools Command Prompt):
;   ml64 /c /Fo testflags.obj flags.asm
;   link /nologo /entry:main /subsystem:console testflags.obj kernel32.lib

EXTERN  ExitProcess:PROC

.code
main PROC
    push 12345678h
    push 11223344h
    pop rax
    pop rbx

    sub rsp, 28h
    xor   ecx, ecx
    call  ExitProcess
main ENDP

END