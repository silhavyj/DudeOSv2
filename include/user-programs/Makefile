CXX      = g++
CXXFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	       -fno-pic -std=c++14 -fno-rtti -fno-exceptions -Wall -Wextra

LD       = ld
LDFLAGS  = --Ttext 0x0 --oformat binary -m elf_i386

programs     = idle
header_files = $(programs:=.bin.h)
bin_files    = $(programs:=.bin)

all: system_functions.o $(header_files)

%.bin.h: %.bin 
	xxd -i $< $@

%.bin: %.o
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $^ -o $@

%.o: %.asm
	nasm -f elf $^

.PHONY: clean
clean:
	$(RM) $(header_files) $(bin_files)