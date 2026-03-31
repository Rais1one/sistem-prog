.386
.model flat, stdcall
option casemap :none

.code

_solve_quadratic PROC a:REAL4, b:REAL4, coeff_c:REAL4, x1:PTR REAL4, x2:PTR REAL4
    ; Проверка a == 0
    fld a
    fldz
    fcomip st(0), st(1)
    je dot_not_quadratic
    ; st(0) = a
    
    ; Вычисляем D = b*b - 4*a*c
    fld b
    fmul st(0), st(0)      ; st(0) = b^2, st(1) = a
    
    fld a
    fmul coeff_c           ; st(0) = a*c, st(1) = b^2, st(2) = a
    fld st(0)              ; Дублируем a*c
    faddp st(1), st(0)     ; st(0) = 2ac, st(1) = b^2, st(2) = a
    fadd st(0), st(0)      ; st(0) = 4ac, st(1) = b^2, st(2) = a
    
    fsubp st(1), st(0)     ; st(0) = D, st(1) = a

    ; Проверка D < 0
    fldz
    fcomip st(0), st(1)
    ja dot_no_roots        ; Если 0 > D

    ; sqrt(D)
    fsqrt                  ; st(0) = sqrt(D), st(1) = a
    
    ; Вычисляем 2*a для знаменателя
    fld st(1)              ; Дублируем a
    fadd st(0), st(0)      ; st(0) = 2a, st(1) = sqrt(D), st(2) = a
    
    ; x1 = (-b + sqrt(D)) / (2*a)
    fld b
    fchs                   ; st(0) = -b, st(1) = 2a, st(2) = sqrt(D), st(3) = a
    
    fld st(2)              ; st(0) = sqrt(D), st(1) = -b, st(2) = 2a, st(3) = sqrt(D), st(4) = a
    faddp st(1), st(0)     ; st(0) = -b + sqrt(D), st(1) = 2a, st(2) = sqrt(D), st(3) = a
    
    fdiv st(0), st(1)      ; st(0) = x1, st(1) = 2a, st(2) = sqrt(D), st(3) = a
    mov edx, x1
    fstp dword ptr [edx]   ; Сохраняем x1 и убираем его из стека
    
    ; x2 = (-b - sqrt(D)) / (2*a)
    ; st(0) = 2a, st(1) = sqrt(D), st(2) = a
    fld b
    fchs                   ; st(0) = -b, st(1) = 2a, st(2) = sqrt(D), st(3) = a
    
    fsub st(0), st(2)      ; st(0) = -b - sqrt(D), st(1) = 2a, st(2) = sqrt(D), st(3) = a
    
    fdiv st(0), st(1)      ; st(0) = x2, st(1) = 2a, st(2) = sqrt(D), st(3) = a
    mov edx, x2
    fstp dword ptr [edx]   ; Сохраняем x2 и убираем его из стека
    
    ; Очищаем стек
    fstp st(0)             ; Убираем 2a
    fstp st(0)             ; Убираем sqrt(D)
    fstp st(0)             ; Убираем a
    
    mov eax, 2
    ret

dot_no_roots:
    ; Очищаем стек при D < 0
    fstp st(0)             ; Убираем D
    fstp st(0)             ; Убираем a
    mov eax, 0
    ret

dot_not_quadratic:
    fstp st(0)             ; Убираем a
    mov eax, -1
    ret

_solve_quadratic ENDP
END