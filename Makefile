# NanoBrain - CLI


# nome do projeto
PROJ_NAME=nanobrain-cli

# .c files
C_SRC=$(wildcard ./src/*.cpp)

# .h files
H_SRC=$(wildcard ./src/*.h)

#Obj files
OBJ=$(subst .cpp,.o,$(subst src,obj,$(C_SRC)))

# Compilador 
CC=g++

# C Flags
FLAGS=-c		\
			-W		\
			-Wall
FOLDER=$(shell pwd)/

# Comando remove files
RM= rm -rf

# Compilação e Link
all: objFolder $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
		@ echo 'Building binary using G++ linker: $@'
		$(CC) -o $@ $^
		@ ln -s $(FOLDER)$(PROJ_NAME) /usr/local/bin/$(PROJ_NAME)
		@ ln -s $(FOLDER)/src/get_pypi /usr/local/bin/$(PROJ_NAME)_py
		@ echo 'Done!'
		@ echo ' '

./obj/%.o: ./src/%.c ./src/%.h
		@ echo 'Building target using G++ compiler: $<'
		$(CC) $< $(FLAGS) -o $@
		@ echo ' '

./obj/main.o: ./src/main.cpp $(H_SRC)
		@ echo 'Building target using G++ compiler: $<'
		$(CC) $< $(FLAGS) -o $@
		@ echo ' '
objFolder: 
		@ mkdir -p obj

clean:
		@ $(RM) ./obj/*.o $(PROJ_NAME) *~
		@ rm /usr/local/bin/$(PROJ_NAME)
		@ rm /usr/local/bin/$(PROJ_NAME)_py
		@ rmdir obj
	
.PHONY: all clean