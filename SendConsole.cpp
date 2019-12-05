#include<stdio.h>
#include<WinSock2.h>
#include<ws2tcpip.h>


#pragma comment (lib,"Ws2_32.lib")

void Cast()
{
	char destination[80] = "255.255.255.255";

	int Sock;
	int yes = 1;
	struct sockaddr_in addr;

	char toSendText[300] = {"\0"};

	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);

	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = inet_addr(destination);

	Sock = socket(AF_INET, SOCK_DGRAM, 0);

	//ブロードキャスト ON
	setsockopt(Sock, SOL_SOCKET, SO_BROADCAST, (char*)&yes, sizeof(yes));


	int Select = 0;

	char name[10] = {"\0"};
	printf("UserName > ");
	scanf("%s",&name[0]);
	rewind(stdin);

	while(1)
	{
		/*
		char Get[1024];
		if(recv(Sock,Get,1024,0) == 0)
		{
			printf("Get > %s\n",&Get);
		}
		*/
		char Message[210] = {"\0"};

		printf("Message:");
		scanf("%[^\n]",Message);
		rewind(stdin);

		if(strcmp(&Message[0],"END") == 0)
		{
			break;
		}

		strcpy(&toSendText[0],"\0");
		strcat(&toSendText[0],&name[0]);
		strcat(&toSendText[0],":");
		strcat(&toSendText[0],&Message[0]);

		sendto(Sock, toSendText, strlen(toSendText) + 1, 0, (struct sockaddr*)&addr, sizeof(addr));
	}

	closesocket(Sock);
	WSACleanup();
	return;
}

void Multicast()
{
	//マルチキャストONのプログラムが足りない
	WSAData wsaData;

	SOCKET sock;
	SOCKADDR_IN addr;
	DWORD ipaddr;

	WSAStartup(MAKEWORD(2,0),&wsaData);

	sock = socket(AF_INET,SOCK_DGRAM,0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);

	//マルチキャスト専用のアドレス
	addr.sin_addr.S_un.S_addr = inet_addr("239.192.1.2");

	//ループバックアドレス　自分自身
	ipaddr = inet_addr("127.0.0.1");

	if(setsockopt(sock,IPPROTO_IP,IP_MULTICAST_IF,(char*)&ipaddr,sizeof(ipaddr) != 0))
	{
		printf("setsockopt:%d\n",WSAGetLastError());
		getchar();
		return;
	}

	sendto(sock,"HELLO",5,0,(SOCKADDR*)&addr,sizeof(addr));

	closesocket(sock);
	WSACleanup();

	return;
}

void UDP_BroadCast()
{
	WSAData wsaData;
	SOCKET sock;
	SOCKADDR_IN addr;
	int yes = 1;

	sock = socket(AF_INET,SOCK_DGRAM,0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = inet_addr("255.255.255.255");

	setsockopt(sock,SOL_SOCKET,SO_BROADCAST,(char*)&yes,sizeof(yes));

	sendto(sock,"HELLO",5,0,(SOCKADDR*)&addr,sizeof(addr));

	printf("Send");
	getchar();

	closesocket(sock);
	WSACleanup();

	return;
}

int main()
{
	Cast();
	return 0;
}