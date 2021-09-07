#include <assert.h>
#define FAIL(m) (fprintf(stderr, __FILE__ ":%s():%d: error: %s\n", __func__, __LINE__, m), err++)
#define LOG(m) (fprintf(stderr, __FILE__ ":%s():%d: log: %s\n", __func__, __LINE__, m))
extern int err;
