section .text
libcc_init:
	jmp label0
extern puts
label0:
	jmp label1
	
global main
main:
	push ebp
	mov ebp, esp
	call libcc_init
	sub esp, 4
	xor eax, eax
	mov eax, dword [ebp + 16]
	mov dword [ebp-4], eax
	sub esp, 4
	xor eax, eax
	mov eax, dword [ebp + 12]
	mov dword [ebp-8], eax
	xor eax, eax
	mov eax, dword [ebp + 12]
	push eax
	pop ebx
	push ebx
	mov eax, 0
	pop ebx
	add eax, ebx
	xchg eax, ebx
	xor eax, eax
	mov eax, dword [ebx]
	push eax
	call puts
	mov eax, 0
	mov esp, ebp
	pop ebp
	ret
	mov esp, ebp
	pop ebp
	ret
label1:
	ret
section .rodata
LIBRARY_STR: db "Made with libcc", 0
