#define _WIN32_WINNT 0x0600  // for QueryFullProcessImageNameW (Vista+)

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


uint32_t djb2_hash_wide(const wchar_t* str) {
    uint32_t hash = 5381;
    wchar_t c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + (uint32_t)c;
    }
    return hash;
}


static BOOL find_process_hash(void) {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) {
        wprintf(L"CreateToolhelp32Snapshot failed (err=%lu)\n", GetLastError());
        return FALSE;
    }

    PROCESSENTRY32W pe = { 0 };
    pe.dwSize = sizeof(pe);
    BOOL found = FALSE;

    if (Process32FirstW(snap, &pe)) {
        do {
            DWORD pid = pe.th32ProcessID;
            uint32_t proc_djb2hash = djb2_hash_wide(pe.szExeFile);
            wprintf(L"%6lu  %s\n", pid, pe.szExeFile);
            if (153264797 == proc_djb2hash) {
                wprintf(L"Found! PID=%lu\n", pid);
                found = TRUE;
                break;
            }
        } while (Process32NextW(snap, &pe));
    }
    else {
        wprintf(L"Process32First failed (err=%lu)\n", GetLastError());
    }
    CloseHandle(snap);
    return found;
}



static BOOL find_process_opaque(void) {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) {
        wprintf(L"CreateToolhelp32Snapshot failed (err=%lu)\n", GetLastError());
        return FALSE;
    }

    uint8_t data[] = { 0x12, 0x30, 0x2d, 0x21, 0x2f, 0x2d, 0x2c, 0x74, 0x76, 0x6c, 0x27, 0x3a, 0x27 };
    size_t len = sizeof(data) / sizeof(data[0]);

    wchar_t* decoded = (wchar_t*) malloc((len + 1) * sizeof(wchar_t));
    if (!decoded) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (size_t i = 0; i < len; i++) { decoded[i] = (wchar_t)(data[i] ^ 0x42); }
    decoded[len] = L'\0';

    PROCESSENTRY32W pe = { 0 };
    pe.dwSize = sizeof(pe);
    BOOL found = FALSE;

    if (Process32FirstW(snap, &pe)) {
        do {
            DWORD pid = pe.th32ProcessID;
            wprintf(L"%6lu  %s\n", pid, pe.szExeFile);
            if (_wcsicmp(pe.szExeFile, decoded) == 0) {
                wprintf(L"Found! PID=%lu\n", pid);
                found = TRUE;
                break;
            }
        } while (Process32NextW(snap, &pe));
    }
    else {
        wprintf(L"Process32First failed (err=%lu)\n", GetLastError());
    }
    CloseHandle(snap);
    return found;
}


static BOOL find_process_cleartext(void) {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) {
        wprintf(L"CreateToolhelp32Snapshot failed (err=%lu)\n", GetLastError());
        return FALSE;
    }

    PROCESSENTRY32W pe = { 0 };
    pe.dwSize = sizeof(pe);
    BOOL found = FALSE;

    if (Process32FirstW(snap, &pe)) {
        do {
            DWORD pid = pe.th32ProcessID;
            wprintf(L"%6lu  %s\n", pid, pe.szExeFile);
            if (_wcsicmp(pe.szExeFile, L"Procmon64.exe") == 0) {
                wprintf(L"Found! PID=%lu\n", pid);
                found = TRUE;
                break;
            }
        } while (Process32NextW(snap, &pe));
    }
    else {
        wprintf(L"Process32First failed (err=%lu)\n", GetLastError());
    }
    CloseHandle(snap);
    return found;
}

int main()
{
    std::cout << "> Cleartext search\n"; find_process_cleartext(); std::cin.get();
    std::cout << "> Opaque search\n"; find_process_opaque(); std::cin.get();
    std::cout << "> Hash search\n"; find_process_hash(); std::cin.get();
}
