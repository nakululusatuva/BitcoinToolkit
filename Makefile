
CC 			= gcc
TARGET 		= btctool
DIR_HEADERS = ./include
DIR_BUILD 	= ./build
DIR_SRC 	= ./src

SRC 	= $(wildcard ${DIR_SRC}/*.c)
BUILD 	= $(patsubst %.c,${DIR_BUILD}/%.o,$(notdir ${SRC}))

$(TARGET) : $(BUILD)
	$(CC) $^ -o $@ -lssl -lcrypto -Wall

${DIR_BUILD}/%.o : ${DIR_SRC}/%.c
	$(CC) -c $^ -I ${DIR_HEADERS} -o $@ -Wall

clean:
	@rm ${DIR_BUILD}/*.o
	@rm btctool