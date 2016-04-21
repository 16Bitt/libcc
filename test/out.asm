section .text
libcc_init:
	jmp label0
extern puts
label0:
	jmp label1
extern putchar
label1:
	jmp label2
	
global twoargs
twoargs:
	push ebp
	mov ebp, esp
	xor eax, eax
	mov eax, dword [ebp + 16]
	push eax
	call puts
	xor eax, eax
	mov eax, dword [ebp + 12]
	push eax
	call puts
	xor eax, eax
	mov eax, dword [ebp + 8]
	push eax
	call puts
	mov esp, ebp
	pop ebp
	ret
label2:
	jmp label3
	
global putint
putint:
	push ebp
	mov ebp, esp
	sub esp, 4
	sub esp, 4
	xor eax, eax
	mov eax, dword [ebp + 8]
	push eax
	pop ebx
	mov eax, 0
	cmp ebx, eax
	jl label5
	xor eax, eax
	jmp label6
label5:
	mov eax, 1
label6:
	or eax, eax
	jz label4
	xor eax, eax
	mov eax, dword [ebp + 8]
	push eax
	pop ebx
	push dword 0
	pop ebx
	mov eax, 1
	sub eax, ebx
	neg eax
	imul ebx
	mov dword [ebp + 8], eax
	mov eax, 45
	push eax
	call putchar
label4:
	xor eax, eax
	mov eax, dword [ebp + 8]
	push eax
	pop ebx
	mov eax, 0
	cmp ebx, eax
	je label8
	xor eax, eax
	jmp label9
label8:
	mov eax, 1
label9:
	or eax, eax
	jz label7
	xor eax, eax
	mov esp, ebp
	pop ebp
	ret
label7:
	xor eax, eax
	mov eax, dword [ebp + 8]
	push eax
	pop ebx
	mov eax, 10
	xor edx, edx
	xchg eax, ebx
	idiv ebx
	mov eax, edx
	mov dword [ebp-4], eax
	xor eax, eax
	mov eax, dword [ebp-4]
	push eax
	pop ebx
	mov eax, 48
	add eax, ebx
	mov dword [ebp-8], eax
	xor eax, eax
	mov eax, dword [ebp + 8]
	push eax
	pop ebx
	mov eax, 10
	xor edx, edx
	xchg eax, ebx
	idiv ebx
	push eax
	call putint
	xor eax, eax
	mov al, byte [ebp-8]
	push eax
	call putchar
	mov esp, ebp
	pop ebp
	ret
label3:
	jmp label10
	
global main
main:
	push ebp
	mov ebp, esp
	call libcc_init
	sub esp, 4
	mov eax, 1
	mov dword [ebp-4], eax
label11:
	xor eax, eax
	mov eax, dword [ebp-4]
	push eax
	pop ebx
	xor eax, eax
	mov eax, dword [ebp + 8]
	cmp ebx, eax
	jl label13
	xor eax, eax
	jmp label14
label13:
	mov eax, 1
label14:
	or eax, eax
	jz label12
	xor eax, eax
	mov eax, dword [ebp-4]
	push eax
	call putint
	mov eax, 32
	push eax
	call putchar
	xor eax, eax
	mov eax, dword [ebp + 12]
	push eax
	pop ebx
	push ebx
	xor eax, eax
	mov eax, dword [ebp-4]
	pop ebx
	shl eax, 2
	add eax, ebx
	xchg eax, ebx
	xor eax, eax
	mov eax, dword [ebx]
	push eax
	call puts
	xor eax, eax
	mov eax, dword [ebp-4]
	push eax
	pop ebx
	mov eax, 1
	add eax, ebx
	mov dword [ebp-4], eax
	jmp label11
label12:
	mov eax, 0
	mov esp, ebp
	pop ebp
	ret
	mov esp, ebp
	pop ebp
	ret
label10:
	ret
section .rodata
LIBRARY_STR: db "Made with libcc", 0
