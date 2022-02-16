Backend для display 

На сервере бинарник (изменить владельца))скопировать в папку /root/display_backend/build
файлы devices_all  и  git-info.txt  скопировать в папку /usr/bin
создать сервис display.servise и в нем указать путь к бинарнику
в файл /root/display_backend/logfile  пишется лог в виде

16.02.2022 11:58:38 device 192.168.000.110 unavailable
16.02.2022 11:59:23 device 192.168.000.110 unavailable
16.02.2022 11:59:23 device 192.168.000.113 unavailable
16.02.2022 11:59:23 device 192.168.000.111 unavailable
16.02.2022 11:59:30 device 192.168.000.110 unavailable
16.02.2022 11:59:30 device 192.168.000.113 unavailable
16.02.2022 11:59:30 device 192.168.000.111 unavailable
16.02.2022 12:00:49 device 192.168.000.113 unavailable
16.02.2022 12:00:56 device 192.168.000.113 unavailable
