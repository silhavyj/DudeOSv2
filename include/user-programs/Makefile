CXX      = g++
CXXFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector  \
	       -fno-pic -std=c++14 -fno-rtti -fno-exceptions -Wall -Wextra \
		   -Iinclude

LD       = ld
LDFLAGS  = --Ttext 0x0 --oformat binary -m elf_i386

programs     = exe/idle exe/shell exe/error exe/test_malloc exe/calc exe/test_fork exe/test_fork2 exe/pipe1 exe/pipe2
header_files = $(programs:=.bin.h)
bin_files    = $(programs:=.bin)

all: $(header_files)

%.bin.h: %.bin 
	xxd -i $< $@
 
%.bin: %.o memory.o  \
		   string.o  \
		   math.o    \
		   system.o system_calls.o
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $^ -o $@

%.o: %.asm
	nasm -f elf $^

.PHONY: clean
clean:
	$(RM) $(header_files) $(bin_files) *.o