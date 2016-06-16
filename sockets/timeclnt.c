#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
int main(int argc, char *argv[])
{
	struct sockaddr_in servadd;
	struct hostent *hp;
	int sock_id, sock_fd, messlen;
	char message[BUFSIZ];
	sock_id = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servadd, sizeof(servadd));
	//hp = gethostbyname(argv[1]);
	//bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);
	servadd.sin_addr.s_addr = inet_addr(argv[1]);
	servadd.sin_port = htons(atoi(argv[2]));
	servadd.sin_family = AF_INET;
	connect(sock_id, (struct sockaddr *)&servadd, sizeof(servadd));
	messlen = read(sock_id, message, BUFSIZ);
	printf("%s", message);
	close(sock_id);
	return 0;
}
