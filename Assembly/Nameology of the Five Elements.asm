.data
str1: .string "s1111410\n"
str2: .string "Please enter the strokes of the last name = "
str3: .string "Please enter the strokes of the first character of the first name = "
str4: .string "Please enter the strokes of the second character of the first name = "
Sky1: .string "\nSky = "
People1: .string "\nPeople = "
Land1: .string "\nLand = "
Outside1: .string "\nOutside = "
Total1: .string "\nTotal = "
Wood:.string " Wood\n"
Fire:.string " Fire\n"
Earth:.string " Earth\n"
Metal:.string " Metal\n"
Water:.string " Water\n"
Sky:.string "Sky"
People:.string "People"
Land:.string "Land"
Outside:.string "Outside"
Total:.string "Total"
generate:.string " generate "
restraint:.string " restraint "
equal:.string " equal "
newline:.string "\n"

.text
main:
	jal printID	#print s1111410
	jal printMsg1	#print str2
	jal inputLast	# return the result a0	
	mv s0,a0
	
	jal printMsg2	#print str3
	jal inputFirst	# return the result a0
	mv s1,a0	
	
	jal printMsg3	#print str4
	jal inputSecond	# return the result a0
	mv s2,a0	
	
	addi s3,s0,1	
	jal divideby10
	mv s4,a0
	jal printsky
	mv a0,s4
	jal computeWuXin # computeWuXin(M=a0), return a0
	jal printWuXinResult
	
	add s3,s0,s1
	jal divideby10
	mv s5,a0
	jal printpeople
	mv a0,s5
	jal computeWuXin # computeWuXin(M=a0), return a0
	jal printWuXinResult
	
	add s3,s1,s2
	jal divideby10
	mv s6,a0
	jal printland
	mv a0,s6
	jal computeWuXin # computeWuXin(M=a0), return a0
	jal printWuXinResult
	
	addi s3,s2,1
	jal divideby10
	mv s7,a0
	jal printoutside
	mv a0,s7
	jal computeWuXin # computeWuXin(M=a0), return a0
	jal printWuXinResult
	
	add s3,s0,s1
	add s3,s3,s2
	jal divideby10
	mv s8,a0
	jal printtotal
	mv a0,s8
	jal computeWuXin # computeWuXin(M=a0), return a0
	jal printWuXinResult
	
	mv a0,s4
	mv a1,s5
	jal relationWuXin
	la t0 Sky
	la t2 People
	jal printrelationresult
	mv a0,s4
	mv a1,s6
	jal relationWuXin
	la t0 Sky
	la t2 Land
	jal printrelationresult
	mv a0,s4
	mv a1,s7
	jal relationWuXin
	la t0 Sky
	la t2 Outside
	jal printrelationresult
	mv a0,s4
	mv,a1,s8
	jal relationWuXin
	la t0 Sky
	la t2 Total
	jal printrelationresult
	mv a0,s5
	mv a1,s6
	jal relationWuXin
	la t0 People
	la t2 Land
	jal printrelationresult
	mv a0,s5
	mv a1,s7
	jal relationWuXin
	la t0 People
	la t2 Outside
	jal printrelationresult
	mv a0,s5
	mv a1,s8
	jal relationWuXin
	la t0 People
	la t2 Total
	jal printrelationresult
	mv a0,s6
	mv a1,s7
	jal relationWuXin
	la t0 Land
	la t2 Outside
	jal printrelationresult
	mv a0,s6
	mv a1,s8
	jal relationWuXin
	la t0 Land
	la t2 Total
	jal printrelationresult
	mv a0,s7
	mv a1,s8
	jal relationWuXin
	la t0 Outside
	la t2 Total
	jal printrelationresult
	j end
printrelationresult:
	addi sp,sp,-4
	sw ra,0(sp)
	li a1,1
	li a2,2
	li a3,3
	li a4,4
	li a5,5
	la t1 equal
	beq a0,a1,printfirstsecond
	la t1 generate
	beq a0,a2,printfirstsecond
	la t1 restraint
	beq a0,a3,printfirstsecond
	la t1 generate
	beq a0,a4,printsecondfirst
	la t1 restraint
	beq a0,a5,printsecondfirst
printfirstsecond:
	mv a0,t0
	li a7 4
	ecall
	mv a0,t1
	li a7 4
	ecall
	mv a0,t2
	li a7 4
	ecall
	la a0,newline
	li a7 4
	ecall
	j endprintrelationresult
printsecondfirst:
	mv a0,t2
	li a7 4
	ecall
	mv a0,t1
	li a7 4
	ecall
	mv a0,t0
	li a7 4
	ecall
	la a0,newline
	li a7 4
	ecall
	j endprintrelationresult
relationWuXin:
	addi sp,sp,-4
	sw ra,0(sp)
	mv t1,a0
	mv t2,a1
	jal ifequal
	li a3,2
	rem a1,t1,a3
	beq a1,zero,Even1
	jal Odd1
	rem a2,t2,a3
	beq a2,zero,Even2
	jal Odd2
ifequal:
	beq t1,t2,Modefirstequalsecond
	addi a1,t1,1
	beq t1,t2,Modefirstequalsecond
	addi a1,t2,1
	beq t1,t2,Modefirstequalsecond
	ret
