.386
.model flat, stdcall
option casemap:none

includelib kernel32.lib
GetTickCount PROTO

.data
seed DWORD 0         

.code

_random_range PROC min:DWORD, max:DWORD
    ; Инициализация seed при первом вызове
    mov eax, seed
    cmp eax, 0
    jne skip_seed_init
    call GetTickCount
    mov seed, eax
skip_seed_init:

    ; Простейший генератор (LCG)
    mov eax, seed
    mov ecx, 214013
    mul ecx
    add eax, 2531011
    mov seed, eax

    ; Берём остаток, чтобы получить число в диапазоне [min, max]
    mov ecx, max
    sub ecx, min
    inc ecx           ; ecx = (max - min + 1)
    xor edx, edx
    div ecx           ; edx = остаток
    add edx, min
    mov eax, edx      ; возвращаемое число

    ret
_random_range ENDP

END