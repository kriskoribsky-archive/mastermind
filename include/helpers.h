#ifndef _HELPERS_H
#define _HELPERS_H

#include "stdbool.h"

/* assertions */
#ifdef DEBUG
#include <assert.h>
#define ASSERT(expr) assert(expr)
#else
#define ASSERT(expr) ((void)0)
#endif

/* pointers, dynamic memory management */
#define CHECK_NULL(ptr)    \
    {                      \
        if ((ptr) == NULL) \
        {                  \
            return NULL;   \
        }                  \
    }

#define CHECK_NULL_FALSE(ptr) \
    {                         \
        if ((ptr) == NULL)    \
        {                     \
            return false;     \
        }                     \
    }

#define CHECK_NULL_VOID(ptr) \
    {                        \
        if ((ptr) == NULL)   \
        {                    \
            return;          \
        }                    \
    }

#define CHECK_EMPTY(s)    \
    {                     \
        if ((*s) == '\0') \
        {                 \
            return NULL;  \
        }                 \
    }

#define MALLOC(nmemb, ptr)                                          \
    {                                                               \
        (ptr) = (decltype(ptr)) malloc((nmemb) * (sizeof(*(ptr)))); \
        assert((ptr) != NULL);                                      \
    }

#define CALLOC(nmemb, ptr)                                          \
    {                                                               \
        (ptr) = (decltype(ptr)) calloc((nmemb), (sizeof(*(ptr))));  \
        assert((ptr) != NULL);                                      \
    }

#define FREE(ptr)     \
    {                 \
        free(ptr);    \
        (ptr) = NULL; \
    }

#endif // _HELPERS_H
