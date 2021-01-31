#include "system.h"
#include "../src/stdlib/string.h"

void print_help();

int main() {
    char *line = (char *)_umalloc(256);

    while (1) {
        _uprintf("/> ");
        _ureadln(line);
        if (strcmp(line, "help", 4) == 0)
            print_help();
        else if (strcmp(line, "exec", 4) == 0) {
            if (_uexec(line + 5) == 0)
                _uprintf("program not found!\n");
        }
        else if (strcmp(line, "ps", 2) == 0)
            _ps();
        else if (strcmp(line, "show programs", 13) == 0)
            _show_programs();
        else if (strlen(line) != 0)
            _uprintf("unknown command!\n");
    }
    _uexit();
}

void print_help() {
    _uprintf("help           - prints out this text\n");
    _uprintf("exec <program> - executes the program\n");
    _uprintf("show programs  - prints out all programs available\n");
    _uprintf("ps             - prints out all processes\n");
}