#
#
# Common of MdrFwU*.pro-files
#
#
version = $$system(git describe --long)
win32: {
#  date = $$system(date /t)
#  time = $$system(time /t)
	BUILD_DATE = $$system( date /t ) $$system( time /t )
	BUILD_DATE = $$member(BUILD_DATE,0)_$$member(BUILD_DATE,1)
}
unix: {
	BUILD_DATE = $$system( date "+%Y-%m-%d_%H-%M" )
}

VERSTR = '\\"$${version}\\"'
#DATSTR = '\\"$${date}\\"'
#TIMSTR = '\\"$${time}\\"'
BD_STR = '\\"$${BUILD_DATE}\\"'


DEFINES += VERION=\"$${VERSTR}\"
#DEFINES += DAT=\"$${DATSTR}\"
#DEFINES += TIM=\"$${TIMSTR}\"
DEFINES += BUILD_DATE=\"$${BD_STR}\"

#message($$VERSTR)
##message($$DATSTR)
##message($$TIMSTR)
#message($$BD_STR)

#win32: MY_BUILD_DT='TODO_date_time'
#unix:  MY_BUILD_DT='"\\\"$(shell date \"+%F %T\")\\\""' - даёт лишние кавычки
#QMAKE_POST_LINK = cp $(TARGET) $(TARGET)_$${BUILD_DATE}

win32:VERSION = 0.3.0.0 # major.minor.patch.build
else:VERSION = 0.3.0    # major.minor.patch
DEFINES  += VER_MAJOR=0
DEFINES  += VER_MINOR=3
DEFINES  += VER_PATCH=0

#include(../../qesp/github/qextserialport/src/qextserialport.pri)


# https://coderoad.ru/6579516/Qt-Creator-%D0%BA%D0%B0%D0%BA-%D0%BE%D1%82%D0%BB%D0%B8%D1%87%D0%B8%D1%82%D1%8C-win32-%D0%BE%D1%82-win64
greaterThan(QT_MAJOR_VERSION, 4) {
    TARGET_ARCH=$${QT_ARCH}
} else {
    TARGET_ARCH=$${QMAKE_HOST.arch}
}
message(TARGET_ARCH $$TARGET_ARCH)

contains(TARGET_ARCH, x86_64) {
    ARCHITECTURE = x64
} else {
    ARCHITECTURE = x86
}
message(ARCHITECTURE $$ARCHITECTURE)
#win32 {
#win32-g++:contains(QMAKE_HOST.arch, x86_64):{
#    LIBS += ... #for win64
#} else {
#    LIBS += ... #for win32
#}
#}


