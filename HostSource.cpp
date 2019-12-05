#include<stdio.h>
#include<WinSock2.h>
#include<WS2tcpip.h>

#pragma comment (lib,"Ws2_32.lib")

void  Server()
{
	int recvSock = 0;

	struct sockaddr_in addr;
	char buf[2048];

	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);

	memset(&recvSock, 0, sizeof(recvSock));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	recvSock = socket(AF_INET, SOCK_DGRAM, 0);

	int status = bind(recvSock, (struct sockaddr*)&addr, sizeof(addr));

	printf("Server:Open\n");

	while (1)
	{
		if (recvfrom(recvSock, buf, 2048, 0, NULL, NULL) == -1)
		{
			status = closesocket(recvSock);
			break;
		}
		printf("%s\n", buf);
	}

	closesocket(recvSock);

	WSACleanup();

	printf("Server:Close\n");
	getchar();

	return;
}

int Multicast()
{
	WSAData wsaData;

	SOCKET sock;
	
	SOCKADDR_IN  addr;
	IP_MREQ mreq;		//マルチキャスト

	char buf[2048];

	WSAStartup(MAKEWORD(2,0),&wsaData);

	sock = socket(AF_INET,SOCK_DGRAM,0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(sock,(LPSOCKADDR)&addr,sizeof(addr));

	memset(&mreq,0,sizeof(mreq));
	mreq.imr_interface.S_un.S_addr = INADDR_ANY;
	mreq.imr_multiaddr.S_un.S_addr = inet_addr("239.192.1.2");

	if(setsockopt(sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,(char*)&mreq,sizeof(mreq)) != 0)
	{
		printf("setsockopt : %d\n",WSAGetLastError());
		return 1;
	}

	printf("Wait\n");

	memset(buf,0,sizeof(buf));
	recv(sock,buf,sizeof(buf),0);

	printf("%s\m",buf);

	closesocket(sock);

	WSACleanup();

	return 0;
}

int main()
{
	Server();
	return 0;
}