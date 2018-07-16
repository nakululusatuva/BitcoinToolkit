CC                  := gcc
CFLAGS              := -g -O3 -Wall
TARGET              := btctool
DIR_BUILD           := ./build
DIR_BUILD_cJSON     := ./build/cJSON
DIR_BUILD_CODEC     := ./build/codec
DIR_BUILD_CONTAINER := ./build/container
DIR_SRC             := ./src
DIR_SRC_cJSON       := ./src/cJSON
DIR_SRC_CODEC       := ./src/codec
DIR_SRC_CONTAINER   := ./src/container
DIR_LIB             := ./lib

SRC             := $(wildcard ${DIR_SRC}/*.c)
SRC_cJSON       := $(wildcard ${DIR_SRC_cJSON}/*.c)
SRC_CODEC       := $(wildcard ${DIR_SRC_CODEC}/*.c)
SRC_CONTAINER   := $(wildcard ${DIR_SRC_CONTAINER}/*.c)
BUILD           := $(patsubst %.c,${DIR_BUILD}/%.o, $(notdir ${SRC}))
BUILD_cJSON     := $(patsubst %.c,${DIR_BUILD_cJSON}/%.o, $(notdir ${SRC_cJSON}))
BUILD_CODEC     := $(patsubst %.c,${DIR_BUILD_CODEC}/%.o, $(notdir ${SRC_CODEC}))
BUILD_CONTAINER := $(patsubst %.c,${DIR_BUILD_CONTAINER}/%.o, $(notdir ${SRC_CONTAINER}))

$(TARGET) : $(BUILD) $(BUILD_cJSON) $(BUILD_CODEC) $(BUILD_CONTAINER)
	$(CC) $^ -o $@ -lcrypto $(CFLAGS)

${DIR_BUILD}/%.o : ${DIR_SRC}/%.c
	$(CC) -c $^ -o $@ $(CFLAGS)

${DIR_BUILD_cJSON}/%.o : ${DIR_SRC_cJSON}/%.c
	$(CC) -c $^ -o $@ $(CFLAGS)

${DIR_BUILD_CODEC}/%.o : ${DIR_SRC_CODEC}/%.c
	$(CC) -c $^ -o $@ $(CFLAGS)

${DIR_BUILD_CONTAINER}/%.o : ${DIR_SRC_CONTAINER}/%.c
	$(CC) -c $^ -o $@ $(CFLAGS)

clean:
	rm ${DIR_BUILD}/*.o
	rm ${DIR_BUILD_cJSON}/*.o
	rm ${DIR_BUILD_CODEC}/*.o
	rm ${DIR_BUILD_CONTAINER}/*.o
	rm btctool