# Makefile for quick test

CPP = g++

CPP_FLAGS = -Wall -g

QT_BASE_INCL_PATH = /usr/include/qt4

QT_GUI_INCL_PATH = $(QT_BASE_INCL_PATH)/QtGui

QT_CORE_INCL_PATH = $(QT_BASE_INCL_PATH)/QtCore

QT_OPENGL_INCL_PATH = $(QT_BASE_INCL_PATH)/QtOpenGL

QT_INCL_PATHS = -I$(QT_BASE_INCL_PATH) \
                -I$(QT_GUI_INCL_PATH) -I$(QT_CORE_INCL_PATH) \
                -I$(QT_OPENGL_INCL_PATH)

QT_LIBS = QtCore QtGui QtOpenGL

QT_LIBS_ALL = $(addprefix -l, $(QT_LIBS))

TARGETS = main_1

info:
	@echo "Some simple demo stuff for OpenGL on Qt4"
	@ echo "Targets are:"
	@ echo "$(TARGETS)"

clean:
	- rm *.o

cleanall: clean
	- rm $(TARGETS)

main_1 : main_1.o CGLWin.o
	$(CPP) $(CPP_FLAGS) -o $@ $^ \
   $(QT_LIBS_ALL) \
   -lGL

main_1.o : main_1.cpp
	$(CPP) $(CPP_FLAGS) $(QT_INCL_PATHS) -c $<

CGLWin.o : CGLWin.cpp CGLWin.hpp
	$(CPP) $(CPP_FLAGS) $(QT_INCL_PATHS) -c $<


