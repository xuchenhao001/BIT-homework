STSG	SEGMENT	STACK	'S'
		DW  32	DUP(?)
STSG	ENDS

; start data segment
DATA	SEGMENT
; input buffer
BUFFER	DB	10    
		DB  0
		DB	10	DUP(0)
; just for output format
PAC     DB  'After Calculate:', '$'
PA      DB  'A:', '$'
PB      DB  'B:', '$'
; variable A, B
A		DW	0
B		DW	0
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
		; start input A & B
		CALL    INPUT
		MOV     A, AX
		CALL    NEWL
		
		CALL    INPUT
		MOV     B, AX
		CALL    NEWL
		
		; start judge & calculate
		TEST    A, 1    
		JNZ     AODD
		TEST    B, 1    
		JNZ     BODD
		; odd number A:N B:N
		; do nothing
		JMP     END
		                
        ; odd number A:N B:Y
BODD:   MOV     AX, B
        MOV     BX, A
        MOV     A, AX
        MOV     B, BX
        JMP     END    
		
AODD:   TEST    B, 1    
        JNZ     ABODD
        ; odd number A:Y B:N                 
        ; do nothing
        JMP     END
                       
        ; odd number A:Y B:Y
ABODD:  ADD     A, 1
        ADD     B, 1

    	; after all
END:    MOV     AH, 9
        LEA     DX, PAC
        INT     21H
        CALL    NEWL
        
        MOV     AH, 9
        LEA     DX, PA
        INT     21H
        MOV     AX, A
		CALL    PRINT
		CALL    NEWL
		
		MOV     AH, 9
        LEA     DX, PB
        INT     21H
        MOV     AX, B
		CALL    PRINT
		CALL    NEWL
		
		MOV		AX, 4C00H
		INT		21H			
MAIN	ENDP 

; input an unsigned integer to AX
INPUT   PROC    NEAR
    	MOV		AH, 0AH		;receive a string to buffer
		LEA		DX,	BUFFER
		INT		21H
		
		XOR     CH, CH
		MOV		CL, BUFFER[1]
		MOV		DI, 2
		XOR		AX, AX
		MOV		BL, 10
		MOV		DH, 0
CVTI:	MUL		BL
		MOV		DL, BUFFER[DI]
		SUB		DL, 48
		ADD		AX, DX
		INC		DI
		LOOP	CVTI
		RET
INPUT   ENDP
; input finished

; print the number in AX 
PRINT   PROC    NEAR
        MOV     DI, 10
        MOV     BUFFER[11], '$' ;the last character is $
        MOV		BL, 10
CVTS:   DIV     BL
        ADD     AH, 48
        MOV     BUFFER[DI], AH
        XOR     AH, AH
        CMP     AL, 0
        JE      DIVTZ
        DEC     DI
        LOOP    CVTS
DIVTZ:  MOV     AH, 9
        LEA     DX, BUFFER[DI]
        INT     21H
        RET                         
PRINT   ENDP
; print finished

; set cursor to a new line
NEWL    PROC    NEAR
        MOV     AH, 2
        MOV     DL, 13 ;CR
        INT     21H
        MOV     AH, 2
        MOV     DL, 10 ;LF
        INT     21H
        RET
NEWL    ENDP
; end set cursor

CODE	ENDS
		END	MAIN

