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

all: geneticAlgorithm

geneticAlgorithm: geneticAlgorithm.o
	$(CC) $(FLAGS) -o geneticAlgorithm geneticAlgorithm.o
	$(RM) *.o

hopfieldNet.o: geneticAlgorithm.cpp
	$(CC) $(FLAGS) -c geneticAlgorithm.cpp

clean:
	$(RM) *.o geneticAlgorithm

clean-data:
	$(RM) ../data/*
