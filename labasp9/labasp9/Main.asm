.386
.model flat, stdcall

; Прототипы внешних функций
extern GetStdHandle@4:PROC
extern WriteConsoleA@20:PROC
extern CharToOemA@8:PROC
extern ReadConsoleA@20:PROC
extern ExitProcess@4:PROC
extern lstrlenA@4:PROC

.data
    prompt db "Введите строку: ",13,10,0
    resultMsg db 13,10,"ASCII:",13,10,0
    newline db 13,10,0
    
    din dd ?
    dout dd ?
    inputBuf db 200 dup (?)                  ; буфер для введенной строки
    outputBuf db 2048 dup (?)              ; буфер для результирующей строки
    lens dd ?
    bytesWritten dd ?
    temp dd ?
    i dd ?

.code
main proc
    ; Перекодируем строку PROMPT
    push offset prompt
    push offset prompt
    call CharToOemA@8

    ; Получим дескриптор ввода
    push -10
    call GetStdHandle@4
    mov din, eax

    ; Получим дескриптор вывода
    push -11
    call GetStdHandle@4
    mov dout, eax

    ; Определим длину строки PROMPT
    push offset prompt
    call lstrlenA@4
    mov temp, eax

    ; Вывод строки PROMPT
    push 0
    push offset bytesWritten
    push temp
    push offset prompt
    push dout
    call WriteConsoleA@20

    ; Ввод строки
    push 0
    push offset lens
    push 200
    push offset inputBuf
    push din
    call ReadConsoleA@20

    ; Убираем символы перевода строки (CR+LF) из введенной строки
    mov esi, offset inputBuf
    xor ecx, ecx
    
find_end:
    mov al, [esi]
    cmp al, 13          ; возврат каретки
    je fix_end
    cmp al, 10          ; перевод строки
    je fix_end
    cmp al, 0           ; конец строки?
    je fix_end
    inc esi
    inc ecx
    jmp find_end
    
fix_end:
    mov byte ptr [esi], 0
    mov lens, ecx       ; сохраняем реальную длину

    ; Проверяем, не пустая ли строка
    cmp lens, 0
    je exit_program

    ; Выводим заголовок результата
    push offset resultMsg
    call lstrlenA@4
    push 0
    push offset bytesWritten
    push eax
    push offset resultMsg
    push dout
    call WriteConsoleA@20

    ; Перекодируем заголовок результата
    push offset resultMsg
    push offset resultMsg
    call CharToOemA@8

    ; Подготавливаем буфер для вывода кодов
    lea edi, outputBuf
    mov i, 0

print_loop:
    mov ecx, i
    cmp ecx, lens
    jge print_done

    ; Загружаем текущий символ
    mov esi, offset inputBuf
    add esi, ecx
    mov al, [esi]
    movzx eax, al        ; расширяем до 32-бит, EAX = код символа
    
    ; Сохраняем символ в буфер вывода
    push eax             ; сохраняем код символа
    mov al, [esi]        ; загружаем сам символ
    stosb                ; сохраняем символ
    pop eax              ; восстанавливаем код символа
    
    ; Выводим " = "
    push eax             ; снова сохраняем код
    mov al, ' '
    stosb
    mov al, '='
    stosb
    mov al, ' '
    stosb
    pop eax              ; восстанавливаем код для функции
    
    call num_to_str      ; EAX содержит код символа
    mov ax, 0A0Dh        ; перевод строки
    stosw

    inc i
    jmp print_loop

print_done:
    ; Добавляем перевод строки
    mov ax, 0A0Dh
    stosw

    ; Вычисляем длину результирующей строки
    mov eax, edi
    lea ebx, outputBuf
    sub eax, ebx

    ; Выводим результат
    push 0
    push offset bytesWritten
    push eax
    push offset outputBuf
    push dout
    call WriteConsoleA@20

    ; Выводим дополнительный перевод строки
    push offset newline
    push offset newline
    call CharToOemA@8
    
    push offset newline
    call lstrlenA@4
    push 0
    push offset bytesWritten
    push eax
    push offset newline
    push dout
    call WriteConsoleA@20

exit_program:
    ; Пауза перед выходом
    push 0
    push offset bytesWritten
    push 1
    push offset temp
    push din
    call ReadConsoleA@20

    ; Выход из программы
    push 0
    call ExitProcess@4
main endp

; ---------------------------------------------
; num_to_str - преобразует число (0-255) в строку из трёх цифр
; Вход:  eax = число, edi = буфер назначения
; Выход: edi указывает на следующий за последним символом байт
; ---------------------------------------------
num_to_str proc
    push eax
    push ebx
    push edx
    push ecx

    ; сотни
    xor edx, edx
    mov ebx, 100
    div ebx
    add al, '0'
    stosb

    ; десятки
    mov eax, edx
    xor edx, edx
    mov ebx, 10
    div ebx
    add al, '0'
    stosb

    ; единицы
    mov eax, edx
    add al, '0'
    stosb

    pop ecx
    pop edx
    pop ebx
    pop eax
    ret
num_to_str endp

end main