#include <assert.h>
#define ERROR(m) (fprintf(stderr, __FILE__ ":%s():%d: error: %s\n", __func__, __LINE__, m))
#define LOG(m) (fprintf(stderr, __FILE__ ":%s():%d: log: %s\n", __func__, __LINE__, m))

/*
 * Used for error messages.
 */
char str[120];
