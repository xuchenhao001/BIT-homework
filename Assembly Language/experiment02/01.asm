STSG	SEGMENT	STACK	'S'
		DW	32	DUP(?)
STSG	ENDS

; start data segment
DATA	SEGMENT
; input buffer
BUFFER	DB	10
		DB	0
		DB	10	DUP(0)

; just for output format
PINDATE	DB	'Please input the date (yyyymmdd):', 0DH, 0AH, '$'
PSDAY1	DB	'It is the ', '$'
PSDAY2	DB	' day in the year', 0DH, 0AH,'$'
N2WD	DB	'SUN$MON$TUE$WED$THU$FRI$SAT$'
PWN		DB	'It is week ', '$'
PPAUC	DB	', ', '$'

; array YEAR, MONTH, DAY
YEAR	DW	0
MONTH	DB	0
DAY		DB	0
DAYSUM	DW	0
WEEKDAY	DB	0
WEEKNUM	DB	0
; for calculate
M2D		DW	0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
FLEAP	DB	0
DATA	ENDS

; start code segment
CODE	SEGMENT
MAIN	PROC	FAR
ASSUME	CS:CODE, DS:DATA, SS:STSG, ES:NOTHING
		PUSH	DS
		XOR		AX, AX
		PUSH	AX
		MOV		AX, DATA
		MOV		DS, AX

		; here is main process
		; input data
		LEA		DX, PINDATE
		CALL	PSTR
		CALL	INDATE
		CALL	NEWL

		CALL	DAYNUM
		; output daysum
		LEA		DX, PSDAY1
		CALL	PSTR
		MOV		AX, DAYSUM
		CALL	PNUM
		LEA		DX, PSDAY2
		CALL	PSTR

		CALL	NUMWEEK
		CALL	DAYWEEK
		; output week number
		LEA		DX, PWN
		CALL	PSTR
		MOV		AL, WEEKNUM
		XOR		AH, AH
		CALL	PNUM
		LEA		DX, PPAUC
		CALL	PSTR
		; output weekday
		MOV		AL, WEEKDAY
		MOV		BL, 4
		MUL		BL
		MOV		BX, AX
		LEA		DX, N2WD[BX]
		CALL	PSTR
		CALL	NEWL

		MOV		AX, 4C00H
		INT		21H
MAIN	ENDP

; calculate the week num of the date
NUMWEEK	PROC	NEAR
		PUSH	AX
		PUSH	BX
		PUSH	DX
		MOV		AL, MONTH
		MOV		BL, DAY
		PUSH	AX
		PUSH	BX

		MOV		MONTH, 1
		MOV		DAY, 1
		CALL	DAYWEEK
		MOV		AL, WEEKDAY
		XOR		AH, AH
		ADD		AX, DAYSUM
		SUB		AX, 1
		XOR		DX, DX
		MOV		BX, 7
		DIV		BX
		ADD		AL, 1
		MOV		WEEKNUM, AL

		POP		BX
		POP		AX
		MOV		DAY, BL
		MOV		MONTH, AL
		POP		DX
		POP		BX
		POP		AX
		RET
NUMWEEK	ENDP
; end calculate the week num of the date

; calculate the weekday of the date
DAYWEEK	PROC	NEAR
		PUSH	AX
		PUSH	BX
		PUSH	CX
		PUSH	DX

		; Zeller 
		MOV		AX, YEAR
		CMP		MONTH, 2
		JA		ADJY
		SUB		AX, 1
ADJY:
		XOR		DX, DX
		MOV		BX, 100
		DIV		BX
		MOV		CX, DX	; DX = y, w=y
		PUSH	AX		; AX = c
		MOV		AX, DX
		MOV		BX, 4
		XOR		DX, DX
		DIV		BX
		ADD		CX, AX	; w=y+[y/4]
		POP		AX		; AX = c
		PUSH	AX
		XOR		DX, DX
		DIV		BX
		ADD		CX, AX	; w=y+[y/4]+[c/4]
		POP		AX
		ADD		AX, AX
		SUB		CX, AX	; w=y+[y/4]+[c/4]-2c
		MOV		AL, MONTH
		CMP		AL, 2
		JA		ADJM
		ADD		AL, 12
ADJM:
		XOR		AH, AH
		ADD		AX, 1
		MOV		BX, 26
		MUL		BX
		MOV		BX, 10
		XOR		DX, DX
		DIV		BX
		ADD		CX, AX	; w=y+[y/4]+[c/4]-2c+[26(m+1)/10]
		ADD		CL, DAY
		ADC		CH, 0
		SUB		CX, 1
		MOV		AX, CX
		MOV		BX, 7
		XOR		DX, DX
		DIV		BX
		MOV		WEEKDAY, DL
		
		POP		DX
		POP		CX
		POP		BX
		POP		AX
		RET
