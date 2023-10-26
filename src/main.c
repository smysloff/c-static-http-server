/* see: https://www.rsdn.org/article/unix/sockets.xml */
/* see: https://dev.to/jeffreythecoder/how-i-built-a-simple-http-server-from-scratch-using-c-739 */

#include <sys/types.h>  /* for some BSD compatibility */
#include <sys/socket.h> /* socket(), bind(), listen(), accept(), send() */
#include <netinet/in.h> /* sockaddr_in */

#include <stdio.h>      /* printf(), perror() */
#include <unistd.h>     /* close(), _exit() */
#include <string.h>     /* strlen(), strcpy(), strcat() */
#include <stdlib.h>     /* malloc(), EXIT_FAILURE, EXIT_SUCCESS */

#include "readfile.h"

#define PORT 3000

static int server_sockfd;
static struct sockaddr_in addr;

void
die(const char* msg, char* data)
{
  perror(msg);
  free(data);
  close(server_sockfd);
  _exit(EXIT_FAILURE);
}

int
main(void)
{
  char* response;
  size_t size;

  /* read index.html to memory */
  const char header[] = "HTTP/1.1 200 OK\nContent-Type:text/html\r\n\n";
  char* body = readfile("views/index.html");
  if (!body) die("body", "");
  size = sizeof(header) + strlen(body);
  if (!(response = malloc(size))) die("response", body);
  strcpy(response, header);
  strcat(response, body);
  free(body);

  /* create server and server socket */
  printf("INFO: socket creation and HTTP-server running\n");
  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sockfd < 0) die("socket", response);

  /* config socket */
  printf("INFO: socket configuration\n");
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(PORT);

  /* bind socket */
  printf("INFO: socket binding\n");
  if (bind(server_sockfd, (struct sockaddr*) &addr, sizeof(addr)) < 0)
    die("bind", response);

  /* listen socket */
  printf("INFO: socket start listening\n");
  if (listen(server_sockfd, 1) < 0)
    die("listen", response);

  while (1)
  {
    int client_sockfd;

    /* accept socket */
    printf("INFO: socket ready to accept new connection\n");
    client_sockfd = accept(server_sockfd, NULL, NULL);
    if (server_sockfd < 0) die("accept", response);

    /* send response */
    printf("INFO: socket get connection and send response\n");
    send(client_sockfd, response, size, 0);

    /* close socket */
    printf("INFO: socket close connection\n");
    close(client_sockfd);
  }	

  /* stop server */
  printf("INFO: socket closing and HTTP-server stopping\n");
  close(server_sockfd);

  /* cleanup */
  free(response);

  return EXIT_SUCCESS;
}
