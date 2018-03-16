CC = g++  
WARNING=  -Wall
CFLAGS += $(WARNING) -g  -std=c++11 -Os
LDFLAGS += -lm -pthread
PROC_NAME = proc

all: clean *.o ${PROC_NAME}

Utils.o : Utils.h  
	${CC} ${CFLAGS} -c Utils.cpp
RS.o : RS.h  
	${CC} ${CFLAGS} -c RS.cpp
${PROC_NAME} : Utils.o RS.o Main.o  
	${CC} ${CFLAGS} Utils.o RS.o Main.o ${LDFLAGS} -o ${PROC_NAME}   
clean:  
	rm -rf *.o  
	rm -rf ${PROC_NAME} 