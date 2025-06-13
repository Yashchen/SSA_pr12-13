#ifndef CRASH_LOGGER_H
#define CRASH_LOGGER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*crash_handler_callback_t)(int signo);

/// Ініціалізувати логер падінь.
/// При переданні callback - він буде викликаний під час падіння.
void crash_logger_init(crash_handler_callback_t callback);

#ifdef __cplusplus
}
#endif

#endif // CRASH_LOGGER_H
