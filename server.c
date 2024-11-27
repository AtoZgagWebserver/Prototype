#include "lib/headerlist.h"
#include "lib/readdata.h"
#include "lib/threadfunc.h"

int main(int argc, char* argv[])
{
    //default set
    if(argc < 3)
    {
        perror("Few argument");
        exit(1);
    }
    int portnum = atoi(argv[1]), worknum = atoi(argv[2]);
    struct sockaddr_in sin, cli;
    int sd, ns, clientlen = sizeof(cli);
    
    // create socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd == -1)
    {
        perror("socket");
        exit(1);
    }

    // port recycle
    int optval = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // create socket struct
    memset((char *)&sin, '\0', sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(portnum);
    sin.sin_addr.s_addr = inet_addr("0.0.0.0");

    //bind
    if (bind(sd, (struct sockaddr *)&sin, sizeof(sin)))
    {
        perror("bind error");
        exit(1);
    }
    if(listen(sd, 50000)) // second parameter is BACKLOG That is max size of queue
    {
        perror("listen error");
        exit(1);
    }
    pthread_t* worker_tid = make_worker(worknum);

    int cnt = 0;
    
    while(1){
		int *ns = malloc(sizeof(int *));
		if((*ns=accept(sd,(struct sockaddr*)&cli,&clientlen))==-1){
			perror("accept");
			exit(1);
		}
        printf("accept\n");
		pthread_t pid;
		pthread_create(&pid,NULL,tcp,(void*)ns);
	}
	close(sd);

    return 0;
}