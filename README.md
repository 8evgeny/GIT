# GIT

1_display         - backend для экрана 
                    Сборка в KeilProjects  
                    Сборка в Linux и в репозитории Arm - milandr_display_cmake

FIRMWARE          - прошивки MILANDR через UART
test_ports_IO     - тест MILANDR порты
test_USB          - тест MILANDR USB

2_ips2            - Тут все - ответвиля от осн репозитлория в начале года Внутри 2 проекта - Белорусский на qbs и Муромский на Cube
                    Далее проекты по папкам

2_ips2_CMake      - К Муромскому прикрутил CMake - закрываю направление


2_ips2_new          Это модификация прошивки для пульта с 3 рычагами - оранжевый
                    Сборка в Win модифицированной прошивки в Кубе
                    Правка кода в CMake в Linux


ips_sc4_pdo_commit_28_01_2022  коммит Мурома - собирается и в Win и в Linux  - основа для дальнейшей правки
