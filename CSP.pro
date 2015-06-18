TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    CSP.hpp \
    test.hpp \
    constraint.hpp

PRECOMPILED_HEADER = $$HEADERS
QMAKE_CXXFLAGS += -std=c++1z
LIBS += -lboost_unit_test_framework

