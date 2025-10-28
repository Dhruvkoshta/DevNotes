#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

struct sockaddr_in createIPv4Address(const char *ip, int port) {
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons((uint16_t)port);
  if (inet_pton(AF_INET, ip, &addr.sin_addr) != 1) {
    perror("inet_pton");
    exit(1);
  }
  return addr;
}

int main(void) {
  const char *ip = "74.125.200.102"; // an example Google IP
  int port = 80;

  int socketFD = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFD == -1) {
    perror("socket");
    return 1;
  }

  struct sockaddr_in serverAddress = createIPv4Address(ip, port);

  if (connect(socketFD, (struct sockaddr *)&serverAddress,
              sizeof(serverAddress)) == -1) {
    perror("connect");
    close(socketFD);
    return 1;
  }
  printf("Connected to the server\n");

  const char *message = "GET / HTTP/1.1\r\n"
                        "Host: google.com\r\n"
                        "Connection: close\r\n"
                        "\r\n";

  size_t to_send = strlen(message);
  size_t sent_total = 0;
  while (sent_total < to_send) {
    ssize_t n = send(socketFD, message + sent_total, to_send - sent_total, 0);
    if (n <= 0) {
      perror("send");
      close(socketFD);
      return 1;
    }
    sent_total += (size_t)n;
  }

  char buffer[4096];
  ssize_t nread;
  while ((nread = recv(socketFD, buffer, sizeof(buffer) - 1, 0)) > 0) {
    buffer[nread] = '\0';
    fputs(buffer, stdout);
  }
  if (nread < 0)
    perror("recv");

  close(socketFD);
  return 0;
}
