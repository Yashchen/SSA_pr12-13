#include <stdio.h>
#include "crash_logger.h"

void my_crash_callback(int signo) {
    // Додаткові дії при падінні, наприклад, запис у файл
    FILE *f = fopen("crash_log.txt", "a");
    if (f) {
        fprintf(f, "Crash callback: signal %d\n", signo);
        fclose(f);
    }
}

int main() {
    crash_logger_init(my_crash_callback);

    printf("App started\n");

    // Примусове падіння, щоб перевірити логер
    int *p = NULL;
    *p = 42;  // викликає SIGSEGV

    return 0;
}
