STSG	SEGMENT	STACK	'S'
		DW	64	DUP(0)
STSG	ENDS

; start data segment
DATA	SEGMENT
		COUNT5	DW	1	; timer for 0.5s
		COUNT1	DW	1	; timer for 0.1s
		DERTX	DB	1	; cursor move dertaX
		DERTY	DB	1	; cursor move dertaY
		BGCLOR	DB	0FH	; background color
DATA	ENDS

; start code segment
CODE	SEGMENT
MAIN	PROC	FAR
ASSUME	CS:CODE, DS:DATA, SS:STSG, ES:NOTHING
		; set DS to current data segment
		MOV		AX, DATA
		MOV		DS, AX

		; here is main process
		; save the interrupt vector
		MOV		AX, 351CH
		INT		21H
		PUSH	ES
		PUSH	BX

		; set new interrupt vector
		PUSH	DS
		MOV		DX, SEG INT_1CH
		MOV		DS, DX
		LEA		DX, INT_1CH
		MOV		AX, 251CH
		INT		21H
		POP		DS

		; set timer & keyboard interrupt
		IN		AL, 21H
		AND		AL, 11111100B
		OUT		21H, AL

		; start interrupt
		STI

		; set 80x25 colorful mode
		MOV		AH, 00H
		MOV		AL, 03H
		INT		10H

		; hide cursor
		MOV		AH, 01H
		MOV		CX, 2000H
		INT		10H

NOTHG:
		; input a char, if CR exit
		MOV		AH, 0
		INT		16H
		CMP		AL, 0DH
		JE		EXITMAN
		JMP		NOTHG

EXITMAN:
		; recovery interrupt vector
		POP		DX
		POP		DS
		MOV		AX, 251CH
		INT		21H

		MOV		AX, 4C00H
		INT		21H
MAIN	ENDP

; input string to BUFFER
INT_1CH	PROC	FAR
		PUSH	AX
		PUSH	BX
		PUSH	CX
		PUSH	DX
		PUSH	DS

		DEC		COUNT5
		JNZ		NBGCLOR

		; change the background color
		ADD		BGCLOR, 00010000B
		AND		BGCLOR, 01111111B

		; 0.5s counter
		MOV		COUNT5, 9

NBGCLOR:
		DEC		COUNT1
		JNZ		ENDINT

		; clear screen
		MOV		AH, 6
		MOV		AL, 0
		; set blue background and white letter
		MOV		BH, BGCLOR
		XOR		CX, CX
		MOV		DX, 184FH
		INT		10H

		; read cursor position
		MOV		AH, 3
		MOV		BH, 0
		INT		10H

		; update cursor position
		ADD		DL, DERTX
		ADD		DH, DERTY
		CMP		DL, 79
		JNE		NOVERX
		MOV		DERTX, 0FFH
	NOVERX:
		CMP		DH, 24
		JNE		NOVERY
		MOV		DERTY, 0FFH
	NOVERY:
		CMP		DL, 0
		JNE		NOLESX
		MOV		DERTX, 1
	NOLESX:
		CMP		DH, 0
		JNE		NOLESY
		MOV		DERTY, 1
	NOLESY:
		; set cursor position
		MOV		AH, 02H
		MOV		BH, 0
		INT		10H

		; set char at cursor
		MOV		AH, 09H
		MOV		AL, '*'
		MOV		BH, 0
		MOV		BL, BGCLOR
		MOV		CX, 1
		INT		10H

		; 0.1s counter
		MOV		COUNT1, 2
ENDINT:
		POP		DS
		POP		DX
		POP		CX
		POP		BX
		POP		AX
		IRET ; return the interrupt
INT_1CH	ENDP

CODE	ENDS
END	MAIN

