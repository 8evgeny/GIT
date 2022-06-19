# Настройка ПО «Конфигуратор» для Windows. 

1. Установите Qt Creator, для этого скачайте установочный файл с https://www.qt.io/, укажите при установке MinGW (набор инструментов разработки программного обеспечения для создания приложений под Windows).

2. Установите boost для Windows, для этого выполните следующие шаги, которые описание по следующей ссылке:
	https://www.boost.org/doc/libs/1_55_0/more/getting_started/windows.html

3. Установите cmake для Windows, для этого выполните следующие шаги, которые описание по следующей ссылке:
	https://cmake.org/install/

4. Соберите qjon, для этого выполните следующие шаги, которые описание по следующей ссылке:
	http://qjson.sourceforge.net/build/

В файле configurator.pro, необходимо изменить пути для boost и qjson. Пример конфигурации библиотек под Windows ниже:

~~~~
win32 {
    INCLUDEPATH += C:/boost/include/boost-1_55
    LIBS += -LC:/boost/lib \
            -llibboost_filesystem-mgw73-mt-1_55 \
            -llibboost_system-mgw73-mt-1_55 \
            -llibboost_serialization-mgw73-mt-1_55 \
            -llibboost_iostreams-mgw73-mt-1_55 \
            -LC:/Users/asmjit_vm/build-qjson-Desktop_Qt_5_12_5_MinGW_64_bit-u041eu0442u043bu0430u0434u043au0430/bin \
            -llibqjson-qt5d
}
~~~~



