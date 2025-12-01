#ifndef RESULT_H
#define RESULT_H

#include <assert.h>

typedef enum Result {
    Ok = 0,
    UnknownErr,
    OutOfMemErr,
    RangeErr,
    HashCollisionErr,

    FileUnavailableErr,
    FileReadingErr,
    FileWritingErr,

    FormatErr,
    ArgErr,
    StateErr,

    RESULT_COUNT,  // should be last
} Result;

static char const *const result_msgs[] = {
    [Ok] = "ok",
    [UnknownErr] = "unknown!",
    [OutOfMemErr] = "out of memory!",
    [RangeErr] = "out of range!",
    [HashCollisionErr] = "hash colision!",

    [FileUnavailableErr] = "file unavailable!",
    [FileReadingErr] = "file reading failed!",
    [FileWritingErr] = "file writing failed!",

    [FormatErr] = "invalid format!",
    [ArgErr] = "invalid argument!",
    [StateErr] = "invalid state!"
};

// clang-format off
static_assert(sizeof(result_msgs) / sizeof(*result_msgs) == RESULT_COUNT, "Several results are not described!");
// clang-format on

/// @brief Returns the short message describing what the result is.
/// @param result result to be analyzed
#define explain(result) (result_msgs[result])

/// @brief If result is not Ok, returns it from the current function.
/// @param result result to be analyzed
/// @param ... block to execute before returning
#define UNROLL_CLEANUP(result, ...)                                            \
    {                                                                          \
        Result const error = (result);                                         \
        switch (error) {                                                       \
        case Ok: break;                                                        \
        default: __VA_ARGS__ return error;                                     \
        }                                                                      \
    }                                                                          \
    ((void)0)

/// @brief If result is not Ok, returns it from the current function.
/// @param result result to be analyzed
#define UNROLL(result) UNROLL_CLEANUP(result, )

#endif
