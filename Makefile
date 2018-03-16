CC = g++  
CFLAGS = -Wall -g  
LDFLAGS = -lm  
PROC_NAME = proc

all: clean *.o ${PROC_NAME}

RS.o : RS.h  
	${CC} ${CFLAGS} -c RS.cpp
${PROC_NAME} : RS.o Main.o  
	${CC} ${CFLAGS} RS.o Main.o ${LDFLAGS} -o ${PROC_NAME}   
clean:  
	rm -rf *.o  
	rm -rf ${PROC_NAME} 