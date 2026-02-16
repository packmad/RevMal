.intel_syntax noprefix
.text


.globl junk_bytes
.type junk_bytes, @function
junk_bytes:
	jz L_END
	jnz L_END
    .byte 0x42, 0x42  # invalid instruction
L_END:
	mov eax, 1
	ret


.globl overlapping_instructions
.type overlapping_instructions, @function
overlapping_instructions:
	push rax          # 50
	mov ax, 0x5eb     # 66 b8 (eb 05) = jmp +7  --|
	xor eax, eax      # 31 c0    ^                |
	.byte 0x74, 0xfa  # je -4 ---/                |
    .byte 0xe8        #                           |
	pop rax           # 58  <---------------------/
    mov eax, 1        # b8 01 00 00 00
	ret               # c3


.globl call_ret_abuse
.type call_ret_abuse, @function
call_ret_abuse:
	call $+5               # e8 00 00 00 00
	add qword ptr[rsp], 6  # 48 83 04 24 06
	ret                    # c3
    mov eax, 1             # b8 01 00 00 00
    ret                    # c3
