.386
.model flat, stdcall

; Прототипы внешних функций
extern GetStdHandle@4:PROC
extern WriteConsoleA@20:PROC
extern CharToOemA@8:PROC
extern ReadConsoleA@20:PROC
extern ExitProcess@4:PROC
extern lstrlenA@4:PROC
extern wsprintfA:PROC

.data
    strn db "Введите строку: ",13,10,0
    resultMsg db 13,10,"Результат замены пробелов на '_':",13,10,0
    countMsg db 13,10,"Количество пробелов: ",0
    notFoundMsg db "не найдено",0
    
    din dd ?
    dout dd ?
    buf db 200 dup (?)      ; исходный буфер
    resultBuf db 200 dup (?) ; буфер для результата
    lens dd ?
    count dd ?
    tempBuf db 16 dup(?)    ; буфер для числа
    newline db 13,10,0
    fmt db '%d',0

.code
main proc
    ; Перекодируем строку STRN
    mov eax, offset strn
    push eax
    push eax
    call CharToOemA@8

    ; Получим дескриптор ввода
    push -10
    call GetStdHandle@4
    mov din, eax

    ; Получим дескриптор вывода
    push -11
    call GetStdHandle@4
    mov dout, eax

    ; Определим длину строки STRN
    push offset strn
    call lstrlenA@4

    ; Вывод строки STRN
    push 0
    push offset lens
    push eax
    push offset strn
    push dout
    call WriteConsoleA@20

    ; Ввод строки
    push 0
    push offset lens
    push 200
    push offset buf
    push din
    call ReadConsoleA@20

    ; Убираем символы перевода строки (CR+LF) из введенной строки
    mov esi, offset buf
    xor ecx, ecx
    
find_end:
    mov al, [esi]
    cmp al, 13          ; возврат каретки?
    je fix_end
    cmp al, 10          ; перевод строки?
    je fix_end
    cmp al, 0
    je fix_end
    inc esi
    inc ecx
    jmp find_end
    
fix_end:
    mov byte ptr [esi], 0
    mov lens, ecx


    ; ============================================================
    ;  SCASB + STOSB - замена пробелов на '_'
    ; ============================================================
    
    
    ; Копируем строку с заменой пробелов
    lea esi, buf          ; источник
    lea edi, resultBuf    ; приемник
    mov ecx, lens         ; длина строки
    cld
    
replace_loop:
    lodsb                 ; загружаем символ
    cmp al, ' '           ; это пробел?
    jne store_char
    mov al, '_'           ; заменяем на подчеркивание
store_char:
    stosb                 ; сохраняем
    loop replace_loop
    
    mov byte ptr [edi], 0 ; завершающий ноль
    
    ; Выводим результат
    push offset resultBuf
    call lstrlenA@4
    push 0
    push offset lens
    push eax
    push offset resultBuf
    push dout
    call WriteConsoleA@20



    ; Небольшая задержка
    mov ecx, 03FFFFFFFH
L1: loop L1

    ; Выход из программы
    push 0
    call ExitProcess@4

main endp

end main
