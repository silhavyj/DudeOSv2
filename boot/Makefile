TARGET = boot.bin
SRC    = boot.asm

all: $(TARGET)

$(TARGET) : $(SRC)
	nasm -f bin $^ -o $@

run: $(TARGET)
	qemu-system-i386 $(TARGET)

.PHONY clean:
clean:
	rm $(TARGET)