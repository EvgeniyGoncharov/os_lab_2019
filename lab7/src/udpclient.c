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
  char mesg[BUFSIZE], recvline[BUFSIZE + 1];
  struct sockaddr_in servaddr;

  if (argc < 4) {
    printf("Usage: %s <ip> <port> <message>\n", argv[0]);
    exit(1);
  }

  int SERV_PORT_UDP = atoi(argv[2]);

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  memset(&servaddr, 0, SLEN);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(argv[1]);
  servaddr.sin_port = htons(SERV_PORT_UDP);

  if (sendto(sockfd, argv[3], strlen(argv[3]), 0, (SADDR *)&servaddr, SLEN) == -1) {
    perror("sendto problem");
    exit(1);
  }

  socklen_t servaddr_len = SLEN;
  if ((n = recvfrom(sockfd, recvline, BUFSIZE, 0, (SADDR *)&servaddr, &servaddr_len)) < 0) {
    perror("recvfrom problem");
    exit(1);
  }

  recvline[n] = 0;
  printf("REPLY FROM SERVER: %s\n", recvline);

  close(sockfd);
}
