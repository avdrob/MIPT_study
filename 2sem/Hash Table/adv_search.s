.section .text
.global adv_search
adv_search:
        
        push %ebp
        movl %esp, %ebp
        
        movl 8(%ebp), %esi
        movl 12(%ebp), %edi
        movl 16 (%ebp), %ecx
                
search:
        pushl %ecx
        pushl %esi
        pushl %edi
        call strcmp
        popl %edi
        popl %esi
        popl %ecx
        
        testb %al, %al
        je node_ret
        
        addl $0x2C, %esi
        loop search
        
        popl %ebp
        xorl %eax, %eax
        ret
        
node_ret:
        subl $0x8, %esi
        movl %esi, %eax
        popl %ebp
        ret