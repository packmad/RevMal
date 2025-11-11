.intel_syntax noprefix
.text

.globl absval1
.type absval1, @function
absval1:
    mov eax, edi        ; eax = x
    test eax, eax       # check sign
    jge done1           # jump if >= 0
    neg eax             # eax = -eax
done1:
    ret

.globl absval2
.type absval2, @function
absval2:
    mov eax, edi        # eax = x
    cdq                 # sign extend eax -> edx (0 or -1)
    xor eax, edx        # eax ^= sign
    sub eax, edx        # eax -= sign => abs(x)
    ret

