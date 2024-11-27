#include "threadfunc.h"
#include "httpfunc.h"

int handle_client(struct Work* w)
{
    int ns = w->ns;
    char buf[HTTP_BUF_MAX_SIZE]; // 수신 버퍼
    char data[HTTP_BUF_MAX_SIZE]; // 요청 데이터를 저장할 배열
    ssize_t n;
    int data_len = 0;

    // 클라이언트로부터 HTTP 요청을 수신
    n = recv(ns, data, HTTP_BUF_MAX_SIZE - 1, 0);
    data_len=strlen(data);

    if(n>0){
        data[data_len] = '\0';

        HTTPRequest http_request;
        memset(&http_request, 0, sizeof(HTTPRequest));

        parse_http_request(data, &http_request);

        // printf("Method: %s\n", http_request.method);
        // printf("Path: %s\n", http_request.path);
        // if (http_request.content_length > 0) {
        //     printf("Content-Length: %d\n", http_request.content_length);
        // }
        // if (strlen(http_request.body) > 0) {
        //     printf("Body:\n%s\n", http_request.body);
        // }

        //요청에 따라 어떻게 처리할지
        if (strcmp(http_request.method, "GET") == 0) 
        { //GET 요청의 경우
            printf("get -> html리턴\n");
            char file_path[512];
            snprintf(file_path, sizeof(file_path), "./static/%s", http_request.path[0] == '/' ? http_request.path + 1 : http_request.path);
            send_file_content(ns, file_path);
        }
        if (strcmp(http_request.method, "POST") == 0) 
        { 
            
        }
    }

    close(ns);
    free(w);

    return 0;
}

struct ThrInfo* make_worker(int work_num)
{
    pthread_t* tid_list = (pthread_t*) malloc(sizeof(pthread_t)*work_num);
    struct ThrInfo* thrinflist = (struct ThrInfo*)malloc(sizeof(struct ThrInfo)*work_num);
    for(int i=0; i<work_num; i++)
    {
        thrinflist[i].number = i;
        thrinflist[i].q = new_queue();
        pthread_create(&thrinflist[i].tid,NULL,worker,(void*)&thrinflist[i]);
    }

    return thrinflist;
}

void* worker(void* arg) // worker number
{
    struct ThrInfo* inf = (struct ThrInfo*)arg;

    while(1) // thread 개수는 미정 차차 맞춰갈 예정임. 
    {
        struct Work* w = pop(inf->q);
        if(w == NULL)continue;
        handle_client(w);
    }
}

struct Queue* new_queue()
{
    // make new queue
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));

    // initialize
    q->items = (struct Work**)malloc(sizeof(struct Work*));
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

void push(struct Work* w,struct Queue* q)
{
    if(full(q)) // size reallocation 
    {
        if(realloc(q->items,sizeof(q->items)*2)==NULL)
        {
            perror("Realloc");
            exit(1);
        }
        // move item to new space
        if(q->front > q->rear)
        {
            for(int i=0; i<q->rear; i++)
            {
                q->items[q->maxsize+i] = q->items[i];
            }
        }
        q->rear = q->maxsize + q->rear;
        q->maxsize*=2;
    }
    q->items[q->rear] = w;
    q->rear = (q->rear + 1)%q->maxsize;
}