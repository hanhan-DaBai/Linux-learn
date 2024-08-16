#ith

#cc=gcc
#TARGET=test
#OBJ=main.o printf.o add.o

#$(TARGET):$(OBJ)
#	$(CC) -o $(TARGET) $(OBJ)
#
#main.o:main.c
#	$(CC) -c main.c
#
#printf.o:printf.c
#	$(CC) -c printf.c
#
#add.o:add.c
#	$(CC) -c add.c

#2th
cc=gcc
TARGET=test
OBJ=main.o printf.o add.o
CCFLAGS = -c -wall 
$(TARGET):$(OBJ)
	$(CC) -o $@ $^

*.o:*.c
	$(CC) $(CCDLAGS) $^ -o $@

.PHONY:clean

clean:
	rm -f *.o $(TARGET)

#3th

#CC=gcc
#TARGET = test
#SRC= $(wildcard *.c)
#OBJ= $(patsubst %.c,%.o,$(SRC))
#
#CCFLAGS = -c -wall
#
#$(TARGET):$(OBJ)
#	$(CC) -o $@ $^
#
#%*.o:%*.c
#	$(CC) $(CCFLAGS) $^ -o $@
#
#.PHONY:clean
#
#clean:
#	rm -f *.o $(TARGET)

