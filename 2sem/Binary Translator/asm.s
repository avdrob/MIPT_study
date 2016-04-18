pushl %eax
pushl %ebx
pushl %ecx
pushl %edx

movl $3, %eax
movl $1, %ebx
subl $11, %esp
movl %esp, %ecx
movl $10, %edx
int $0x80

movl $0xa, %ebx

step:
	cmpb (%esp), %bl
	je enough
	subb $'0', (%esp)
	mul %bl
	addb (%esp), %eax
	inc %esp
	loop step

enough:
movl %ecx, %esp
popl %edx
popl %ecx
popl %ebx
popl %eax





