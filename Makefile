CC              := gcc
CFLAGS          := -g -O3 -Wall
TARGET          := btctool
DIR_BUILD       := ./build
DIR_BUILD_cJSON := ./build/cJSON
DIR_BUILD_CODEC := ./build/codec
DIR_SRC         := ./src
DIR_SRC_cJSON   := ./src/cJSON
DIR_SRC_CODEC   := ./src/codec
DIR_LIB         := ./lib

SRC         := $(wildcard ${DIR_SRC}/*.c)
SRC_cJSON   := $(wildcard ${DIR_SRC_cJSON}/*.c)
SRC_CODEC   := $(wildcard ${DIR_SRC_CODEC}/*.c)
BUILD       := $(patsubst %.c,${DIR_BUILD}/%.o, $(notdir ${SRC}))
BUILD_cJSON := $(patsubst %.c,${DIR_BUILD_cJSON}/%.o, $(notdir ${SRC_cJSON}))
BUILD_CODEC := $(patsubst %.c,${DIR_BUILD_CODEC}/%.o, $(notdir ${SRC_CODEC}))

$(TARGET) : $(BUILD) $(BUILD_cJSON) $(BUILD_CODEC)
	$(CC) $^ -o $@ -lcrypto $(CFLAGS)

${DIR_BUILD}/%.o : ${DIR_SRC}/%.c
	$(CC) -c $^ -o $@ $(CFLAGS)

${DIR_BUILD_cJSON}/%.o : ${DIR_SRC_cJSON}/%.c
	$(CC) -c $^ -o $@ $(CFLAGS)

${DIR_BUILD_CODEC}/%.o : ${DIR_BUILD_CODEC}/%.c
	$(CC) -c $^ -o $@ $(CFLAGS)

clean:
	rm ${DIR_BUILD}/*.o
	rm ${DIR_BUILD_cJSON}/*.o
	rm ${DIR_BUILD_CODEC}/*.o
	rm btctool