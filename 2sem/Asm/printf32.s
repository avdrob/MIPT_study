#==================================================
#	Prints a string with parameters.
#
#	Format: 'abc %par def %par ...', '\0'
#		par == 'd' - print decimal number;
#		par == 'x' - print hex number;
#		par == 'b' - print binary number;
#		par == 's' - print string;
#		par == 'c' - print character;
#		par == '%' - print '%'
#
#	Entry: 
#		pushl arg n
#		pushl arg n-1
#		...
#		pushl arg 1
#		pushl string offset
#
#		Here's n = number of parameters.
#		args are numbers to print themselves
#			or ASCII codes.
#		In case of %s it must be:
#			pushl string offset
#	
#	Exit:
#		(%ecx) == the last character ('\0')
#
#	Destr:
#		%eax, %ebx, %ecx, %edx, %esi
#==================================================

.section .data
hex_num:
        .ascii "0123456789ABCDEF"
        
        DCM: .long 10 
        BIN: .long 1
        HXD: .long 15

error_mes:
        .ascii "\nUnknown specificator: "
        len = . - error_mes
        
.section .text
.globl print
print:
        pushl %ebp
        movl %esp, %ebp
        subl $4, %esp
        
        movl 8(%ebp), %ecx
        movl $12, -4(%ebp)
        
outp:      
        xorb %al, %al
        cmpb %al, (%ecx)
        je exit
        
        movb $'%', %al
        cmpb %al, (%ecx)
        je id
        
        movl $4, %eax
        movl $1, %ebx
        movl $1, %edx
        int $0x80
        
        inc %ecx
        jmp outp
        
exit:         
        addl $4, %esp
        popl %ebp
        ret 
#==================================================

id:							
        inc %ecx
        
        movb $'s', %al
        cmpb %al, (%ecx)
        je str
        
        movb $'x', %al
        cmpb %al, (%ecx)
        je hex
        
        movb $'b', %al
        cmpb %al, (%ecx)
        je bnr
        
        movb $'d', %al
        cmpb %al, (%ecx)
        je dcml
        
        movb $'c', %al
        cmpb %al, (%ecx)
        je chr
        
        movb $'%', %al
        cmpb %al, (%ecx)
        je perc
        
        pushl %ecx							# Emergency exit
        movl $4, %eax						# (unknown specificator)
        movl $1, %ebx
        movl $error_mes, %ecx
        movl $len, %edx
        int $0x80
        
        popl %ecx
        movl $4, %eax
        movl $1, %edx
        int $0x80
       
        jmp exit
       #==================================================
       str:
	  pushl %ecx
	  
	  movl  %ebp, %ecx
	  addl -4(%ebp), %ecx
	  movl (%ecx), %ecx
	  
	  putchr:
	          xorb %al, %al
	          cmpb %al, (%ecx)
	          je back
	          
	          movl $4, %eax
	          movl $1, %ebx
	          movl $1, %edx
	          int $0x80
        
	          inc %ecx
	          jmp putchr
	  
	  back:
	          popl %ecx
	          inc %ecx
	          addl $4, -4(%ebp)
	          jmp outp
	          
        #==================================================  
        hex:
	  pushl %ecx
	  
	  movl %ebp, %eax
	  addl -4(%ebp), %eax
	  movl (%eax), %eax
	  
	  movl $32, %ecx
	  
	  get_numh:
	          movl  %eax, %esi
	          subb $4, %cl
	          shr %cl, %esi
	          addb $4, %cl
	          andl HXD, %esi
	          
	          pushl %ecx
	          pushl %eax
	  
	          movl $hex_num, %ecx
	          addl %esi, %ecx
	          movl $4, %eax
	          movl $1, %ebx
	          movl $1, %edx
	          int $0x80
	  
	          popl %eax
	          popl %ecx
	          testl %ecx, %ecx
	          subl $4, %ecx
	          jne get_numh

	  popl %ecx
	  inc %ecx
	  addl $4, -4(%ebp)
	  jmp outp
	  
        #==================================================	  
        bnr:
	  pushl %ecx
	  
	  movl %ebp, %eax
	  addl -4(%ebp), %eax
	  movl (%eax), %eax
	  
	  movl $32, %ecx
	  
	  get_numb:
	          movl  %eax, %esi
	          decb %cl
	          shr %cl, %esi
	          incb %cl
	          andl BIN, %esi
	  
	          pushl %ecx
	          pushl %eax
	  
	          movl $hex_num, %ecx
	          addl %esi, %ecx
	          movl $4, %eax
	          movl $1, %ebx
	          movl $1, %edx
	          int $0x80
	  
	          popl %eax
	          popl %ecx
	          testl %ecx, %ecx
	          dec %ecx
	          jne get_numb
	          
	  popl %ecx
	  inc %ecx
	  addl $4, -4(%ebp)
	  jmp outp
	  
        #==================================================
        dcml:
	  pushl %ecx
	  
	  movl %ebp, %eax
	  addl -4(%ebp), %eax
	  movl (%eax), %eax
	  
	  xorl %esi, %esi
	  movl $10, %ebx
	  get_numd:
	          xorl %edx, %edx
	          divl %ebx
	          subl $1, %esp
	          movb %edx, (%esp)
	          inc %esi
	          
	          testl %eax, %eax
	          jne get_numd
	          
	  movl %esp, %ebx
	  
	  pop_out:
	          movl $hex_num, %ecx
	          addb (%esp), %ecx
	          movl $4, %eax
	          movl $1, %ebx
	          movl $1, %edx
	          int $0x80
	          
	          addl $1, %esp
	          dec %esi
	          testl %esi, %esi
	          jne pop_out
	          
	  popl %ecx
	  inc %ecx
	  addl $4, -4(%ebp)
	  jmp outp
	  
        #==================================================
        chr:
	  pushl %ecx
	  
	  movl $4, %eax
	  movl $1, %ebx
	  movl %ebp, %ecx
	  addl -4(%ebp), %ecx
	  movl $1, %edx
	  int $0x80
	  
	  popl %ecx
	  inc %ecx
	  addl $4, -4(%ebp)
	  jmp outp
	  
        #==================================================
        perc:
	  pushl %ecx
	  pushl $'%'
	  
	  movl $4, %eax
	  movl $1, %ebx
	  movl %esp, %ecx
	  movl $1, %edx
	  int $0x80
	  
	  addl $4, %esp
	  popl %ecx
	  inc %ecx
	  jmp outp
        #==================================================
