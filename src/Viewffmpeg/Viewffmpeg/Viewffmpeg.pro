TEMPLATE = app
CONFIG += console c++11
SOURCES += main.cpp

INCLUDEPATH += $$PWD/../../../include

#区分32位和64位windows程序
opt = $$find(QMAKESPEC,"msvc2017_64")
isEmpty(opt){
message("win32 lib")
LIBS += -L$$PWD/../../../lib/win32
}
!isEmpty(opt){
message("win64 lib")
LIBS += -L$$PWD/../../../lib/win64
}
#message($$QMAKESPEC)
