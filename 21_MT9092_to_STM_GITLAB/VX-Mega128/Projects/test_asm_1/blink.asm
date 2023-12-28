; Светодиодная мигалка на микроконтроллере ATmega8
; https://ph0en1x.net

;.include "m128def.inc"

;.INCLUDEPATH "/usr/share/avra/" ; путь для подгрузки INC файлов
.INCLUDE "/usr/share/avra/m128def.inc"            ; загрузка предопределений для ATmega128
.LIST                           ; включить генерацию листинга

.CSEG                           ; начало сегмента кода
.ORG 0x0000                     ; начальное значение для адресации

; -- инициализация стека --
LDI R16, Low(RAMEND)  ; младший байт конечного адреса ОЗУ в R16
OUT SPL, R16          ; установка младшего байта указателя стека
LDI R16, High(RAMEND) ; старший байт конечного адреса ОЗУ в R16
OUT SPH, R16          ; установка старшего байта указателя стека

.equ Delay = 0xC0     ; установка константы времени задержки

; -- устанавливаем каналы PD0 и PD1 порта PORTD (PD) на вывод --
LDI R16, 0b00000110   ; поместим в регистр R16 число 6 (0x6)
OUT DDRD, R16         ; загрузим значение из регистра R16 в порт DDRD

; -- основной цикл программы --
Start:
    SBI PORTD, PORTD1 ; подача на пин с каналом PD1 высокого уровня
    CBI PORTD, PORTD2 ; подача на пин сканалом PD2 низкого уровня
    RCALL Wait        ; вызываем подпрограмму задержки по времени
    SBI PORTD, PORTD2 ; подача на пин с каналом PD1 высокого уровня
    CBI PORTD, PORTD1
    RCALL Wait
    RJMP Start        ; возврат к метке Start, повторяем все в цикле

; -- подпрограмма задержки по времени --
Wait:
    LDI  R17, Delay   ; загрузка константы для задержки в регистр R17
WLoop0:  
    LDI  R18, 0xFF      ; загружаем число 50 (0x32) в регистр R18
WLoop1:  
    LDI  R19, 0xFF    ; загружаем число 200 (0xC8, $C8) в регистр R19
WLoop2:  
    DEC  R19          ; уменьшаем значение в регистре R19 на 1
    BRNE WLoop2       ; возврат к WLoop2 если значение в R19 не равно 0 
    DEC  R18          ; уменьшаем значение в регистре R18 на 1
    BRNE WLoop1       ; возврат к WLoop1 если значение в R18 не равно 0
    DEC  R17          ; уменьшаем значение в регистре R17 на 1
    BRNE WLoop0       ; возврат к WLoop0 если значение в R17 не равно 0
RET                   ; возврат из подпрограммы Wait

Program_name: .DB "Simple LEDs blinking program"
