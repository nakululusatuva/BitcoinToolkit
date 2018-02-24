
CC 			= gcc
TARGET 		= btctool
DIR_HEADERS = ./include
DIR_BUILD 	= ./build
DIR_SRC 	= ./src

SRC 	= $(wildcard ${DIR_SRC}/*.c)
BUILD 	= $(patsubst %.c,${DIR_BUILD}/%.o,$(notdir ${SRC})) ${DIR_BUILD}/std.o

$(TARGET) : $(BUILD)
	$(CC) $^ -o $@ -lssl -lcrypto

${DIR_BUILD}/%.o : ${DIR_SRC}/%.c
	$(CC) -c $< -I ${DIR_HEADERS} -o $@

${DIR_BUILD}/std.o : ${DIR_SRC}/std/*.c
	$(CC) -c $^ -I ${DIR_HEADERS} -o $@

clean:
	@rm ${DIR_BUILD}/*.o
	@rm btctool