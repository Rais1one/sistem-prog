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
din dd ?
dout dd ?
buf db 200 dup (?)
lens dd ?


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
    Call WriteConsoleA@20

    ; Ввод строки
    push 0
    push offset lens
    push 200
    push offset buf
    push din
    Call ReadConsoleA@20

    ; Удаление пробелов из введенной строки
    LEA ESI, BUF           
    LEA EDI, BUF           
    MOV ECX, LENS

    remove_spaces:
    LODSB                  ; Загружаем следующий байт из BUF в AL
    CMP AL, 0              ; Проверяем конец строки
    JE done                ; Если конец строки, переходим к завершению
    CMP AL, ' '            ; Проверяем, является ли символ пробелом
    JE remove_spaces       ; Если пробел, переходим к следующему символу
    STOSB                  ; Если не пробел, сохраняем символ в BUF
    LOOP remove_spaces

    done:
; Обновляем длину строки
    SUB EDI, OFFSET BUF    ; Получаем длину выходной строки
    MOV LENS, EDI 

    ; Вывод полученной строки
    push 0
    push offset lens
    push lens
    push offset buf
    push dout
    Call WriteConsoleA@20
 

    ; Небольшая задержка
    mov ecx, 03FFFFFFFH
L1: loop L1

    ; Выход из программы
    push 0
    call ExitProcess@4
main endp

end main
