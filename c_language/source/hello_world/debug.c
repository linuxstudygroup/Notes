#define LOGGING_LEVEL 0
#define WARNING_LEVEL 1
#define ERROR_LEVEL 2

#define DEBUG_LEVEL WARNING_LEVEL

#define DEBUG(x) ((x) >= DEBUG_LEVEL)

#if DEBUG(LOGGING_LEVEL)
int logging;
#endif

#if DEBUG(WARNING_LEVEL)
int warning;
#endif

#if DEBUG(ERROR_LEVEL)
int error;
#endif
