// veh_counter_graceful.cpp
#define NOMINMAX
#include <windows.h>
#include <atomic>
#include <iostream>

static std::atomic<long> g_vehHits{ 0 };

static LONG CALLBACK VehCounter(PEXCEPTION_POINTERS info)
{
    if (info && info->ExceptionRecord &&
        info->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
    {
        const long n = ++g_vehHits;
        std::cout << "[VEH] Access violation observed. Count=" << n
            << "  Addr=" << info->ExceptionRecord->ExceptionAddress << "\n";
    }

    // Observe only; let other handlers (like SEH below) handle it.
    return EXCEPTION_CONTINUE_SEARCH;
}

int veh()
{
    void* h = AddVectoredExceptionHandler(/*First=*/1, VehCounter);
    if (!h) {
        std::cerr << "AddVectoredExceptionHandler failed: " << GetLastError() << "\n";
        return 1;
    }

    std::cout << "Triggering an access violation (will be handled by SEH)...\n";

    __try
    {
        volatile int* p = reinterpret_cast<int*>(0x1);
        *p = 42; // AV
        std::cout << "Unreachable\n";
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        std::cout << "[SEH] Caught the exception, continuing gracefully.\n";
    }

    std::cout << "Total VEH hits: " << g_vehHits.load() << "\n";

    RemoveVectoredExceptionHandler(h);
    std::cout << "Done.\n";
    return 0;
}

int exceptions() {
    return veh();
}