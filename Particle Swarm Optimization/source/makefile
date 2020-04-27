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

all: pso generateData

pso: pso.o
	$(CC) $(FLAGS) -o pso pso.o
	$(RM) *.o

pso.o: pso.cpp
	$(CC) $(FLAGS) -c pso.cpp

generateData: generateData.o
	$(CC) $(FLAGS) -o generateData generateData.o
	$(RM) *.o

generateData.o: generateData.cpp
	$(CC) $(FLAGS) -c generateData.cpp

clean:
	$(RM) *.o pso generateData

clean-data:
	$(RM) ../data/*
