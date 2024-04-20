#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSIZE 1024
#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char *argv[]) {
  int sockfd, n;
  char mesg[BUFSIZE];
  struct sockaddr_in servaddr, cliaddr;

  if (argc < 2) {
    printf("Usage: %s <port>\n", argv[0]);
    exit(1);
  }

  int SERV_PORT_UDP = atoi(argv[1]);

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  memset(&servaddr, 0, SLEN);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  servaddr.sin_port = htons(SERV_PORT_UDP);

  if (bind(sockfd, (SADDR *)&servaddr, SLEN) < 0) {
    perror("bind problem");
    exit(1);
  }

  printf("SERVER starts at port %d\n", SERV_PORT_UDP);

  while (1) {
    unsigned int len = SLEN;

    if ((n = recvfrom(sockfd, mesg, BUFSIZE, 0, (SADDR *)&cliaddr, &len)) < 0) {
      perror("recvfrom problem");
      exit(1);
    }

    printf("REQUEST: %s\n", mesg);

    if (sendto(sockfd, mesg, n, 0, (SADDR *)&cliaddr, len) < 0) {
      perror("sendto problem");
      exit(1);
    }
  }

  close(sockfd);
}
