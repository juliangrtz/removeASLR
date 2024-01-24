#
# In order to execute this Makefile just type "make"
#

OBJS	= removePIE.o xnu-definitions.o
SOURCE	= removePIE.c xnu-definitions.c
HEADER	= xnu-definitions.h
OUT	= removePIE
CC	 = gcc
FLAGS	 = -g -c -Wall
# -g option enables debugging mode 
# -c flag generates object code for separate files


all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)


# create/compile the individual files >>separately<<
removePIE.o: removePIE.c
	$(CC) $(FLAGS) removePIE.c

xnu-definitions.o: xnu-definitions.c
	$(CC) $(FLAGS) xnu-definitions.c


# clean house
clean:
	rm -f $(OBJS) $(OUT)