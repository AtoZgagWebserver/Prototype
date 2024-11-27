#include "httpfunc.h"

void parse_http_request(const char *request, HTTPRequest *http_request) {
    //요청 메소드, 경로, 내용 길이, 내용 만 일단 추출
    char *pos;

    // 요청 메소드 (GET, POST) 추출
    if (strncmp(request, "GET", 3) == 0) {
        strcpy(http_request->method, "GET");
        pos = strchr(request + 4, ' ');
        if (pos != NULL) {
            strncpy(http_request->path, request + 4, pos - (request + 4));
            http_request->path[pos - (request + 4)] = '\0';
        }
    } else if (strncmp(request, "POST", 4) == 0) {
        strcpy(http_request->method, "POST");
        pos = strchr(request + 5, ' ');
        if (pos != NULL) {
            strncpy(http_request->path, request + 5, pos - (request + 5));
            http_request->path[pos - (request + 5)] = '\0';
        }
    } else {
        strcpy(http_request->method, "UNKNOWN");
    }

    http_request->content_length = 0;
    pos = strstr(request, "Content-Length: ");
    if (pos != NULL) {
        http_request->content_length = atoi(pos + 16);
    }

    pos = strstr(request, "\r\n\r\n");
    if (pos != NULL) {
        strcpy(http_request->body, pos + 4);
    } else {
        http_request->body[0] = '\0';
    }
}

void send_file_content(int cli, const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("file not found\n");
        const char *not_found_response = 
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n"
            "\r\n"
            "404 Not Found";
        send(cli, not_found_response, strlen(not_found_response), 0);
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *file_content = malloc(file_size + 1);
    if (file_content == NULL) {
        perror("malloc");
        fclose(file);
        return;
    }

    fread(file_content, 1, file_size, file);
    file_content[file_size] = '\0';

    char header[256];
    snprintf(header, sizeof(header), 
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n" //html로 테스트, css,javascript,csv, 이미지, 오디오, 비디오 등 다양한 형태가 있어서
             "Content-Length: %ld\r\n" //요청한 데이터가 어떤건지 확인한느 부분이 필요?
             "\r\n", file_size);
    send(cli, header, strlen(header), 0);
    send(cli, file_content, file_size, 0);

    free(file_content);
    fclose(file);
}

void *tcp(void* arg) {
    int *ns = (int *)arg;
    char buf[HTTP_BUF_MAX_SIZE];  // 수신 버퍼
    ssize_t n;
    char data[HTTP_BUF_MAX_SIZE];  // 요청 데이터를 저장할 배열
    int data_len = 0;

    // 클라이언트로부터 HTTP 요청을 수신
    n = recv(*ns, data, HTTP_BUF_MAX_SIZE - 1, 0);
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
        if (strcmp(http_request.method, "GET") == 0) { //GET 요청의 경우
            printf("get -> html리턴\n");
            char file_path[512];
            snprintf(file_path, sizeof(file_path), "./static/%s", http_request.path[0] == '/' ? http_request.path + 1 : http_request.path);
            send_file_content(*ns, file_path);
        }
        if (strcmp(http_request.method, "POST") == 0) { 
        }


    }

    close(*ns);
    free(ns);
    return NULL;
}