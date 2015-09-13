TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

PRECOMPILED_HEADER = $$HEADERS
QMAKE_CXXFLAGS += -std=c++1z
LIBS += -lboost_unit_test_framework

HEADERS += \
    CSP.hpp \
    test.hpp \
    constraint.hpp
