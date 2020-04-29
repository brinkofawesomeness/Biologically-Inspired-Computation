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

all: geneticAlgorithm generateData

generateData: generateData.o
	$(CC) $(FLAGS) -o generateData generateData.o
	$(RM) *.o

geneticAlgorithm: geneticAlgorithm.o
	$(CC) $(FLAGS) -o geneticAlgorithm geneticAlgorithm.o
	$(RM) *.o

generateData.o: generateData.cpp
	$(CC) $(FLAGS) -c generateData.cpp

geneticAlgorithm.o: geneticAlgorithm.cpp
	$(CC) $(FLAGS) -c geneticAlgorithm.cpp

clean:
	$(RM) *.o geneticAlgorithm generateData

clean-data:
	$(RM) ../data/*
