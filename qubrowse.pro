
QT          *= script webkit gui
TEMPLATE     = app
TARGET       = 
DEPENDPATH  += .
INCLUDEPATH += .
MOC_DIR      = .build
OBJECTS_DIR  = .build
LIBS        *= -ldl

# Input
HEADERS += ubrowse.h autoscroller.h gcpuprofiler.h
SOURCES += main.cc ubrowse.cc autoscroller.cc gcpuprofiler.cc
