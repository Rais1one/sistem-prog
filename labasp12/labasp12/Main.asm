.386
.model flat, stdcall

extern GetStdHandle@4:PROC
extern WriteConsoleA@20:PROC
extern ReadConsoleA@20:PROC
extern ExitProcess@4:PROC
extern lstrlenA@4:PROC
extern CharToOemA@8:PROC

.data
    prompt db 'Vvedite stroku: ',0
    charPrompt db 13,10,'Vvedite simvol dlya poiska: ',0
    resultMsg db 13,10,'Simvol "',0
    resultMsg2 db '" naiden ',0
    resultMsg3 db ' raz(a)',0
    notFoundMsg db 13,10,'Simvol ne naiden',0
    
    inputBuf db 256 dup(?)      ; буфер для строки
    charBuf db 16 dup(?)         ; буфер для символа
    outputBuf db 16 dup(?)       ; буфер для числа
    bytesWritten dd ?
    charCount dd ?
    hConsole dd ?
    hInput dd ?
    temp dd ?
    searchChar db 0
    crlf db 13,10,0
    pauseMsg db 13,10,'Press Enter to exit...',0

.code
; -------------------------------------------------------------
; Преобразование числа в строку
; Вход: EAX = число, EDI = буфер для строки
; Выход: EDI указывает на конец строки
; -------------------------------------------------------------
itoa proc
    push eax
    push ebx
    push ecx
    push edx
    
    mov ebx, 10
    xor ecx, ecx
    
    test eax, eax
    jnz convert
    mov byte ptr [edi], '0'
    inc edi
    jmp done
    
convert:
    xor edx, edx
    div ebx
    push edx
    inc ecx
    test eax, eax
    jnz convert
    
write_digits:
    pop eax
    add al, '0'
    stosb
    loop write_digits
    
done:
    mov byte ptr [edi], 0
    pop edx
    pop ecx
    pop ebx
    pop eax
    ret
itoa endp

; -------------------------------------------------------------
; Функция подсчета вхождений символа в строке
; Вход: ESI = указатель на строку, AL = искомый символ
; Выход: EAX = количество вхождений
; -------------------------------------------------------------
count_char proc
    push ebx
    push ecx
    push edx
    push esi
    
    xor ecx, ecx        ; счетчик
    mov bl, al          ; сохраняем искомый символ
    mov edx, esi        ; сохраняем указатель
    
count_loop:
    mov al, [edx]
    test al, al
    jz count_done
    cmp al, 13
    je count_done
    cmp al, 10
    je count_done
    cmp al, bl
    jne next_char
    inc ecx
next_char:
    inc edx
    jmp count_loop
    
count_done:
    mov eax, ecx
    
    pop esi
    pop edx
    pop ecx
    pop ebx
    ret
count_char endp

main proc
    ; Получаем дескриптор консоли для вывода
    push -11
    call GetStdHandle@4
    mov hConsole, eax
    
    ; Получаем дескриптор консоли для ввода
    push -10
    call GetStdHandle@4
    mov hInput, eax

    ; ============================================================
    ; ВВОД СТРОКИ
    ; ============================================================

    ; Преобразуем приглашение в OEM для вывода
    push offset prompt
    push offset prompt
    call CharToOemA@8
    
    ; Вывод приглашения для ввода строки
    push offset prompt
    call lstrlenA@4
    push 0
    push offset bytesWritten
    push eax
    push offset prompt
    push hConsole
    call WriteConsoleA@20
    
    ; Ввод строки
    push 0
    push offset bytesWritten
    push 256
    push offset inputBuf
    push hInput
    call ReadConsoleA@20
    
    ; Убираем символы перевода строки (CR+LF)
    mov esi, offset inputBuf
    xor ecx, ecx
    
clean_input:
    mov al, [esi]
    cmp al, 13
    je clean_input_done
    cmp al, 10
    je clean_input_done
    test al, al
    jz clean_input_done
    inc esi
    inc ecx
    jmp clean_input
    
