    ; ============================================================
    ; СПОСОБ 2: Более простой реверс (для сравнения)
    ; ============================================================
        
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
    reverseMsg db 13,10,"Реверс строки: ",13,10,0
    pauseMsg db 13,10,"Нажмите Enter для выхода...",0
    
    din dd ?
    dout dd ?
    buf db 200 dup (?)      ; исходный буфер
    reverseBuf db 200 dup (?) ; буфер для реверсированной строки
    lens dd ?
    strLen dd ?             ; сохраняем длину строки
    tempBuf db 16 dup(?)    ; буфер для временных данных
    newline db 13,10,0
    bytesWritten dd ?       ; для WriteConsole

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
    push offset bytesWritten
    push eax
    push offset strn
    push dout
    call WriteConsoleA@20

    ; Ввод строки
    push 0
    push offset bytesWritten
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
    mov strLen, ecx     ; сохраняем длину строки

    ; Выводим сообщение о реверсе
    push offset reverseMsg
    push offset reverseMsg
    call CharToOemA@8
    
    push offset reverseMsg
    call lstrlenA@4
    push 0
    push offset bytesWritten
    push eax
    push offset reverseMsg
    push dout
    call WriteConsoleA@20
    
    ; ============================================================
    ; РЕВЕРС СТРОКИ (простой способ)
    ; ============================================================
    
    ; Проверяем, что строка не пустая
    cmp strLen, 0
    je skip_reverse
    
    ; Устанавливаем указатели
    lea esi, buf          ; ESI = начало исходной строки
    lea edi, reverseBuf   ; EDI = начало буфера результата
    
    ; Перемещаем ESI в конец строки
    mov eax, strLen
    add esi, eax
    dec esi               ; ESI указывает на последний символ
    
    mov ecx, strLen       ; счетчик = длина строки
    
reverse_loop:
    mov al, [esi]         ; берем символ с конца
    mov [edi], al         ; сохраняем в буфер
    dec esi               ; перемещаемся назад
    inc edi               ; перемещаемся вперед
    loop reverse_loop
    
    mov byte ptr [edi], 0 ; завершающий ноль
    
skip_reverse:
    ; Выводим реверсированную строку
    push offset reverseBuf
    call lstrlenA@4       ; получаем длину реверсированной строки
    
    push 0
    push offset bytesWritten
    push eax
    push offset reverseBuf
    push dout
    call WriteConsoleA@20
    
    ; Выводим перевод строки
    push offset newline
    call lstrlenA@4
    push 0
    push offset bytesWritten
    push eax
    push offset newline
    push dout
    call WriteConsoleA@20

    ; Пауза перед выходом
    push offset pauseMsg
    push offset pauseMsg
    call CharToOemA@8
    
    push offset pauseMsg
    call lstrlenA@4
    push 0
    push offset bytesWritten
    push eax
    push offset pauseMsg
    push dout
    call WriteConsoleA@20
    
    ; Ожидаем нажатия Enter
    push 0
    push offset bytesWritten
    push 1
    push offset tempBuf
    push din
    call ReadConsoleA@20

    ; Выход из программы
    push 0
    call ExitProcess@4

main endp

end main
