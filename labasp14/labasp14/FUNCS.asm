.386
.model flat

extern _VAL3: DWORD

.code
_FUNCS proc
	mov eax, [esp+4]
	add eax, [esp+8]
	add eax, [esp+12]
	mov [_VAL3], eax
	ret
_FUNCS endp
end