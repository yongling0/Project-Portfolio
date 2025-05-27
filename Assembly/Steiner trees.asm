.data
str1: .string "s1111410\n"
fout:   .asciz "s1111410_output.txt"      # filename for output
fin:   .asciz "1.txt"      # filename for input
buffer: .space 100   # 用於存放文字檔內容的緩衝區
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
    la a1, buffer    # 指向讀取到的字串數據
    la a2, integerbuffer    # 指向解析後的整數數據存儲位置
    lbu t2, 0(a1)    # 讀取一個字節
    addi t2,t2,-48
    mv s1,t2 #存取次數
process_data:
    lbu t2, 0(a1)    # 讀取一個字節
    beqz t2, parse_end  # 如果是字串結尾，則跳轉到結束
    # 檢查是否為數字
    li t3, 48        # '0' 的 ASCII 值
    li t4, 57        # '9' 的 ASCII 值
    blt t2, t3, skip_processing
    bgt t2, t4, skip_processing

    # 如果是數字，將其添加到當前的整數中
    sub t2, t2, t3   # 轉換 ASCII 碼到數字
    sb t2, 0(a2)     # 將整數存儲到 integerbuffer 中
    
    #mv a0,t2
    #li a7, 1
    #ecall
    
    addi a2, a2, 1   # 移動到下一個存儲位置
    addi a1, a1, 1   # 移動到下一個字節
    j process_data

skip_processing:
    addi a1, a1, 1  # buffer 指針後移一位
    j process_data  # 處理下一個字節
   
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
 # 開啟檔案
    la a0, fin
    li a1, 0    # 文字檔模式: O_RDONLY (唯讀)
    li a7, 1024 
    ecall       

    # 檢查檔案是否成功開啟
    li t0, -1
    bne a0, t0, open_success

    # 若檔案開啟失敗，退出程序
    li a7, 10   
    li a0, -1   
    ecall       

open_success:
    # 讀取檔案內容
    mv a0, a0   
    la a1, buffer   
    li a2, 100  
    li a7, 63   # 系統調用號: sys_read (讀取檔案)
    ecall       

    
    #mv a0, a1   
    #li a1, 100  
    #li a7, 4   
    #ecall      

    # 關閉檔案
    mv a0, a0   
    li a7, 57   # 系統調用號: sys_close (關閉檔案)
    ecall       

    # 結束程序
    #li a7, 93   # 系統調用號: sys_exit (退出程序)
    #li a0, 0    # 返回值: 0 (成功)
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
