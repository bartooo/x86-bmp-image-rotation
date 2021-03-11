;-------------------------------------------------------------------------------
;author    		  : Bartosz Cywiński
;description	  : Rotation of black-white BMP image 
;-------------------------------------------------------------------------------

section .text
global copyLines

copyLines:
; 	arguments of function:
;	rdi - unsigned char* pointer to allocated memory
;	rsi - unsigned char* pointer to source image
;	rdx - int width modulo 8
;	rcx - int height of image
;	r8 - int length of lines in source image in bytes


prolog:
	push	rbp
	mov	rbp, rsp
	
	mov r10, 0			; r10 = current height
	mov r9, rcx			; r9 = total height 
	mov ch, 0			; register
	mov cl, dl			

while_loop:
	cmp r10, r9
	jge loop_exit
	mov al, byte [rsi]
	sal al, cl
	shr al, 7
	shl ch, 1
	or  ch, al
	inc r10
	sub rsi, r8			; go to next line

	;if loop
	test r10, 7		
	jnz while_loop
	
copy_byte:
	mov byte[rdi], ch	; stores byte from register
	inc rdi				; go to next column
	jmp while_loop
	
loop_exit:
	and r10, 7			; r10b - modulo 8 height of image
	mov cl, 8
	sub cl, r10b
	shl ch, cl
	mov byte[rdi], ch	; store last byte if image is not divisible by 8
	
epilog:
	pop	rbp
	ret