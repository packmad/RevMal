; Build (x64 Native Tools Command Prompt):
;   ml64 /c /Fo testflags.obj flags.asm
;   link /nologo /entry:main /subsystem:console testflags.obj kernel32.lib

EXTERN  ExitProcess:PROC

.code
main PROC
    xor rax, rax

    mov al,255; 
    add al,1
    
    mov al,1; 
    sub al,2
    
    mov al,127; 
    add al,1
    
    mov al,-128; 
    sub al,1

    ; Exit cleanly (no printing)
    xor   ecx, ecx
    call  ExitProcess
main ENDP

END
