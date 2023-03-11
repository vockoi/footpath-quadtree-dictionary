CC = gcc
CFLAGS = -Wall -g
LDLIBS = 

SRC = dict4.c data.c point_region_data.c data_dictionary.c data_index.c quadtree.c
OBJ = $(SRC:.c=.o)

EXE = dict4

dict4: dict4.o data.o point_region_data.o data_dictionary.o data_index.o quadtree.o
	gcc -o dict4 dict4.o data.o point_region_data.o data_dictionary.o data_index.o quadtree.o

dict4.o: dict4.c

data.o: data.c data.h

point_region_data.o: point_region_data.c point_region_data.h

data_dictionary.o: data_dictionary.c data_dictionary.h

data_index.o: data_index.c data_index.h

quadtree.o: quadtree.c quadtree.h

clean:
	rm -f *.o dict3 dict4