clean_input_done:
    mov byte ptr [esi], 0

    ; ============================================================
    ; ВВОД СИМВОЛА ДЛЯ ПОИСКА
    ; ============================================================
    ; Преобразуем приглашение для ввода символа
    push offset charPrompt
    push offset charPrompt
    call CharToOemA@8
    
    ; Вывод приглашения для ввода символа
    push offset charPrompt
    call lstrlenA@4
    push 0
    push offset bytesWritten
    push eax
    push offset charPrompt
    push hConsole
    call WriteConsoleA@20
    
    ; Ввод символа
    push 0
    push offset bytesWritten
    push 16
    push offset charBuf
    push hInput
    call ReadConsoleA@20
    
    ; Берем первый символ из введенной строки
    mov al, [charBuf]
    mov searchChar, al
    
    ; ============================================================
    ; ПОДСЧЕТ ВХОЖДЕНИЙ СИМВОЛА
    ; ============================================================
    mov esi, offset inputBuf
    mov al, searchChar
    call count_char
    mov charCount, eax
    
    ; ============================================================
    ; ВЫВОД РЕЗУЛЬТАТА
    ; ============================================================
    cmp charCount, 0
    je not_found
    
    ; Преобразуем сообщение о результате в OEM
    push offset resultMsg
    push offset resultMsg
    call CharToOemA@8
    
    ; Вывод "Simvol '"
    push offset resultMsg
    call lstrlenA@4
    push 0
    push offset bytesWritten
    push eax
    push offset resultMsg
    push hConsole
    call WriteConsoleA@20
    
    ; Вывод искомого символа
    mov al, searchChar
    mov [outputBuf], al
    mov byte ptr [outputBuf+1], 0
    push offset outputBuf
    call lstrlenA@4
    push 0
    push offset bytesWritten
    push eax
    push offset outputBuf
    push hConsole
    call WriteConsoleA@20
    
    ; Преобразуем resultMsg2
    push offset resultMsg2
    push offset resultMsg2
    call CharToOemA@8
    
    ; Вывод "' naiden "
    push offset resultMsg2
    call lstrlenA@4
    push 0
    push offset bytesWritten
    push eax
    push offset resultMsg2
    push hConsole
    call WriteConsoleA@20
    
    ; Формирование числа
    lea edi, outputBuf
    mov eax, charCount
    call itoa
    
    ; Вывод числа
    push offset outputBuf
    call lstrlenA@4
    push 0
    push offset bytesWritten
    push eax
    push offset outputBuf
    push hConsole
    call WriteConsoleA@20
    
    ; Преобразуем resultMsg3
    push offset resultMsg3
    push offset resultMsg3
    call CharToOemA@8
    
    ; Вывод " raz(a)"
    push offset resultMsg3
    call lstrlenA@4
    push 0
    push offset bytesWritten
    push eax
    push offset resultMsg3
    push hConsole
    call WriteConsoleA@20
    
    jmp show_pause
    
not_found:
    ; Преобразуем сообщение " ne naiden "
    push offset notFoundMsg
    push offset notFoundMsg
    call CharToOemA@8
    
    ; Вывод "Simvol ne naiden"
    push offset notFoundMsg
    call lstrlenA@4
    push 0
    push offset bytesWritten
    push eax
    push offset notFoundMsg
    push hConsole
    call WriteConsoleA@20
    
show_pause:
    ; Вывод перевода строки
    push offset crlf
    call lstrlenA@4
    push 0
    push offset bytesWritten
    push eax
    push offset crlf
    push hConsole
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
    push hConsole
    call WriteConsoleA@20
    
    push 0
    push offset bytesWritten
    push 1
    push offset temp
    push hInput
    call ReadConsoleA@20
    
    ; Завершение
    push 0
    call ExitProcess@4

main endp 
end main