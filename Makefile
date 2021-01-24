SRC_FILES  	 := $(shell find src -name *.cc)
SRC_FILES    := $(filter-out src/main.cc,$(SRC_FILES))
ALL_SRC_FILES = src/main.cc $(SRC_FILES)
OBJ_FILES	  = $(ALL_SRC_FILES:.cc=.o)	

KERNEL_MAIN       = _kmain
OS_IMAGE          = dudeosv2.img
OS_BIN            = dudeosv2.bin
KERNEL_START_CODE = 0x6400000

LD          = ld
CXX         = g++
CXX_FLAGS   = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector  \
			  -fno-pic -std=c++14 -fno-rtti -fno-exceptions -Wall -Wextra \
			  -Iinclude -fno-leading-underscore -Wno-write-strings
			  
LD_FLAGS    = --Ttext $(KERNEL_START_CODE) -m elf_i386 -s --oformat binary -e $(KERNEL_MAIN)

all: $(OS_IMAGE)

$(OS_IMAGE) : $(OBJ_FILES)
	$(LD) $(LD_FLAGS) $(OBJ_FILES) -o $(OS_BIN)
	make -C boot
	cat boot/boot.bin $(OS_BIN) /dev/zero | head -c 1474560 > $(OS_IMAGE)

%.o : %.cc
	$(CXX) $(CXX_FLAGS) -c $^ -o $@

.PHONY run:
run: $(OS_IMAGE)
	qemu-system-i386 $^

.PHONY clean:
clean:
	make -C boot clean
	find -name '*.o' | xargs $(RM)
	rm $(OS_IMAGE)
	rm $(OS_BIN)