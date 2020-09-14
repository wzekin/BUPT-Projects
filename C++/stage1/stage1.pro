# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

#TARGET = server

QT = core qml

CONFIG += c++17
msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
#LIBS += -lgrpc++ -lgrpc -lprotobuf -lpq -lpqxx -lgrpc++_reflection

HEADERS = \
    lib/category.h \
    lib/monster.h \
    lib/monster_factory.h \
    lib/socket.h \
    lib/type.h

SOURCES = \
    lib/blastoise.cc \
    lib/category.cc \
    lib/charizard.cc \
    lib/golduck.cc \
    lib/monster.cc \
    lib/ninetales.cc \
    lib/pidgeot.cc \
    lib/venusaur.cc \
    lib/victreebel.cc \
    lib/xatu.cc \
    test.cpp
