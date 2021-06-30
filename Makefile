# source folder containing *.cc and *.asm files
SRC_FOLDER := src

# main.cc (the kernel main file)
MAIN_CXX := $(SRC_FOLDER)/main.cc

# get all files from the source folder with the extension
# of .cc (c++) and filter out main.cc
CXX_SRC_FILES := $(shell find src -name *.cc)
CXX_SRC_FILES := $(filter-out src/main.cc,$(CXX_SRC_FILES))
CXX_ALL_SRC_FILES = $(MAIN_CXX) $(CXX_SRC_FILES)

# get all file from the source folder with the extension
# of .asm (assembly)
ASM_SRC_FILES = $(shell find src -name *.asm)

# all object files = source files with the
# extension of .o instead
OBJ_FILES = $(CXX_ALL_SRC_FILES:.cc=.o) \
			$(ASM_SRC_FILES:.asm=.o)

# kernel main function (entry point)
KERNEL_MAIN_FCE = _kmain

# kernel code start address
KERNEL_START_CODE = 0x6400000

# name of the OS, image file, and binary file
OS_NAME  = dudeosv2
OS_IMAGE = $(OS_NAME).img
OS_BIN   = $(OS_NAME).bin

# compiler for c++ files
CXX         = g++
CXX_FLAGS   = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector  \
			  -fno-pic -std=c++14 -fno-rtti -fno-exceptions -Wall -Wextra \
			  -fno-leading-underscore -Wno-write-strings -Iinclude

# linker for object files
LD			= ld 
LD_FLAGS    = --Ttext $(KERNEL_START_CODE) -m elf_i386 -s --oformat binary -e $(KERNEL_MAIN_FCE)

# compiles and builds the operating system
# inside a docker container
.PHONE build-os-in-docker:
build-os-in-docker: build-docker-image
	docker run --rm -v $$PWD:/root/$(OS_NAME) $(OS_NAME)

# creates a docker image as defined
# in the Dockerfile so it could be used
# to compile the os afterwards
.PHONE build-docker-image:
build-docker-image:
	docker build -t $(OS_NAME) .

# builds the OS (this is meant to be done
# inside a docker container)
build-os: user_programs $(OS_IMAGE)

# builds the OS image off of the object files
# 1474560 = 1.4MB
$(OS_IMAGE) : $(OBJ_FILES)
	$(LD) $(LD_FLAGS) $(OBJ_FILES) -o $(OS_BIN)
	make -C boot
	cat boot/boot.bin $(OS_BIN) /dev/zero | head -c 1474560 > $(OS_IMAGE)

# compiles the user programms (shell etc.)
user_programs:
	make -C include/user-programs

# creates object files from cc files (c++)
%.o : %.cc
	$(CXX) $(CXX_FLAGS) -c $^ -o $@

# creates object files from asm files (assembly)
%.o : %.asm
	nasm -f elf $^

# installs qemu as an emulator
# for the operating system
.PHONY install-emulator:
install-emulator:
	apt-get update
	apt-get install qemu-system-i386

# starts the os using qemu as the emulator
# if the image does not exist, run `make compile`
# first to build the image
.PHONY run:
run:
	qemu-system-i386 $(OS_IMAGE)

# removes all the object files as well as
# the image of the os along with its binary file
.PHONY clean:
clean:
	make -C include/user-programs clean &
	make -C boot clean &
	find -name '*.o' | xargs $(RM) &
	$(RM) $(OS_IMAGE) &
	$(RM) $(OS_BIN)