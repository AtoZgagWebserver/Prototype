#include "readdata.h"

struct Question* read_gag()
{
    struct Question* quest_list = (struct Question*)malloc(sizeof(struct Question)*1600);

    FILE* fp = fopen("../data/gag/gag1.txt", "r");
    if(fp == NULL)
    {
        perror("File Open error");
        exit(1);
    }

    int cnt = 0;
    char quest[256] = {0};
    char ans[256] = {0};
    
    while(fgets(quest,255,fp)!=NULL){
        if(fgets(ans,255,fp)==NULL){
            perror("data read error");
            exit(1);
        }
        quest_list[cnt].quest = strdup(quest);
        quest_list[cnt].ans = strdup(ans);
        cnt++; 
    }

    return quest_list;
}

