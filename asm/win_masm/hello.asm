; Build: 
;   ml /c /coff hello.asm
;   link /subsystem:console /machine:x86 /entry:main hello.obj kernel32.lib

.386
.model flat, stdcall
option casemap:none

; --- Win32 constants and prototypes (no includes needed) ---
STD_OUTPUT_HANDLE EQU -11

ExitProcess PROTO :DWORD
GetStdHandle PROTO :DWORD
WriteFile    PROTO :DWORD, :DWORD, :DWORD, :DWORD, :DWORD

.data
    msg db "Hello from x86 assembly on Windows 11!", 13, 10
    msgLen EQU ($ - msg)

.data?
    bytesWritten dd ?

.code
main PROC
    ; HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    INVOKE GetStdHandle, STD_OUTPUT_HANDLE

    ; WriteFile(hOut, msg, msgLen, &bytesWritten, NULL);
    INVOKE WriteFile, eax, ADDR msg, msgLen, ADDR bytesWritten, 0

    ; ExitProcess(0);
    INVOKE ExitProcess, 0
main ENDP

END main