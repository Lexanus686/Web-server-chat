#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
  
  DWORD WINAPI Client2ToClient(LPVOID client_socket);
  DWORD WINAPI ClientToClient2(LPVOID client_socket2);

int main (void)
{
	system("chcp 1251");
	system("cls");
	struct sockaddr_in local;
	int s, rc;
	char buff[1000], local_buf[20];
	
	if (WSAStartup(0x0202,(WSADATA *) &local_buf[0])) 
    {
	      printf("Error WSAStartup %d\n",
             WSAGetLastError());
      return -1;
    }
	
	local.sin_family = AF_INET;
	local.sin_port = htons (7500);
	local.sin_addr.s_addr = htonl (INADDR_ANY);
	
	s = socket (AF_INET, SOCK_STREAM, 0);
	if (s < 0)
	{
		perror("ошибка вызова socket");
		exit(1);
	}
	
	rc = bind(s, (struct sockaddr *)&local, sizeof(local));
	if (rc < 0)
	{
		perror("ошибка вызова bind");
		exit(1);
	}
	
	rc = listen(s, 5);
	if (rc)
	{
		perror("ошибка вызова listen");
		exit(1);
	}
	
	printf("Server v1.4b is online\n");
	
	printf("ќжидание подключений\n");

    SOCKET client_socket;
    struct sockaddr_in client_addr;

    int client_addr_size=sizeof(client_addr);
    
    SOCKET client_socket2;
    struct sockaddr_in client_addr2;
    
    int client_addr_size2=sizeof(client_addr2);
    DWORD thID, thID2;
    
	while (((client_socket = accept(s, &client_addr, &client_addr_size)) && (client_socket2 = accept(s, &client_addr2, &client_addr_size2))))
	{
      CreateThread(NULL,NULL,Client2ToClient,
              &client_socket,NULL,&thID);
      CreateThread(NULL,NULL,ClientToClient2,
              &client_socket2,NULL,&thID2);
    
    FILE *fileWriter = fopen("text.txt", "a+");
    fseek(fileWriter, 0, SEEK_END);
    int file_size = ftell(fileWriter);
    fseek(fileWriter, 0, SEEK_SET);
    char* loader[file_size];
    loader[0] = 0;
    fread(&loader[0], file_size, 1, fileWriter);
    
    send(client_socket, &loader[0], file_size, 0);
    send(client_socket2, &loader[0], file_size, 0);
    
	buff[0] = 0;
    int flag = 0;
    while(1)	
      if (flag == 0){
        int bytes_here = recv(client_socket, &buff[0], sizeof(buff), 0);
	    if ((bytes_here <= 0) || (!strcmp(&buff[0], "quit\n")))
	    {	
	  	closesocket(client_socket);
	  	closesocket(client_socket2);
	  	close(fileWriter);
	  	return 0;
        }
        fprintf(fileWriter, buff);
        send(client_socket2, &buff[0],bytes_here,0);
        flag++;
	    }
      else if (flag == 1)
	  {
        int bytes_here = recv(client_socket2, &buff[0], sizeof(buff), 0);
	    if ((bytes_here <= 0) || (!strcmp(&buff[0], "quit\n")))
	    {
	  	closesocket(client_socket2);
	  	closesocket(client_socket);
	  	close(fileWriter);
	  	return 0;
        }
        fprintf(fileWriter, buff);
        send(client_socket, &buff[0],bytes_here,0);
        flag--;
	  }
}
}
DWORD WINAPI Client2ToClient(LPVOID client_socket){}
DWORD WINAPI ClientToClient2(LPVOID client_socket2){}
