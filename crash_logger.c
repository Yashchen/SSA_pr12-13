#define _GNU_SOURCE
#include <signal.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "crash_logger.h"
#include <string.h>

static crash_handler_callback_t user_callback = NULL;

static void signal_handler(int signo) {
    void *buffer[30];
    int size;

    // Вивід сигналу
    fprintf(stderr, "Crash detected: signal %d (%s)\n", signo, strsignal(signo));

    // Стек викликів
    size = backtrace(buffer, 30);
    fprintf(stderr, "Stack trace:\n");
    backtrace_symbols_fd(buffer, size, STDERR_FILENO);

    // Виклик користувацького callback, якщо задано
    if (user_callback) {
        user_callback(signo);
    }

    // Відновлення поведінки за замовчуванням і повторне відправлення сигналу,
    // щоб ядро могло створити core dump.
    signal(signo, SIG_DFL);
    raise(signo);
}

void crash_logger_init(crash_handler_callback_t callback) {
    user_callback = callback;

    // Сигнали для обробки падіння
    int signals[] = {SIGSEGV, SIGABRT, SIGFPE, SIGILL, SIGBUS};
    int n = sizeof(signals) / sizeof(signals[0]);

    for (int i = 0; i < n; i++) {
        struct sigaction sa;
        sa.sa_handler = signal_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART | SA_NODEFER;
        sigaction(signals[i], &sa, NULL);
    }
}
