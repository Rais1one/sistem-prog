.386
.MODEL FLAT, C

EXTERN VAL: DWORD

.CODE
COPY PROC
    ; Просто записываем значение 4 в VAL
    mov eax, 4
    mov [VAL], eax
    ret
COPY ENDP

END