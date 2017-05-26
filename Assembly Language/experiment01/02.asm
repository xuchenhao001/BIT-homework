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
PINA    DB  'Please input 20 numbers to A:', 0DH, 0AH, '$'
PINB    DB  'Please input 20 numbers to B:', 0DH, 0AH, '$'
PINER   DB  'No Result!', 0DH, 0AH, '$'
AFTSC   DB  'After search:','$'
PINSP   DB  ' ', '$'
PA      DB  'A:', '$'
PB      DB  'B:', '$'
; array A, B
A		DB	20  DUP(0)
B		DB	20  DUP(0)
C       DB  20  DUP(0)
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
		; start input and sort A & B
		LEA     BX, A
		LEA     DX, PINA
		CALL    PSTR
		CALL    INARR
		CALL    SORT
		LEA     DX, PA
		CALL    PSTR
		MOV     CX, 20
        LEA     BX, A
        CALL    OUTARR
        CALL    NEWL
		
		LEA     BX, B
		LEA     DX, PINB
		CALL    PSTR
		CALL    INARR
		CALL    SORT
		LEA     DX, PB
		CALL    PSTR
		MOV     CX, 20
        LEA     BX, B
        CALL    OUTARR
        CALL    NEWL
        LEA     DX, AFTSC
        CALL    PSTR
        CALL    NEWL
		
		; start search to array C
		MOV     SI, 19
		MOV     DI, 19
		XOR     BX, BX
SEARCH:		
		MOV     AL, A[SI]
		CMP     AL, B[DI]
		JB      MINIA        
		JA      MINIB        
        ; A == B
        CMP     BX, 0
        JE      NOSAM
        CMP     C[BX-1], AL   ;judge if the same number has been record
        JE      HAVSAM
NOSAM:  ; there is no the same number      
        MOV     C[BX], AL
        INC     BX
HAVSAM: ; there is the same number
        CMP     SI, 0
        JZ      ESEARCH
        CMP     DI, 0
        JZ      ESEARCH
        DEC     SI
        DEC     DI
        JMP     SEARCH                     
MINIA:  ; A < B
        CMP     DI, 0
        JZ      ESEARCH   
        DEC     DI
        JMP     SEARCH
MINIB:  ; A > B
        CMP     SI, 0
        JZ      ESEARCH   
        DEC     SI
        JMP     SEARCH
ESEARCH:; search end
        MOV     CX, BX
        LEA     BX, C
        CALL    OUTARR
		
		MOV		AX, 4C00H                
		INT		21H			
MAIN	ENDP 

; sort 20 numbers in array BX
SORT    PROC    NEAR
        PUSH    SI
        PUSH    CX
        PUSH    AX
        PUSH    DI
        PUSH    DX
        
        XOR     SI, SI  ; SI seems like i
        MOV     CX, 19 
OUTLOP: 
        MOV     AL, [BX][SI]
        PUSH    CX
        MOV     DI, SI  ; DI seems like j     
        PUSH    SI 
INLOP: 
        INC     DI
        MOV     DL, [BX][DI]
        CMP     DL, AL
        JB      FDB           ; if find out a lower one
        LOOP    INLOP
        JMP     FNR
FDB:    MOV     SI, DI
        MOV     AL, DL
        LOOP    INLOP    
FNR:    POP     DI            ; find out the 1st of this ring
        XCHG    [BX][DI], AL  ; exchange the 1st with the mini
        MOV     [BX][SI], AL  ; place the 1st to old mini
        MOV     SI, DI
        INC     SI
        POP     CX
        LOOP    OUTLOP 
        
        POP     DX
        POP     DI
        POP     AX
        POP     CX
        POP     SI
        RET
SORT    ENDP
; end sort

; output numbers in array BX, whose size is in CX
OUTARR  PROC    NEAR
        PUSH    DI
        PUSH    CX
        PUSH    DX
        PUSH    AX
        
        XOR     DI, DI
        CMP     CX, 0
        JNE     NORMOUT
        LEA     DX, PINER
        CALL    PSTR
        JMP     EOUTARR
NORMOUT:
        XOR     AH, AH             
        MOV     AL, [BX][DI]
        PUSH    DI
        CALL    PNUM
        LEA     DX, PINSP
        CALL    PSTR
        POP     DI
        INC     DI
        LOOP    NORMOUT
EOUTARR:
        POP     AX
        POP     DX
        POP     CX
        POP     DI        
        RET
OUTARR  ENDP 
; end output numbers

; input 20 numbers to array [BX]
INARR   PROC    NEAR
        PUSH    DI
        PUSH    CX
        PUSH    AX
        
        MOV     DI, 0
        MOV     CX, 20
NXTNUM: 
        CALL    INNUM
        MOV     [BX][DI], AL
        INC     DI
        CALL    NEWL
        LOOP    NXTNUM
        
        POP     AX
        POP     CX
        POP     DI
        RET
INARR   ENDP

; input an unsigned integer to AX
INNUM   PROC    NEAR
        PUSH    CX
        PUSH    DX
        PUSH    DI
        
    	MOV		AH, 0AH		;receive a string to buffer
		LEA		DX,	BUFFER
		INT		21H
		
		XOR     CH, CH
		MOV		CL, BUFFER[1]
		MOV		DI, 2
		XOR		AX, AX
		MOV		DH, 10
CVTI:	
        MUL		DH
		MOV		DL, BUFFER[DI]
		SUB		DL, 48
		ADD		AL, DL
		ADC     AH, 0
		INC		DI
		LOOP	CVTI
		
		POP     DI
		POP     DX
		POP     CX
		RET
INNUM   ENDP
; input finished

; print the number in AX 
PNUM    PROC    NEAR
        PUSH    BX
        PUSH    DX
        PUSH    DI
        
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
        
        POP     DI
        POP     DX
        POP     BX
        RET                         
PNUM    ENDP
; print number finished

; print the string in [DX]
PSTR    PROC    NEAR
        PUSH    AX
        
        MOV     AH, 9
        INT     21H
        
        POP     AX
        RET
PSTR    ENDP
; print string finished

; set cursor to a new line
NEWL    PROC    NEAR
        PUSH    AX
        PUSH    DX
        
        MOV     AH, 2
        MOV     DL, 13 ;CR
        INT     21H
        MOV     AH, 2
        MOV     DL, 10 ;LF
        INT     21H
        
        POP     DX
        POP     AX
        RET
NEWL    ENDP
; end set cursor to new line

CODE	ENDS
		END	MAIN

