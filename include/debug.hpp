
#pragma once

#define DEBUG_MODE 0
// #define DEBUG_MODE 1

#define debug(fmt, ...) \
            do { if (DEBUG_MODE) fprintf(stderr, fmt, ##__VA_ARGS__); } while (0)
