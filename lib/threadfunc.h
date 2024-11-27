#include "headerlist.h"

#ifndef THREADSTRUCT
#define THREADSTRUCT

struct ThrInfo
{
    int number;
    pthread_t tid;
    struct Queue* q;
};

struct Work
{
    int ns;
};

struct Queue
{
    struct Work** items;
    int front,rear;
    int maxsize;
};

#endif

void* worker(void* arg);
int handleclient();
struct Queue* new_queue();
int empty(struct Queue* q);
int size(struct Queue* q);
struct Work* pop(struct Queue* q);
void push(struct Work w,struct Queue* q);
pthread_t* make_worker(int work_num);
