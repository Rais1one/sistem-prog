.386
.model flat

extern _VAL2: DWORD

.code
_FUNC proc
	mov eax, [esp+4]
	add eax, [esp+8]
	add eax, [esp+12]
	mov [_VAL2], eax
	ret
_FUNC endp
end