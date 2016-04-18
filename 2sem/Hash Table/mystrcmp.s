.section .text
.global mystrcmp
mystrcmp:
	
	push %ebp
	movl %esp, %ebp

	#push %esi
	#push %edi
	movl 8(%ebp), %edi
	movl 12(%ebp), %esi
	
	xorl %ecx, %ecx
	
compare:	
	cmpsb 
	ja above
	jb below
	
	movb (%edi), %al
	addb (%esi), %al
	cmpb %al, %cl
	je equ
	jmp compare

equ:
	#pop %edi
	#pop %esi
	pop %ebp
	movl $0, %eax
	ret

above:
	#pop %edi
	#pop %esi
	pop %ebp
	movl $1, %eax
	ret

below:
	#pop %edi
	#pop %esi
	pop %ebp
	movl $-1, %eax
	ret


