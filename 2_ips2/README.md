Это копия всего репозитория GIT ips2
Исходники ips_sc4_pdo изменены, чтобы сборка прошивки шла в Linux 
28.01.2022 - прошивка собирается но в устройстве не работает

Сборка прошивки ведется в папке на уровень выше 2_ips2_CMake
Сборка конфигуратора в Linux - в папке configurator_ets (дома не собирается - только на работе - из дома нет доступа к artifactory)

promwad2 - проект прошивки от белоруссов - собирается в qbs !!!

# Configurator

# How to build Projects
* Install cmake
* Install python3
* Install conan
'''
pip install conan
'''

* Add Artifactory 
'''
    conan remote add artifactory http://conan.git-holding.ru:8082/artifactory/api/conan/conan-local
    conan user -p develop1 -r artifactory develop
'''

# How to build
## Dependencies
* qt 5.15.2
* cmake 3.16

## Windows 10
### MSVC 2019 x64
### Visual Studio 2019
'''
    python build.py -d msvc
    python build.py -r msvc
'''
## Linux Xubuntu 19.10
### gcc 9.2.1 x64
'''
    python build.py -d gcc
    python build.py -r gcc
'''
