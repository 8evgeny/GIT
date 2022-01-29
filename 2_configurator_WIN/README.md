Это копия конфигуратора из репозитория GIT
Для сборки я в Win


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

### How to build
## MSVC 2017
'''
    mkdir build && cd build
    conan install .. --profile=../msvc.profile --build=missing
    cmake "-GVisual Studio 15 2017" ..
    cmake --build . --target configurator
'''
## Mingw
'''
    mkdir build && cd build
    conan install .. --profile=../mingw.profile --build=missing
    cmake "-GCodeBlocks - MinGW Makefiles" ..
    cmake --build . --target configurator
'''
