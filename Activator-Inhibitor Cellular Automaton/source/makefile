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

all: exp3 aica

exp3: exp3.o
	$(CC) $(FLAGS) -o exp3 exp3.o

exp3.o: exp3.cpp
	$(CC) $(FLAGS) -c exp3.cpp

aica: aica.o
	$(CC) $(FLAGS) -o aica aica.o
	$(RM) *.o

aica.o: aica.cpp
	$(CC) $(FLAGS) -c aica.cpp

clean:
	$(RM) *.o exp3 aica

clean-data:
	$(RM) csv/* images/*
