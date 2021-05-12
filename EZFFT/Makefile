# 目标
TARGET :=out
#build 目录
BUILD_DIR=build

CC := gcc

SZ := size
# 头文件目录
INCDIRS:= \
test \
# 源文件目录
SRCDIRS:=\
./

#全局弘定义 
DEFS :=\

INCLUDE:= $(patsubst %, -I%, $(INCDIRS))

CFILES:=\
test.c \
ezfft.c
# 库
LIBS:=\
-lm \
# -lpthread

# 标志
CFLAGS:=\
-g \
-DDEBUG_ON_PC \

# -fno-stack-protector

OBJECTS:=$(addprefix $(BUILD_DIR)/,$(notdir $(CFILES:.c=.o)))

.PHONLYS:clean
.PHONLYS:all


VPATH:=$(SRCDIRS)

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) $(DEFS) -c  $(CFLAGS) $<  $(INCLUDE) -o $@

$(BUILD_DIR)/$(TARGET):$(OBJECTS)
	$(CC) $(OBJECTS) $(DEFS) $(CFLAGS) -o $@ $(LIBS)

$(BUILD_DIR):
	mkdir $@

clean:
	rm -rf $(BUILD_DIR)



