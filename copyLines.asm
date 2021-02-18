section .text
global copyLines

copyLines:
; 	arguments of function:
;	[ebp+8] - unsigned char* pointer to allocated memory
;	[ebp+12] - unsigned char* pointer to source image
;	[ebp+16] - int width modulo 8
;	[ebp+20] - int height of image
;	[ebp+24] - int length of lines in source image in bytes


prolog:
	push	ebp
	mov	ebp, esp
	
	mov ebx, 0			; height
	
	mov ch, 0			; register
	mov cl, [ebp+16]	; [ebp+16] - modulo 8 of width
	
	mov esi, [ebp+8]	; writePtr
	mov edi, [ebp+12]	; readPtr
	mov edx, [ebp+20]	; [ebp+20] - height of image

while_loop:
	cmp ebx, edx
	jge loop_exit
	mov al, byte [edi]
	sal al, cl
	shr al, 7
	shl ch, 1
	or  ch, al
	inc ebx
	sub edi, [ebp+24]	; go to next line

	;if loop
	mov eax, ebx
	and eax, 7			; modulo 8 of current height
	cmp al, 0
	jne while_loop
	
copy_byte:
	mov byte[esi], ch	; stores byte from register
	inc esi				; go to next column
	jmp while_loop
	
loop_exit:
	and edx, 7			; dl - modulo 8 height of image
	mov cl, 8
	sub cl, dl
	shl ch, cl
	mov byte[esi], ch	; store last byte if image is not divisible by 8
	
epilog:
	pop	ebp
	ret