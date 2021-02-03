#include <system.h>
#include <stdlib/string.h>

#define HELP          "help"
#define EXEC          "exec"
#define PS            "ps"
#define KILL          "kill"
#define SHOW_PROGRAMS "show programs"
#define CLEAR         "clear"

void print_help();

int main() {
    unsigned int pid;
    char *line = (char *)_umalloc(256);

    while (1) {
        _uprintf("/> ");
        _ureadln(line);
        if (strcmp(line, HELP, strlen(HELP)) == 0)
            print_help();
        else if (strcmp(line, EXEC, strlen(EXEC)) == 0) {
            if (_uexec(line + strlen(EXEC) + 1) == 0)
                _uprintf("program not found!\n");
        }
        else if (strcmp(line, PS, strlen(PS)) == 0)
            _ps();
        else if (strcmp(line, SHOW_PROGRAMS, strlen(SHOW_PROGRAMS)) == 0)
            _show_programs();
        else if (strcmp(line, KILL, strlen(KILL)) == 0) {
            pid = atoi(line + strlen(KILL) + 1);
            if (pid != 0) {
                if (_ukill(pid) != 0)
                    _uprintf("invalid pid!\n");
            } else
                _uprintf("invalid pid!\n");
        }
        else if (strcmp(line, CLEAR, strlen(CLEAR)) == 0)
            _uclear();
        else if (strlen(line) != 0)
            _uprintf("unknown command!\n");
    }
    _uexit();
}

void print_help() {
    _uprintf("help           - prints out this text\n");
    _uprintf("exec <program> - executes the program\n");
    _uprintf("show programs  - prints out all programs available\n");
    _uprintf("kill <pid>     - kills the process\n");
    _uprintf("ps             - prints out all processes\n");
    _uprintf("clear          - clears the screen\n");
}