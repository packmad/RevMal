// bcrypt_aes_cbc_example.c
// Build (Developer Command Prompt for VS):
//   cl /TC bcrypt_aes_cbc_example.c /W4 /EHsc bcrypt.lib
//
// Notes:
// - Uses Windows CNG (BCrypt) "modern" crypto API.
// - AES-256-CBC with PKCS#7 padding.
// - For a real app: generate a random key+IV (BCryptGenRandom) and store/transport them securely.

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <bcrypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib, "bcrypt.lib")

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

static void die_nt(const char* msg, NTSTATUS st) {
    fprintf(stderr, "%s (NTSTATUS=0x%08X)\n", msg, (unsigned)st);
    exit(1);
}

static void print_hex(const char* label, const unsigned char* p, DWORD cb) {
    printf("%s (%lu bytes): ", label, (unsigned long)cb);
    for (DWORD i = 0; i < cb; i++) printf("%02X", p[i]);
    printf("\n");
}

int crypto() {
    // Example plaintext
    const char* plaintext = "Hello from BCrypt AES-CBC!";
    DWORD plaintext_len = (DWORD)strlen(plaintext);

    // Hardcoded demo key (32 bytes = AES-256) and IV (16 bytes = AES block size).
    // DO NOT hardcode keys like this in real software.
    unsigned char key_bytes[32] = {
        0x60,0x3D,0xEB,0x10,0x15,0xCA,0x71,0xBE,0x2B,0x73,0xAE,0xF0,0x85,0x7D,0x77,0x81,
        0x1F,0x35,0x2C,0x07,0x3B,0x61,0x08,0xD7,0x2D,0x98,0x10,0xA3,0x09,0x14,0xDF,0xF4
    };
    unsigned char iv0[16] = {
        0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F
    };

    BCRYPT_ALG_HANDLE hAlg = NULL;
    BCRYPT_KEY_HANDLE hKey = NULL;
    PUCHAR key_object = NULL;
    DWORD key_object_len = 0, cbData = 0;

    NTSTATUS st;

    // 1) Open AES algorithm provider
    st = BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_AES_ALGORITHM, NULL, 0);
    if (!NT_SUCCESS(st)) die_nt("BCryptOpenAlgorithmProvider", st);

    // 2) Set chaining mode to CBC
    st = BCryptSetProperty(
        hAlg,
        BCRYPT_CHAINING_MODE,
        (PUCHAR)BCRYPT_CHAIN_MODE_CBC,
        (DWORD)sizeof(BCRYPT_CHAIN_MODE_CBC),
        0
    );
    if (!NT_SUCCESS(st)) die_nt("BCryptSetProperty(CHAINING_MODE)", st);

    // 3) Determine size of key object and allocate it
    st = BCryptGetProperty(hAlg, BCRYPT_OBJECT_LENGTH, (PUCHAR)&key_object_len, sizeof(key_object_len), &cbData, 0);
    if (!NT_SUCCESS(st)) die_nt("BCryptGetProperty(OBJECT_LENGTH)", st);

    key_object = (PUCHAR)HeapAlloc(GetProcessHeap(), 0, key_object_len);
    if (!key_object) {
        fprintf(stderr, "HeapAlloc failed\n");
        exit(1);
    }

    // 4) Create/generate symmetric key from raw key material
    st = BCryptGenerateSymmetricKey(hAlg, &hKey, key_object, key_object_len, key_bytes, (DWORD)sizeof(key_bytes), 0);
    if (!NT_SUCCESS(st)) die_nt("BCryptGenerateSymmetricKey", st);

    // Encryption uses/updates the IV buffer internally, so use a working copy each time.
    unsigned char iv_enc[16];
    memcpy(iv_enc, iv0, sizeof(iv_enc));

    // 5) Query required ciphertext size (with PKCS#7 padding)
    DWORD ciphertext_len = 0;
    st = BCryptEncrypt(
        hKey,
        (PUCHAR)plaintext, plaintext_len,
        NULL,
        iv_enc, (DWORD)sizeof(iv_enc),
        NULL, 0,
        &ciphertext_len,
        BCRYPT_BLOCK_PADDING
    );
    if (!NT_SUCCESS(st)) die_nt("BCryptEncrypt(size query)", st);

    unsigned char* ciphertext = (unsigned char*)malloc(ciphertext_len);
    if (!ciphertext) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    // Reset IV copy before the real encrypt call (because the size query also mutates it)
    memcpy(iv_enc, iv0, sizeof(iv_enc));

    // 6) Encrypt
    DWORD ciphertext_written = 0;
    st = BCryptEncrypt(
        hKey,
        (PUCHAR)plaintext, plaintext_len,
        NULL,
        iv_enc, (DWORD)sizeof(iv_enc),
        ciphertext, ciphertext_len,
        &ciphertext_written,
        BCRYPT_BLOCK_PADDING
    );
    if (!NT_SUCCESS(st)) die_nt("BCryptEncrypt", st);

    print_hex("Ciphertext", ciphertext, ciphertext_written);

    // 7) Decrypt (again, use a fresh IV copy)
    unsigned char iv_dec[16];
    memcpy(iv_dec, iv0, sizeof(iv_dec));

    DWORD decrypted_len = 0;
    st = BCryptDecrypt(
        hKey,
        ciphertext, ciphertext_written,
        NULL,
        iv_dec, (DWORD)sizeof(iv_dec),
        NULL, 0,
        &decrypted_len,
        BCRYPT_BLOCK_PADDING
    );
    if (!NT_SUCCESS(st)) die_nt("BCryptDecrypt(size query)", st);

    unsigned char* decrypted = (unsigned char*)malloc(decrypted_len + 1); // +1 for null terminator
    if (!decrypted) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    // Reset IV again (size query mutated it)
    memcpy(iv_dec, iv0, sizeof(iv_dec));

    DWORD decrypted_written = 0;
    st = BCryptDecrypt(
        hKey,
        ciphertext, ciphertext_written,
        NULL,
        iv_dec, (DWORD)sizeof(iv_dec),
        decrypted, decrypted_len,
        &decrypted_written,
        BCRYPT_BLOCK_PADDING
    );
    if (!NT_SUCCESS(st)) die_nt("BCryptDecrypt", st);

    decrypted[decrypted_written] = '\0';
    printf("Decrypted: %s\n", decrypted);

    // Cleanup
    free(ciphertext);
    free(decrypted);
    if (hKey) BCryptDestroyKey(hKey);
    if (hAlg) BCryptCloseAlgorithmProvider(hAlg, 0);
    if (key_object) HeapFree(GetProcessHeap(), 0, key_object);

    return 0;
}