;---- Определяем целевое устройство
        .device atmega16
;---- Сегмент данных
        .dseg
;---- Сегмент кода
        .cseg
        .org 0x0000

        ldi r16, 10
M1:
        inc r16
        rjmp M1

;---- Сегмент EEPROM
        .eseg
