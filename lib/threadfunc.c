#include "headerlist.h"
#include "threadfunc.h"

int handclient()
{
    return 0;
    //what to do?
}

pthread_t* make_worker(int work_num)
{
    pthread_t* tid_list = (pthread_t*) malloc(sizeof(thread_t)*work_num);

    for(int i=0; i<n; i++)
    {
        struct ThrInfo* thrinf = (struct ThrInfo*)malloc(sizeof(struct ThrInfo));

        thrinf->number = i;
        thrinf->q = new_queue();

        pthread_create(&tid_list[i],NULL,worker,(void*)thrinf);
    }

    return tid_list;
}

void* worker(void* arg) // worker number
{
    struct ThrInfo inf = *(struct ThrInfo*)arg;

    while(1) // thread 개수는 미정 차차 맞춰갈 예정임. 
    {
        struct Work* w = pop(inf->q);
        if(w == NULL)continue;
        printf("Thread number : %d, Work : %d\n",inf->number ,w->item);
    }
}

struct Queue new_queue()
{
    // make new queue
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));

    // initialize
    q->items = (struct Work*)malloc(sizeof(struct Work));
    q->maxsize = 1;
    q->front = 0;
    q->rear = 0;

    //return
    return q;
}

int empty(struct Queue* q)
{
    return q->front==q->rear;
}

int size(struct Queue* q)
{
    return q->rear-q->front;   
}

int full(struct Queue* q)
{
    return (q->rear-q->front) == q->maxsize;
}

struct Work* pop(struct Queue* q)
{
    //if queue is empty than return NULL;
    if(empty(q))return NULL;

    struct Work* res = &q->items[q->front];
    q->front = (q->front+1)%q->maxsize;

    return res;
}

void push(struct Work w,struct Queue* q)
{
    if(full(q)) // size reallocation 
    {
        realloc(q->items,sizeof(q->items)*2);
        // move item to new space
        if(q->front > q->rear)
        {
            for(int i=0; i<q->rear; i++)
            {
                q->items[maxsize+i] = q->items[i];
            }
        }
        q->rear = q->maxsize + rear;
        q->maxsize*=2;
    }
    q->items[q->rear] = w;
    q->rear = (q->rear + 1)%q->maxsize;
}