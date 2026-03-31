.386
.model flat, c

public Laba6

.code
Laba6 proc
    push ebp
    mov ebp, esp
    push esi
    push edi

    mov esi, [ebp + 8]    ; Источник (откуда читаем)
    mov edi, esi          ; Приемник (куда пишем)

remove_spaces:
    lodsb                 ; AL = [ESI], ESI++
    cmp al, 0             ; Конец строки (нуль-терминатор)?
    je done               ; Если да, выходим из цикла
    
    cmp al, ' '           ; Проверяем на пробел
    je remove_spaces      ; Если пробел, просто идем к следующему символу (ESI уже сдвинулся)
    
    stosb                 ; Если не пробел, записываем в [EDI], EDI++
    jmp remove_spaces

done:
    mov byte ptr [edi], 0 ; ОБЯЗАТЕЛЬНО ставим 0 в новый конец строки

    pop edi
    pop esi
    pop ebp
    ret
Laba6 endp

end
