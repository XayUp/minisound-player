#include <stdio.h>

#include <woodi.h>

// defining a wrapper

#define LOGGER_INTERFACE(Self)                                                 \
    { int (*const log)(Self *const self, char *const str); }

WRAPPER(Logger, LOGGER_INTERFACE);

// you may want to define a function if you want. i prefer macros because they
// are shorter
#define logger_log(self, str) WRAPPER_CALL(log, self, str)

// defining implementations

typedef struct SimpleLogger {
    FILE *file;
} SimpleLogger;

void simple_logger_init(SimpleLogger *const self, FILE *file) {
    self->file = file;
}
void simple_logger_uninit(SimpleLogger *const self) { self->file = NULL; }

int simple_logger_log(SimpleLogger *const self, char *const str) {
    if (fputs(str, self->file) == EOF) return 1;

    return 0;
}

// wrapping

// you declare a function, which body is provided by `WRAP_BODY` macro. you may
// want to declare it in header and define in source or to define as `inline` in
// header
Logger simple_logger_ww_logger(SimpleLogger *const self) WRAP_BODY(
    Logger,
    // careful with this - there are no type safety here
    LOGGER_INTERFACE(SimpleLogger),
    {
        .log = simple_logger_log,
    }
);

// usage

void log_helloworld(Logger *const logger) {
    logger_log(logger, "Hello world!");
}

int main() {
    SimpleLogger simple_logger;
    simple_logger_init(&simple_logger, stdout);

    // any logger can be injected here which is pretty cool
    Logger logger = simple_logger_ww_logger(&simple_logger);
    log_helloworld(&logger);

    simple_logger_uninit(&simple_logger);
    // logger should not be used from here, as it's object got uninited
}
