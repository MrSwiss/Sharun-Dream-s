#include "boot.h"
#include "Player.h"

#include <errno.h>

SOCKET sock;
void *Test_Thread;
bool localhost = false;
bool net_run = false;
bool net_running = false;

void Network_Init()
{
#ifdef WIN32
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2,0), &WSAData);
#endif
	localhost = false;
	sock = -1;
	net_run = false;
	net_running = false;
	Test_Thread = NULL;
}

char exiting(char);
void Network_Kill()
{
	exiting(1);
}

void Network_Test(void *arg)
{
	int sock2, ret = 0, count = 10;
	char buffer[10];
	struct sockaddr_in sin;
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	sin.sin_family = AF_INET;
	sin.sin_port = htons(Cfg_Server_Port);
	DEBUG("%s (%i) :: Testing Connexion...\n", __func__, __LINE__);

	sock2 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock2 >= 0) {
		if (connect(sock2, (const struct sockaddr *)&sin, sizeof(sin)) >= 0) {
			ret = recv(sock2, buffer, 10, 0);
			if (!ret) {
				close(sock2);
				sock2 = -1;
				Network_Close();
			}
		}
		close(sock2);
	}
	if (!count) Network_Kill();
	DEBUG("%s (%i) :: Connexion Ok.\n\n", __func__, __LINE__);
	Test_Thread = 0;
	if (arg) c_THREAD_free(NULL);
}

int Network_Start(unsigned short port)
{
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == -1) {
#ifdef WIN32
		DEBUG("%s (%i) :: Could not create socket (%d)\n", __func__, __LINE__, WSAGetLastError());
#else
		DEBUG("%s (%i) :: Could not create socket (%d)\n", __func__, __LINE__, errno);
#endif
		return 1;
	}

	struct sockaddr_in sa;
	sa.sin_family = AF_INET;
	if (localhost)
		sa.sin_addr.s_addr = inet_addr("127.0.0.1");
	else
		sa.sin_addr.s_addr = INADDR_ANY;
	sa.sin_port = htons(port);

	int reuse = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(int));

	if (bind(sock, (SOCKADDR *)&sa, sizeof(sa)) == -1) {
#ifdef WIN32
		DEBUG("%s (%i) :: Could not bind socket (%d)\n", __func__, __LINE__, WSAGetLastError());
#else
		DEBUG("%s (%i) :: Could not bind socket (%d)\n", __func__, __LINE__, errno);
#endif
		return 1;
	}

	if (listen(sock, 0) == -1) {
#ifdef WIN32
		DEBUG("%s (%i) :: Could not listen socket (%d)\n", __func__, __LINE__, WSAGetLastError());
#else
		DEBUG("%s (%i) :: Could not listen socket (%d)\n", __func__, __LINE__, errno);
#endif
		return 1;
	}

	DEBUG("%s (%i) :: Network Started (%s : %i).\n", __func__, __LINE__, localhost ? "localhost" : "ANY", port);
	return 0;
}


void Network_Stop()
{
	net_run = false;
}


void NetLink_close(netlink_t *NetLink)
{
	if (NetLink->sock >= 0) closesocket(NetLink->sock);
	NetLink->sock = 0;
	if (NetLink->Head) free(NetLink->Head);
	NetLink->Head = NULL;
	CloseHandle(NetLink->C_Thread);
	NetLink->C_Thread = 0;
}

void Network_Detect(netlink_t *NetLink)
{

	NetLink->Head = malloc(512);
	memset(NetLink->Head, 0, 512);
#ifndef NO_HTTP
	// detect if it's a http client
	struct timeval tv;
	tv.tv_usec = 0;
	tv.tv_sec = 2;
	setsockopt(NetLink->sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
	while (!NetLink->Size)
		NetLink->Size = recv(NetLink->sock, NetLink->Head, 6, 0);
	tv.tv_sec = 0;
	setsockopt(NetLink->sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
#endif
	// if recv has too long (1 seconde), well it's a tera client
	if (NetLink->Size < 0) {
		send(NetLink->sock, (const char[4]){1, 0, 0, 0}, 4, 0);
		Player_Login(NetLink);
	}
#ifndef NO_HTTP
	else {
		if (NetLink->Size > 5 && (!memcmp(NetLink->Head, "GET ", 4) || !memcmp(NetLink->Head, "POST ", 5) || !memcmp(NetLink->Head, "HEAD ", 5) || !memcmp(NetLink->Head, "TRACE ", 6))) {
			NetLink->Size += recv(NetLink->sock, NetLink->Head+NetLink->Size, 512-NetLink->Size, 0);
			HTTP_Work(NetLink);
		} else {
			// unsupported connexion
DEBUG("%s (%i)\n", __func__, __LINE__);
			NetLink_close(NetLink);
DEBUG("%s (%i)\n", __func__, __LINE__);
			free(NetLink);
DEBUG("%s (%i)\n", __func__, __LINE__);
			c_THREAD_free(NULL);
DEBUG("%s (%i)\n", __func__, __LINE__);
		}
	}
#endif
}

void Network_Run()
{
	net_run = true;
	net_running = true;
	Test_Thread = CreateThread(NULL, 0, (void*)Network_Test, (void*)1, 0, NULL);
	while (net_run) {
		struct sockaddr_in csin = { 0 };
		int csin_s = sizeof(csin);
		if (sock >= 0) {
			SOCKET csock = accept(sock, (struct sockaddr *)&csin, &csin_s);
			if(csock == -1) {
#ifdef WIN32
				DEBUG("%s (%i) :: Client connexion Error (%d %d)\n", __func__, __LINE__, WSAGetLastError(), csock);
#else
				DEBUG("%s (%i) :: Client connexion Error (%d %d)\n", __func__, __LINE__, errno, csock);
#endif
				if(errno == 22 && csock == -1)
					if (!Test_Thread) Network_Kill();
			} else {
				netlink_t *NetLink = malloc(sizeof(netlink_t));
				NetLink->C_Thread = NULL;
				NetLink->Size = 0;
				NetLink->sock = csock;
				char *tmpIP = inet_ntoa(csin.sin_addr);
				strcpy(NetLink->IP, tmpIP);
				NetLink->C_Thread = CreateThread(NULL, 0, (void*)Network_Detect, NetLink, 0, NULL);
			}
		}
	}
	net_running = false;
}

void Network_Close()
{
	net_run = false;
	if (sock >= 0)
		closesocket(sock);
	sock = -1;
#ifdef WIN32
	WSACleanup();
#endif
	DEBUG("%s (%i) :: Network Stopped.\n", __func__, __LINE__);
}
