ExitProcess PROTO

.data

.code
main PROC

	sub rsp, 28h   ;reserved the stack area as parameter passing area
	mov rcx, 54   ; specify Exit Code
	call ExitProcess
main ENDP

END
