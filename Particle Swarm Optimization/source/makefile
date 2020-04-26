# Specify how to compile

CC = g++
FLAGS = -Wall -Werror -g
#INCLUDES = 
#LIBS = 


# define the source, object, and executable files

#SRCS =
#OBJS = 
#EXECUTABLES =


# generic compilation

all: pso

pso: pso.o
	$(CC) $(FLAGS) -o pso pso.o
	$(RM) *.o

pso.o: pso.cpp
	$(CC) $(FLAGS) -c pso.cpp

clean:
	$(RM) *.o pso

clean-data:
	$(RM) ../data/*
