.code
compute_function proc
    sub rsp, 8
    movsd qword ptr [rsp], xmm0  ; сохраняем x
    
    ; Загружаем x в стек FPU
    fld qword ptr [rsp]           ; st(0) = x
    
    ; Вычисляем 2*x^2
    fld st(0)                     ; st(0) = x, st(1) = x
    fmul st(0), st(0)              ; st(0) = x^2, st(1) = x
    fadd st(0), st(0)              ; st(0) = 2*x^2, st(1) = x
    
    ; Вычисляем x + 2*x^2
    fadd st(0), st(1)              ; st(0) = x + 2*x^2, st(1) = x
    
    ; Меняем местами для деления
    fxch st(1)                     ; st(0) = x, st(1) = x + 2*x^2
    
    ; Вычисляем x - 1
    fld1                           ; st(0) = 1, st(1) = x, st(2) = x+2x^2
    fsubp st(1), st(0)              ; st(0) = x-1, st(1) = x+2x^2
    
    ; Делим (x+2x^2)/(x-1)
    fdivp st(1), st(0)              ; st(0) = (x+2x^2)/(x-1)
    
    ; Сохраняем результат
    fstp qword ptr [rsp]
    movsd xmm0, qword ptr [rsp]
    add rsp, 8
    ret
compute_function endp
end