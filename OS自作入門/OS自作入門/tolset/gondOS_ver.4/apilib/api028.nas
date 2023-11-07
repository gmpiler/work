[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api028.nas"]

		GLOBAL	_api_sprintf

[SECTION .text]

_api_sprintf:	; void api_sprintf(char *addressToWrite, char *sthToWrite);
		PUSH	EBX
		MOV		EDX,28
		MOV		EBX,[ESP+ 8]	; addressToWrite
		MOV		ECX,[ESP+12]	; sthToWrite
		INT		0x40
		POP		EBX
		RET
