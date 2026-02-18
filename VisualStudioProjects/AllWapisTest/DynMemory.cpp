
#include "MainHeader.h"

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <windows.h>
using namespace std;



void dynamic_memory_allocation(int n) {
    size_t sz = (size_t)n * sizeof(unsigned char);
    void* buf = VirtualAlloc(NULL, sz, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (buf == NULL) {
        fprintf(stderr, "VirtualAlloc() failed\n");
        exit(1);
    }
    char* ipbuff = (char*)buf;
    for (size_t i = 0; i < sz; ++i) {
        ipbuff[i] = 0x90; // nop
    }
    ipbuff[sz - 1] = 0xc3; // ret
    DWORD oldProtect = 0;
    if (!VirtualProtect(buf, sz, PAGE_EXECUTE_READ, &oldProtect)) {
        fprintf(stderr, "VirtualProtect to RX failed\n");
        VirtualFree(buf, 0, MEM_RELEASE);
        exit(1);
    }
    typedef void (*nops_fun)();
    nops_fun nops = (nops_fun)buf;
    fprintf(stdout, "Entering nops() sled\n");
    nops();
    fprintf(stdout, "Exiting nops() sled\n");
    VirtualFree(buf, 0, MEM_RELEASE);
}

int memory() {
    dynamic_memory_allocation(42);
    return 0;
}