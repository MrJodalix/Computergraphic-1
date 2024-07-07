# Quelldateien
SRCS             = main.c io.c logic.c scene.c levels.c stringOutput.c snake.c

# ausfuehrbares Ziel
TARGET           = ueb02

# Objektdateien
OBJS             = $(SRCS:.c=.o)

# Compiler
CC               = gcc

# Linker Flags
CFLAGS  = -Wall -Wextra -Wno-unused-parameter -Werror

# Linker
LD               = gcc

# Linker libraries
LDLIBS    	 = -lm  -lglut -lGLU -lGL 

.SUFFIXES: .o .c
.PHONY: all clean

# TARGETS
all: $(TARGET)

# Linken des ausfuehrbaren Programms
$(TARGET): $(OBJS)
	$(LD) $(OBJS) $(LDLIBS) -o $(TARGET)

# Kompilieren der Objektdateien
%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $*.o $*.c

# einfaches Aufraeumen
clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
	rm -f *~


