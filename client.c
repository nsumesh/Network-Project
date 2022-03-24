#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

char encryptionOfString(int key, char a)
{
    int encrypt;
    char ch = a;
    encrypt = (int)ch;
    encrypt = encrypt + key;
    if(encrypt>90)
    {
        encrypt = encrypt - 'Z' + 'A'-1;
    }
    ch = (char)encrypt;
    return ch;
}


int main(int argc, char **argv)
{
    int clientfd, port;
    char *host;
    char *id;
    char buf[500];
    if(argc!=4)
    {
        exit(0);
    }
    id = argv[1];
    host = argv[3];
    port = atoi(argv[2]);
    struct sockaddr_in serveraddr;
    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        clientfd = -1;
    }
    bzero((char *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    inet_pton(AF_INET, host, &serveraddr.sin_addr);  
    if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    {
        clientfd = -1;
    }
    char msg[500];
    sprintf(msg, "cs230 HELLO %s\n", id);
    send(clientfd, msg, strlen(msg), 0);
    int n = recv(clientfd, &buf, sizeof(buf), 0);
    buf[n] = '\0';
    char cs[500], st[500], enc[500];
    int key;
    char c;
    while(strstr(buf,"bye")!="bye")
    {
        sscanf(buf,"%s %s %d %s\n", cs, st, &key, enc);
        for(int i = 0; i<strlen(enc); i++)
        {
            enc[i] = encryptionOfString(key, enc[i]);
        }
        sprintf(cs,"cs230 %s\n", enc);
	printf("%s",cs);
        send(clientfd, cs, strlen(cs), 0);
        n= recv(clientfd, buf, sizeof(buf), 0);
        buf[n] = '\0';
    }
    printf("Flag has been found, Terminated");
}
