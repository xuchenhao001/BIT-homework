STSG	SEGMENT	STACK	'S'
		DW	50	DUP(0)
STSG	ENDS

; start data segment
DATA	SEGMENT
; input buffer
BUFFER	DB	50
		DB	0
		DB	50	DUP(0)

; just for output format
PSTCA	DB	'Please input the sentence A:', 0DH, 0AH, '$'
PSTCB	DB	'Please input the sentence B:', 0DH, 0AH, '$'
PSWRD	DB	'The longest word in both A & B is:', 0DH, 0AH, '$'
PSERR	DB	'There is no word in both A & B!', 0DH, 0AH, '$'

; sentence A & B
SENTCA	DB	55	DUP(0)	; record the sentence A in buffer
		DB	20H
SENTCB	DB	55	DUP(0)	; record the sentence B in buffer
		DB	20H
COUNT	DB	0
MAX		DB	0
MAXST	DB	0
MAXED	DB	0
DATA	ENDS

; start code segment
CODE	SEGMENT
MAIN	PROC	FAR
ASSUME	CS:CODE, DS:DATA, SS:STSG, ES:NOTHING
		; set DS to current data segment
		MOV		AX, DATA
		MOV		DS, AX

		; here is main process
		LEA		DX, PSTCA
		CALL	PSTR
		CALL	INSTR
		LEA		BX, SENTCA
		CALL	CPBF
		CALL	NEWL

		LEA		DX, PSTCB
		CALL	PSTR
		CALL	INSTR
		LEA		BX, SENTCB
		CALL	CPBF
		CALL	NEWL
		; find and record the longest word to SENTCB[MAXST, MAXED]
		; whose length is MAX
		CALL	LSTWORD
		
		; print the answer
		CMP		MAX, 0
		JE		NOFIND
		LEA		DX, PSWRD
		CALL	PSTR
		XOR		BH, BH
		MOV		BL, MAXED
		MOV		SENTCB[BX], '$'
		MOV		BL, MAXST
		LEA		DX, SENTCB[BX+1]
		CALL	PSTR
		JMP		ENDFD
NOFIND:	; no answer
		LEA		DX, PSERR
		CALL	PSTR
ENDFD:
		MOV		AX, 4C00H
		INT		21H
MAIN	ENDP

; search the longest word in both SENTCA & SENTCB
LSTWORD	PROC	NEAR
		PUSH	AX
		PUSH	SI
		PUSH	DI

		XOR		SI, SI
		CMP		SENTCA[0], 0
		JNA		EOSERC
		INC		SI
OSERCH:	; outer loop, sentence A
		MOV		AX, SI
		CMP		AL, SENTCA[0]
		JAE		EOSERC

		CMP		SENTCA[SI], 20H
		JNE		EISECH

		XOR		DI, DI
		CMP		SENTCB[0], 0
		JNA		EISECH
		INC		DI
ISERCH:	; inner loop, sentence B
		MOV		AX, DI
		CMP		AL, SENTCB[0]
		JAE		EISECH
		PUSH	SI
		PUSH	DI
		CMP		SENTCB[DI], 20H
		JNE		CISECH
		MOV		COUNT, 0

		INC		SI
		INC		DI
LPSECH:	; find match, count for word length
		MOV		AL, SENTCA[SI]
		CMP		AL, SENTCB[DI]
		JNE		CISECH
		CMP		AL, 20H
		JE		JZMAX
		ADD		COUNT, 1
		INC		SI
		INC		DI
		JMP		LPSECH
JZMAX:	; find a longer match word, record to value MAX, MAXST and MAXED
		MOV		AL, MAX
		CMP		COUNT, AL
		JNA		CISECH
		CMP		SENTCA[SI], 20H
		JNE		CISECH
		MOV		AL, COUNT
		MOV		MAX, AL
		MOV		AX, DI
		MOV		MAXED, AL
		POP		DI
		PUSH	DI
		MOV		AX, DI
		MOV		MAXST, AL
CISECH:
		POP		DI
		POP		SI
		INC		DI
		JMP		ISERCH
		; record max value end
EISECH:
		INC		SI
		JMP		OSERCH
		; inner loop end
EOSERC:
		; outer loop end
		POP		DI
		POP		SI
		POP		AX
		RET
LSTWORD	ENDP
; end search

; copy BUFFER to [BX]
CPBF	PROC	NEAR
		PUSH	AX
		PUSH	CX
		PUSH	SI

		XOR		SI, SI
		MOV		AL, BUFFER[1]
		MOV		[BX], AL
		ADD		[BX], 1
		MOV		[BX+1], 20H	;insert a space to string start
		XOR		CH, CH
		MOV		CL, BUFFER[1]
CPBFLP:
		MOV		AL, BUFFER[SI+2]
		MOV		[BX][SI+2], AL
		INC		SI
		LOOP	CPBFLP
		MOV		[BX][SI+2], 20H	;insert a space to string end 
		
		POP		SI
		POP		CX
		POP		AX
		RET
CPBF	ENDP
; end copy BUFFER

; input string to BUFFER
INSTR	PROC	NEAR
		PUSH	AX
		PUSH	DX

		MOV		AH, 0AH
		LEA		DX, BUFFER
		INT		21H

		POP		DX
		POP		AX
		RET
INSTR	ENDP
; input input string to BUFFER

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
