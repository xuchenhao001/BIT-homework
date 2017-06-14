.386
.model	flat, stdcall	;Use the flat memory model. Use stdcall
option casemap:none

includelib msvcrt.lib

; Function prototype
printf			PROTO C :dword, :VARARG
scanf			PROTO C :dword, :VARARG
qsort			PROTO C :dword, :dword, :dword, :dword
strcmp			PROTO C :dword, :dword

Student			struct
	sNum		DB	10	dup(?)
	sName		DB	10	dup(?)
	cmpPcp		DW	?
	datStr		DW	?
	asemly		DW	?
	sum			DW	?
Student			ends

; Create a data segment.
.data
	stuNo		Student	10	dup(<>)
	szPrompt	DB	'Please input student %d info: ', 0ah, 0
	inDataFm	DB	'%s %s %d %d %d', 0
	pFstBScor	DB	'The first 3 by score is: ', 0ah, 0
	pFstBNum	DB	'The first 5 by Student number: ', 0ah, 0
	outDataFm	DB	'    StuNom    StuNam    cmpPcp    datStr    asemly', 0ah, 0
	outData		DB	'%10s%10s%10d%10d%10d', 0ah, 0
	; jump table
	jmp_tab		DD	input
				DD	sortByScore
				DD	firstByScore
				DD	firstByNum
.data?
.const
; Indicates the start of a code segment.
.code
cmpSco			proc	C	a:dword, b:dword
	mov			ecx, [a]
	mov			eax, [b]
	mov			cx, (Student ptr [ecx]).sum
	mov			ax, (Student ptr [eax]).sum
	and			ecx, 0000ffffh
	and			eax, 0000ffffh
	sub			eax, ecx
	ret
cmpSco			endp

cmpNum			proc	C	a:dword, b:dword
	invoke		strcmp, [a], [b]
	ret
cmpNum			endp

input			proc	C
	mov			ecx, 10
	lea			ebx, stuNo
repinput:
	mov			eax, 11
	sub			eax, ecx
	push		ebx
	push		ecx

	invoke		printf, offset szPrompt, eax

	lea			eax, (Student ptr [ebx]).asemly
	push		eax
	lea			eax, (Student ptr [ebx]).datStr
	push		eax
	lea			eax, (Student ptr [ebx]).cmpPcp
	push		eax
	lea			eax, (Student ptr [ebx]).sName
	push		eax
	lea			eax, (Student ptr [ebx]).sNum
	push		eax
	lea			eax, inDataFm
	push		eax
	call		scanf			; invoke scanf
	add			esp,24

	; calculate the sum 
	mov			dx, (Student ptr [ebx]).asemly
	add			dx, (Student ptr [ebx]).datStr
	add			dx, (Student ptr [ebx]).cmpPcp
	mov			(Student ptr [ebx]).sum, dx

	pop			ecx
	pop			ebx
	add			ebx, type Student
	loop		repinput
	ret
input			endp

sortByScore		proc	C
	invoke		qsort, offset stuNo, 10, sizeof Student, offset cmpSco
	ret
sortByScore 	endp

firstByScore	proc	C
	invoke		printf, offset pFstBScor
	invoke		printf, offset outDataFm
	lea			ebx, stuNo
	mov			ecx, 3
prtFst3:
	push		ecx
	push		ebx
	invoke		printf, offset outData, addr (Student ptr [ebx]).sNum, addr (Student ptr [ebx]).sName, (Student ptr [ebx]).cmpPcp, (Student ptr [ebx]).datStr, (Student ptr [ebx]).asemly
	pop			ebx
	pop			ecx
	add			ebx, type Student
	loop		prtFst3
	ret
firstByScore 	endp

firstByNum		proc	C
	invoke		qsort, offset stuNo, 10, sizeof Student, offset cmpNum
	invoke		printf, offset pFstBNum
	invoke		printf, offset outDataFm
	lea			ebx, stuNo
	mov			ecx, 5
prtFst5:
	push		ecx
	push		ebx
	invoke		printf, offset outData, addr (Student ptr [ebx]).sNum, addr (Student ptr [ebx]).sName, (Student ptr [ebx]).cmpPcp, (Student ptr [ebx]).datStr, (Student ptr [ebx]).asemly
	pop			ebx
	pop			ecx
	add			ebx, type Student
	loop		prtFst5
	ret
firstByNum		endp

main			proc
	mov			ecx, 4
	lea			ebx, jmp_tab
mainLoop:
	push		ebx
	push		ecx

	call		dword ptr [ebx]

	pop			ecx
	pop			ebx
	add			ebx, type dword
	loop		mainLoop

	ret
main			endp
end main