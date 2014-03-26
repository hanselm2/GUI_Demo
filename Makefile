CC = g++
CFLAGS = -Wall
PROG = gui_demo


SRCS = main.cpp Vector3f.h Vector3f.cpp Object.h Button.h Button.cpp Menu.h Menu.cpp Enviroment.h Enviroment.cpp testenviro.h

ifeq ($(shell uname), Darwin) 
	LIBS = -framework OpenGL -framework GLUT 
else
	LIBS = -lglut -lGL -lGLU
endif

all: $(PROG) 

$(PROG): $(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
