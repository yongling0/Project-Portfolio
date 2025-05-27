.data
str1: .string "s1111410\n"
fout:   .asciz "s1111410_output.txt"      # filename for output
fin:   .asciz "1.txt"      # filename for input
buffer: .space 100   # �Ω�s���r�ɤ��e���w�İ�
integerbuffer: .space 100
output: .space 100

.text
main:
	jal printID	#print s1111410
	jal ReadFile
	jal ParseData
	jal MeanX
	mv s2,t3
	jal MeanY
	mv s3,t3
	jal CaculatedistanseX
	mv s4,a0
	jal CaculatedistanseY
	mv s5,a0
	jal CompareXY
	j End
CompareXY:
	addi sp,sp,-4
        sw ra,0(sp)
        la a2, integerbuffer
        la a3, output
        mv a1,s1
        li a4,48
        #addi a1,a1,1
	bge s4,s5,chooseY
	#jal chooseX
chooseY:
	beq a1,zero,judgeMaxMinX
        addi a1,a1,-1
        lbu t1, 1(a2)
        add t1,t1,a4
        sb t1, 0(a3)
        addi a3,a3,1
        li t4,44      
        sb t4,0(a3)     
        addi a3,a3,1
        add t3,s3,a4
        sb t3,0(a3)
        addi a3,a3,1
        li t4,44      
        sb t4,0(a3)
        addi a3,a3,1
        addi a2,a2,2
        jal x0, chooseY
judgeMaxMinX:
	li a5,0
	li a6,10
	la a2, integerbuffer
	mv a1,s1
loopchooseY:
	beq a1,zero,endchooseY
        addi a1,a1,-1
        lbu t1, 1(a2)
        blt t1,a6,changeMinX
        bge t1,a5,changeMaxX
        addi a2,a2,2
        jal x0,loopchooseY
changeMinX:
	mv a6,t1
	addi a2,a2,2
	jal loopchooseY
changeMaxX:
	mv a5,t1
	addi a2,a2,2
	jal loopchooseY
endchooseY:
	add t3,a6,a4
	sb t3,0(a3)
	addi a3,a3,1
        li t4,44      
        sb t4,0(a3)     
        addi a3,a3,1
        add t3,s3,a4
        sb t3,0(a3)
        addi a3,a3,1
        li t4,44      
        sb t4,0(a3)     
        addi a3,a3,1
        add t3,a5,a4
        sb t3,0(a3)
	addi a3,a3,1
	li t4,44      
        sb t4,0(a3)     
        addi a3,a3,1
	add t3,s3,a4
        sb t3,0(a3)
la a0,output
li a7,4
ecall 

la a0,buffer
li a7,4
ecall

	lw ra,0(sp)
	addi sp,sp,4
	ret
CaculatedistanseY:
	li t2,0
	addi sp,sp,-4
        sw ra,0(sp)
	la a2, integerbuffer
	mv a1,s1
loop2:
	beq a1,zero,end2
        addi a1,a1,-1
        lbu t1, 2(a2)
        bge t1,s3,minusMeanY
        jal MeanYminus
DistanceY:
        add t2,t2,t1
        addi a2,a2,2
        jal loop2
minusMeanY:
	sub t1,t1,s3
	j DistanceY
MeanYminus:
	sub t1,s3,t1
	j DistanceY
end2:
	mv a0,t2
	lw ra,0(sp)
	addi sp,sp,4
	ret
CaculatedistanseX:
	li t2,0
	addi sp,sp,-4
        sw ra,0(sp)
	la a2, integerbuffer
	mv a1,s1
loop1:
	beq a1,zero,end1
        addi a1,a1,-1
        lbu t1, 1(a2)
        bge t1,s2,minusMeanX
        jal MeanXminus
DistanceX:
        add t2,t2,t1
        addi a2,a2,2
        jal loop1
minusMeanX:
	sub t1,t1,s2
	j DistanceX
MeanXminus:
	sub t1,s2,t1
	j DistanceX
end1:
	mv a0,t2
	lw ra,0(sp)
	addi sp,sp,4
	ret 
MeanY:
	li t2,0
        addi sp,sp,-4
        sw ra,0(sp)
	la a2, integerbuffer
	mv a1,s1
