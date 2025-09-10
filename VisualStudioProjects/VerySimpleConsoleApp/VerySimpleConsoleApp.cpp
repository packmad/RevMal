#include <iostream>
#include <stdio.h>
#include <string.h>
#include <windows.h>
using namespace std;

/*************************************************************************************************/

int if_then_else(int n) {
    if (n == 0) return 0;
    if (n % 2 == 0) { return 1; }
    else { return -1; }
}


int while_loop(int n) {
    int r = 11;
    while (n > 0) {
        --n;
        r += n;
    }
    return r;
}

int do_while_loop(int n) {
    int r = 13;
    do {
        --n;
        r += n;
    } while (n > 0);
    return r;
}

int for_loop(int n) {
    int r = 17;
    for (int i = 0; i < n; ++i) {
        r += i;
    }
    return r;
}

int read_integer() {
    int i;
    cout << "Enter an integer: ";
    if (cin >> i) return i;
    cout << "Invalid input :(";
    exit(1);
}

void control_flow(int n) {
    int r;
    const char* fmt = "> %d\n";
    r = if_then_else(n);
    printf(fmt, r);
    r = while_loop(n);
    printf(fmt, r);
    r = do_while_loop(n);
    printf(fmt, r);
    r = for_loop(n);
    printf(fmt, r);
}

/*************************************************************************************************/

void show_primitive_data_types() {
    printf("=== Native C Data Types ===\n\n");

    // Integer types
    char c = 'A';
    signed char sc = -100;
    unsigned char uc = 200;

    short s = -12345;
    unsigned short us = 54321;

    int i = -123456;
    unsigned int ui = 123456U;

    long l = -1234567890L;
    unsigned long ul = 1234567890UL;

    long long ll = -123456789012345LL;
    unsigned long long ull = 123456789012345ULL;

    printf("char: %c\n", c);
    printf("signed char: %d\n", sc);
    printf("unsigned char: %u\n", uc);
    printf("short: %d\n", s);
    printf("unsigned short: %u\n", us);
    printf("int: %d\n", i);
    printf("unsigned int: %u\n", ui);
    printf("long: %ld\n", l);
    printf("unsigned long: %lu\n", ul);
    printf("long long: %lld\n", ll);
    printf("unsigned long long: %llu\n", ull);

    // Floating-point types
    float f = 3.14f;
    double d = 3.141592653589793;
    long double ld = 3.14159265358979323846L;

    printf("\nfloat: %f\n", f);
    printf("double: %lf\n", d);
    printf("long double: %Lf\n", ld);

    // Boolean type (from <stdbool.h>)
    bool flag = true;
    printf("\nbool: %d (true=1, false=0)\n", flag);

    // Pointers
    int* p = &i;
    printf("\nPointer to int: %p, value=%d\n", (void*)p, *p);

    // Sizeof each type
    printf("\n=== sizeof() each type ===\n");
    printf("sizeof(char) = %zu\n", sizeof(char));
    printf("sizeof(short) = %zu\n", sizeof(short));
    printf("sizeof(int) = %zu\n", sizeof(int));
    printf("sizeof(long) = %zu\n", sizeof(long));
    printf("sizeof(long long) = %zu\n", sizeof(long long));
    printf("sizeof(float) = %zu\n", sizeof(float));
    printf("sizeof(double) = %zu\n", sizeof(double));
    printf("sizeof(long double) = %zu\n", sizeof(long double));
    printf("sizeof(bool) = %zu\n", sizeof(bool));
    printf("sizeof(void*) = %zu\n", sizeof(void*));
}


int arrays_and_pointers(int n) {
    int arr[5] = { 10, 20, 30, 40, 50 };
    int* p = arr;
    int r = 0;
    for (int i = 0, j = 0; i < n; i++) {
        j = i % 5;
        printf("arr[%d] = %d = %d\n", j, arr[j], *(p + j));
        r += arr[j];
    }
    return r;
}

/*************************************************************************************************/

void dynamic_memory_allocation(int n) {
    size_t sz = (size_t) n * sizeof(unsigned char);
    void* buf = VirtualAlloc(NULL, sz, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    if (buf == NULL) {
        fprintf(stderr, "VirtualAlloc() failed\n");
        exit(1);
    }
    char* ipbuff = (char*)buf;
    for (size_t i = 0; i < sz; ++i) {
        ipbuff[i] = 0x90; // nop
    }
    ipbuff[sz-1] = 0xc3; // ret
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

/*************************************************************************************************/

struct Person {
    char name[20];
    char surname[20];
    int birthyear;
};

// Function to increase the birthyear of each person by 5
void increaseBirthYear(struct Person* p, int x) {
    p->birthyear += x;
}

// Function to print all persons
void printPeople(struct Person people[], int size) {
    for (int i = 0; i < size; i++) {
        printf("Name: %s, Surname: %s, Birthyear: %d\n",
            people[i].name, people[i].surname, people[i].birthyear);
    }
}

void struct_management() {
    const int sz = 3;
    struct Person people[sz] = {
        {"Alice", "Smith", 1990},
        {"Bob", "Johnson", 1985},
        {"Charlie", "Brown", 2000}
    };
    for (int i = 0; i < sz; i++) {
        increaseBirthYear(&people[i], i);
    }
    printPeople(people, 3);
}


int main()
{
    int n, r;
    n = read_integer();
    control_flow(n);
    show_primitive_data_types();
    r = arrays_and_pointers(n);
    dynamic_memory_allocation(n);
    struct_management();
    return r;
}