Odd2:
	li t0,10
	#generate
	addi a1,t2,2 #add two
	rem a1,a1,t0
	beq a1,t1,Modesecondgeneratefirst
	addi a2,t2,3 #add three
	rem a2,a2,t0
	beq a2,t1,Modesecondgeneratefirst
	#restraint
	addi a1,t2,4 #add four
	rem a1,a1,t0
	beq a1,t1,Modesecondrestraintfirst
	addi a2,t2,5 #add five
	rem a2,a2,t0
	beq a2,t1,Modesecondrestraintfirst
Odd1:
	li t0,10
	#generate
	addi a1,t1,2 #add tw0
	rem a1,a1,t0
	beq a1,t2,Modefirstgeneratesecond
	addi a2,t1,3 #add three
	rem a2,a2,t0
	beq a2,t2,Modefirstgeneratesecond
	#restraint
	addi a1,t1,4 #add four
	rem a1,a1,t0
	beq a1,t2,Modefirstrestraintsecond
	addi a2,t1,5 #add five
	rem a2,a2,t0
	beq a2,t2,Modefirstrestraintsecond
	rem a2,t2,a3
	beq a2,zero,Even2
	jal Odd2
Even1:
	li t0,10
	#generate
	addi a1,t1,1 #add one
	rem a1,a1,t0
	beq a1,t2,Modefirstgeneratesecond
	addi a2,t1,2 #add two
	rem a2,a2,t0
	beq a2,t2,Modefirstgeneratesecond
	#restraint
	addi a1,t1,3 #add three
	rem a1,a1,t0
	beq a1,t2,Modefirstrestraintsecond

	addi a2,t1,4 #add four
	rem a2,a2,t0
	beq a2,t2,Modefirstrestraintsecond
	jal Odd1
Even2:
	li t0,10
	#generate
	addi a1,t2,1 #add one
	rem a1,a1,t0
	beq a1,t1,Modesecondgeneratefirst
	addi a2,t2,2 #add two
	rem a2,a2,t0
	beq a2,t1,Modesecondgeneratefirst
	#restraint
	addi a1,t2,3 #add three
	rem a1,a1,t0
	beq a1,t1,Modesecondrestraintfirst
	addi a2,t2,4 #add four
	rem a2,a2,t0
	beq a2,t1,Modesecondrestraintfirst
	jal Odd2
endprintrelationresult:
	lw ra,0(sp)
	addi sp,sp,4
	ret  
endrelation: 
	lw ra,0(sp)
	addi sp,sp,4
	ret  
Modefirstequalsecond:
	li a0,1
	j endrelation
Modefirstgeneratesecond:
	li a0,2
	j endrelation
Modefirstrestraintsecond:
	li a0,3
	j endrelation
Modesecondgeneratefirst:
	li a0,4
	j endrelation
Modesecondrestraintfirst:
	li a0,5
	j endrelation
divideby10:
	mv t0,s3
	li t1, 10
	rem t2,t0,t1
	mv a0,t2
	ret
printsky:
	la a0,Sky1
	li a7, 4
	ecall
	mv a0,s3
	li a7,1
	ecall
	ret
printpeople:
	la a0,People1
	li a7, 4
	ecall
	mv a0,s3
	li a7,1
	ecall
	ret
printland:
	la a0,Land1
	li a7, 4
	ecall
	mv a0,s3
	li a7,1
	ecall
	ret
printoutside:
	la a0,Outside1
	li a7, 4
	ecall
	mv a0,s3
	li a7,1
	ecall
	ret
printtotal:
	la a0,Total1
	li a7, 4
	ecall
	mv a0,s3
	li a7,1
	ecall
	ret
printID:
        la  a0, str1	# prepare to print studentID
        li  a7, 4		# print studentID
        ecall
	ret
inputLast:
	li a7,5		#Reads an int from input console
	ecall
	ret	
inputFirst:
	li a7,5		#Reads an int from input console
	ecall
	ret
inputSecond:
	li a7,5		#Reads an int from input console
	ecall
	ret
printMsg1:
	la a0, str2			# prepare to print string 2
	li a7, 4			# print string
 	ecall
 	ret
printMsg2:
	la a0, str3			# prepare to print string 3
	li a7, 4			# print string
 	ecall
 	ret
 printMsg3:
	la a0, str4			# prepare to print string 3
	li a7, 4			# print string
 	ecall
 	ret
computeWuXin:
	mv t1,a0
	addi sp,sp,-4
	sw ra,0(sp)
	blez t1,ModeWater #12(Wood),34(Fire),56(Earth),78(Metal),90(Water)	
	li a2,2	 
	ble t1,a2,ModeWood
	li a2,4
	ble t1,a2,ModeFire
	li a2,6	 
	ble t1,a2,ModeEarth
	li a2,8
	ble t1,a2,ModeMetal
	li a2,9
	ble t1,a2,ModeWater
	
endcomputeWuXin: 
	lw ra,0(sp)
	addi sp,sp,4
	ret  #return (a0)

ModeWood:
	la a0,Wood
	j endcomputeWuXin
	
ModeFire:
	la a0,Fire
	j endcomputeWuXin

ModeEarth:
	la a0,Earth
	j endcomputeWuXin

ModeMetal:
	la a0,Metal
	j endcomputeWuXin

ModeWater:
	la a0,Water
	j endcomputeWuXin
printWuXinResult:
	li a7, 4			# print string
 	ecall
 	ret
end:
	li a7, 10			
 	ecall