loopY:
	beq a1,zero,endY
        addi a1,a1,-1
        lbu t1, 2(a2)
        add t2,t2,t1
        addi a2,a2,2
        jal x0, loopY
endY:
        divu t3,t2,s1
	lw ra,0(sp)
	addi sp,sp,4
	ret 
MeanX:
	li t2,0
        addi sp,sp,-4
        sw ra,0(sp)
	la a2, integerbuffer
	mv a1,s1
loopX:
	beq a1,zero,endX
        addi a1,a1,-1
        lbu t1, 1(a2)
        add t2,t2,t1
        addi a2,a2,2
        jal x0, loopX
endX:
        divu t3,t2,s1
	lw ra,0(sp)
	addi sp,sp,4
	ret 
ParseData:
    addi sp,sp,-4
    sw ra,0(sp)
    la a1, buffer    # ���VŪ���쪺�r��ƾ�
    la a2, integerbuffer    # ���V�ѪR�᪺��Ƽƾڦs�x��m
    lbu t2, 0(a1)    # Ū���@�Ӧr�`
    addi t2,t2,-48
    mv s1,t2 #�s������
process_data:
    lbu t2, 0(a1)    # Ū���@�Ӧr�`
    beqz t2, parse_end  # �p�G�O�r�굲���A�h����쵲��
    # �ˬd�O�_���Ʀr
    li t3, 48        # '0' �� ASCII ��
    li t4, 57        # '9' �� ASCII ��
    blt t2, t3, skip_processing
    bgt t2, t4, skip_processing

    # �p�G�O�Ʀr�A�N��K�[���e����Ƥ�
    sub t2, t2, t3   # �ഫ ASCII �X��Ʀr
    sb t2, 0(a2)     # �N��Ʀs�x�� integerbuffer ��
    
    #mv a0,t2
    #li a7, 1
    #ecall
    
    addi a2, a2, 1   # ���ʨ�U�@�Ӧs�x��m
    addi a1, a1, 1   # ���ʨ�U�@�Ӧr�`
    j process_data

skip_processing:
    addi a1, a1, 1  # buffer ���w�Ჾ�@��
    j process_data  # �B�z�U�@�Ӧr�`
   
parse_end:
    	lw ra,0(sp)
	addi sp,sp,4
	ret 
printID:
        la       a0, str1	# prepare to print studentID
        li       a7, 4		# print studentID
        ecall
	ret
ReadFile:
 # �}���ɮ�
    la a0, fin
    li a1, 0    # ��r�ɼҦ�: O_RDONLY (��Ū)
    li a7, 1024 
    ecall       

    # �ˬd�ɮ׬O�_���\�}��
    li t0, -1
    bne a0, t0, open_success

    # �Y�ɮ׶}�ҥ��ѡA�h�X�{��
    li a7, 10   
    li a0, -1   
    ecall       

open_success:
    # Ū���ɮפ��e
    mv a0, a0   
    la a1, buffer   
    li a2, 100  
    li a7, 63   # �t�νեθ�: sys_read (Ū���ɮ�)
    ecall       

    
    #mv a0, a1   
    #li a1, 100  
    #li a7, 4   
    #ecall      

    # �����ɮ�
    mv a0, a0   
    li a7, 57   # �t�νեθ�: sys_close (�����ɮ�)
    ecall       

    # �����{��
    #li a7, 93   # �t�νեθ�: sys_exit (�h�X�{��)
    #li a0, 0    # ��^��: 0 (���\)
    #ecall       

SaveFile:  
	# Open (for writing) a file that does not exist
	li   a7, 1024     # system call for open file
	la   a0, fout     # output file name
	li   a1, 1        # Open for writing (flags are 0: read, 1: write)
	ecall             # open a file (file descriptor returned in a0)
	mv   s6, a0       # save the file descriptor
  
	# Write to file just opened
	li   a7, 64       # system call for write to file
	mv   a0, s6       # file descriptor
	la   a1, buffer   # address of buffer from which to write
	li   a2, 19       # hardcoded buffer length
	ecall             # write to file
	###############################################################
	# Close the file
	li   a7, 57       # system call for close file
	mv   a0, s6       # file descriptor to close
	ecall             # close file
	ret
	###############################################################
End:
	li a7, 10			
	ecall
