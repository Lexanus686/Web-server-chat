#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check (char* buf[0], int *nbytes, SOCKET *s){
	if (!strcmp(&buf[0], "quit\n") || (*nbytes <= 0))
    	{
    		printf("Exit...");
            closesocket(*s);
            WSACleanup();
            return 0;
		}
}

int main (void)
{
	struct sockaddr_in peer;
	int s, rc;
	char buf[1000];
	system("chcp 1251");
	system("cls");
	
	if (WSAStartup(0x202,(WSADATA *)&buf[0]))
    {
      printf("WSAStart error %d\n",WSAGetLastError());
      return -1;
    }
	peer.sin_family = AF_INET;
	peer.sin_port = htons(7500);
	peer.sin_addr.s_addr = inet_addr ("127.0.0.1");
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0)
	{
		perror("מרטבךא גûחמגא socket");
		exit(1);
	}
	rc = connect(s, (struct sockaddr *)&peer, sizeof(peer));
	if (rc)
	{
		perror("מרטבךא גûחמגא connect");
		exit(1);
	}
	recv(s, &buf[0], sizeof(buf), 0);
	printf("History: %s\n", buf);
	buf[0] = 0;
	printf("Type quit for Exit from this silly chat\n");  
	int nbytes = 1;
	while (1){
    	printf("You:");
    	fgets(&buf[0], sizeof(buf), stdin);
    	send(s, &buf[0], sizeof(buf), 0);
    	if (!check(&buf[0], &nbytes, &s))
    	  return 0;
    	nbytes = recv(s, &buf[0], sizeof(buf), 0);
    	if (!check(&buf[0], &nbytes, &s))
    	  return 0;
    	printf("He:%s", buf);
}

}
