#include <Windows.h>
#include <stdio.h>

void NTAPI TLSCallback(PVOID DllHandle, DWORD Reason, PVOID Reserved)
{
    if (Reason == DLL_PROCESS_ATTACH) {
        printf("Hi :)\n");
    }
    else if (Reason == DLL_PROCESS_DETACH) {
        printf("Bye :(\n");
    }
}

// Put the callback into the TLS directory
#ifdef _MSC_VER
#pragma comment(linker, "/INCLUDE:_tls_used")
#pragma comment(linker, "/INCLUDE:tls_callback_func")

// Allocate callback array in .CRT$XLB section
extern "C" {
#pragma const_seg(".CRT$XLB")
    EXTERN_C const PIMAGE_TLS_CALLBACK tls_callback_func = TLSCallback;
#pragma const_seg()
}
#endif

int main()
{
    printf("Inside main...\n");
    return 0;
}
