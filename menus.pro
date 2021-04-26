QT += widgets

HEADERS       = mainwindow.h \
    bbbitem.h \
    mob.h \
    player.h
SOURCES       = mainwindow.cpp \
                bbbitem.cpp \
                main.cpp \
                mob.cpp \
                player.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/mainwindows/menus
INSTALLS += target