DAYWEEK	ENDP
; end calculate the weekday of the date

; calculate the num of the day in a year to AX
DAYNUM	PROC	NEAR
		PUSH	AX
		PUSH	BX

		MOV		BL, MONTH
		ADD		BL, MONTH
		XOR		BH, BH
		MOV		AX, M2D[BX]
		ADD		AL, DAY
		ADC		AH, 0
		; consider February
		CMP		BX, 2
		JNA		NOFEB
		; judge if it is a leap year
		CALL	LYEAR
		CMP		FLEAP, 0
		JE		NOFEB
		; need to consider February
		ADD		AX, 1
NOFEB:	; no need to consider February
		MOV		DAYSUM, AX
		POP		BX
		POP		AX
DAYNUM	ENDP
; end calculate the num of the day in a year

; judge leap year
LYEAR	PROC	NEAR
		PUSH	AX
		PUSH	BX
		PUSH	DX

		MOV		AX, YEAR
		; can be divided by 400
		PUSH	AX
		MOV		BX, 400
		XOR		DX, DX
		DIV		BX
		POP		AX
		CMP		DX, 0
		JE		ISLEAP
		; can be divided by 4
		PUSH	AX
		MOV		BX, 4
		XOR		DX, DX
		DIV		BX
		POP		AX
		CMP		DX, 0
		JNE		NOLEAP
		; can be divided by 100
		MOV		BX, 100
		XOR		DX, DX
		DIV		BX
		CMP		DX, 0
		JE		NOLEAP
ISLEAP:	
		MOV		FLEAP, 1
		JMP		ELEAP
NOLEAP:	
		MOV		FLEAP, 0
ELEAP:
		POP		DX
		POP		BX
		POP		AX
		RET
LYEAR	ENDP
; end judge leap year

; input year, month, date to array YEAR, MONTH, DAY
INDATE	PROC	NEAR
		PUSH	AX
		PUSH	BX
		PUSH	CX
		PUSH	DX
		PUSH	DI

		; input string to buffer
		MOV		AH, 0AH
		LEA		DX, BUFFER
		INT		21H

		; input year
		XOR		CH, CH
		MOV		CL, 4
		MOV		DI, 2
		XOR		AX, AX
		MOV		BX, 10
CVTI1:
		MUL		BX
		SUB		BUFFER[DI], 48
		ADD		AL, BUFFER[DI]
		ADC		AH, 0
		INC		DI
		LOOP	CVTI1
		MOV		YEAR, AX

		; input month
		MOV		CL, 2
		MOV		DI, 6
		XOR		AX, AX
CVTI2:
		MUL		BX
		SUB		BUFFER[DI], 48
		ADD		AL, BUFFER[DI]
		ADC		AH, 0
		INC		DI
		LOOP	CVTI2
		MOV		MONTH, AL

		; input day
		MOV		CL, 2
		MOV		DI, 8
		XOR		AX, AX
CVTI3:
		MUL		BX
		SUB		BUFFER[DI], 48
		ADD		AL, BUFFER[DI]
		ADC		AH, 0
		INC		DI
		LOOP	CVTI3
		MOV		DAY, AL

		POP		DI
		POP		DX
		POP		CX
		POP		BX
		POP		AX
		RET
INDATE	ENDP
; input year finished

; print the number in AX 
PNUM	PROC	NEAR
		PUSH	BX
		PUSH	DX
		PUSH	DI

		MOV		DI, 10
		MOV		BUFFER[11], '$' ;the last character is $
		MOV		BL, 10
CVTS:	DIV		BL
		ADD		AH, 48
		MOV		BUFFER[DI], AH
		XOR		AH, AH
		CMP		AL, 0
		JE		DIVTZ
		DEC		DI
		JMP		CVTS
DIVTZ:	MOV		AH, 9
		LEA		DX, BUFFER[DI]
		INT		21H

		POP		DI
		POP		DX
		POP		BX
		RET
PNUM	ENDP
; print number finished

; print the string in [DX]
PSTR	PROC	NEAR
		PUSH	AX

		MOV		AH, 9
		INT		21H

		POP		AX
		RET
PSTR	ENDP
; print string finished

; set cursor to a new line
NEWL	PROC	NEAR
		PUSH	AX
		PUSH	DX

		MOV		AH, 2
		MOV		DL, 13 ;CR
		INT		21H
		MOV		AH, 2
		MOV		DL, 10 ;LF
		INT		21H

		POP		DX
		POP		AX
		RET
NEWL	ENDP
; end set cursor to new line

CODE	ENDS
END	MAIN
