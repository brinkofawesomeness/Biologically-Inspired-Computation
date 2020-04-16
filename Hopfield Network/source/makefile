# Specify how to compile

CC = g++
FLAGS = -Wall -Werror -g
INCLUDES = 
LIBS = 


# define the source, object, and executable files

#SRCS = aica.cpp
#OBJS = $(SRCS:.cpp=.o)
#EXECUTABLES = aica


# generic compilation

all: hopfieldNet

hopfieldNet: hopfieldNet.o
	$(CC) $(FLAGS) -o hopfieldNet hopfieldNet.o
	$(RM) *.o

hopfieldNet.o: hopfieldNet.cpp
	$(CC) $(FLAGS) -c hopfieldNet.cpp

clean:
	$(RM) *.o *.csv hopfieldNet

clean-data:
	$(RM) csv/* images/*
