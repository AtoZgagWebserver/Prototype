#include "headerlist.h"

#ifndef HTTPDEFINE
#define HTTPDEFINE

#define HTTP_BUF_MAX_SIZE 1024

#endif

#ifndef HTTPSTRUCT
#define HTTPSTRUCT

struct HTTPRequest
{
    char method[8];
    char path[256];
    int content_length;
    char body[HTTP_BUF_MAX_SIZE];
};

#endif

