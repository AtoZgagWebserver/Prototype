#include "readdata.h"
#include "headerlist.h"
#include <stdlib.h>

struct QuestionList* read_gag()
{
    struct QuestionList* q_list = (struct QuestionList*)malloc(sizeof(struct QuestionList));
    q_list->size = 0;
    q_list->maxsize = 1;
    q_list->item = (struct Question*)malloc(sizeof(struct Question));

    FILE* fp = fopen("../rsc/gag/gag1.txt", "r");
    if(fp == NULL)
    {
        perror("File Open error");
        exit(1);
    }

    char quest[256] = {0};
    char ans[256] = {0};
    while(fgets(quest,255,fp)!=NULL)
    {
        if(fgets(ans,255,fp)==NULL)
        {
            perror("data read error");
            exit(1);
        }
        q_list->item[q_list->size].quest = strdup(quest);
        q_list->item[q_list->size].ans = strdup(ans);
        q_list->size++;
        if(q_list->size>=q_list->maxisze)
        {
            q_list->maxsize*=2;
            realloc(q_list->item,sizeof(struct Question)*q_list->maxsize);
        }
    }

    return q_list;
}

struct Question* get_random_Question(struct QuestionList* q_list, int q_size)
{
    //ready for random function
    srand(time(NULL));
    int* chk = (int*)calloc(sizeof(int),q_size);
    if(chk == NULL)
    {
        return NULL;
    }
    //result array
    struct Question* result = (struct Question*)malloc(sizeof(struct Question)*q_size);
    if(result == NULL)
    {
        return NULL;
    }
    
    for(int i=0; i<q_size; i++)
    {
        int random_q_number = rand()%q_list->size;
        result[i] = q_list->item[random_q_number];
    }

    return result;
}
