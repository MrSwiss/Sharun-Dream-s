#include "internal/network.hpp"
#include "Sharun.hpp"
#include "internal/thread.hpp"

#include <cerrno>
#include <csignal>

void HTTP_Thread(thread_list*);

network::network()
{
#ifdef WIN32
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2,0), &WSAData);
#else
	signal(SIGPIPE, SIG_IGN);
#endif
}

network::~network()
{
	stop();
#ifdef WIN32
	WSACleanup();
#endif
}

int network::start()
{
	int count = 0;
	for (int i=0; i<3; i++) {
		SOCKET sock = socket(AF_INETX, SOCK_STREAM, IPPROTO_TCP);
		if (sock == INVALID_SOCKET) {
#ifdef WIN32
			DEBUG("%s (%i) :: Could not create socket (%d)\n", __FILE__, __LINE__, WSAGetLastError());
#else
			DEBUG("%s (%i) :: Could not create socket (%d)\n", __FILE__, __LINE__, errno);
#endif
			continue;
		}

		struct sockaddr_in sa;
		sa.sin_family = AF_INET;
		ushort port = 0;
		switch (i) {
			case 2 :
				port = Settings->Net.Ports.Bridge;
				break;
			case 1 :
				port = Settings->Net.Ports.Httpd;
				if (port == Settings->Net.Ports.Game)
					port = 0;
				break;
			default :
				port = Settings->Net.Ports.Game;
		}
		if (!port) {
			CloseSocket(&sock);
			continue;
		}
		sa.sin_port = htons(port);
		if (Settings->Net.localhost)
			sa.sin_addr.s_addr = inet_addr("127.0.0.1");
		else
			sa.sin_addr.s_addr = INADDR_ANY;

		int sockopt = 1;
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&sockopt, sizeof(int));

		if (bind(sock, (SOCKADDR *)&sa, sizeof(sa)) == SOCKET_ERROR) {
#ifdef WIN32
			DEBUG("%s (%i) :: Could not bind socket (%i - %d)\n", __FILE__, __LINE__, port, WSAGetLastError());
#else
			DEBUG("%s (%i) :: Could not bind socket (%i - %d)\n", __FILE__, __LINE__, port, errno);
#endif
			CloseSocket(&sock);
			continue;
		}

		if (listen(sock, 100) == SOCKET_ERROR) {
#ifdef WIN32
			DEBUG("%s (%i) :: Could not listen socket (%i - %d)\n", __FILE__, __LINE__, port, WSAGetLastError());
#else
			DEBUG("%s (%i) :: Could not listen socket (%i - %d)\n", __FILE__, __LINE__, port, errno);
#endif
			CloseSocket(&sock);
			continue;
		}

		count++;
		new connexion_list(sock, (port_type)i);
		DEBUG("%s (%i) :: Network Started (%s : %i).\n", __FILE__, __LINE__, Settings->Net.localhost ? "localhost" : "ANY", port);
	}
	for (int i=0; count && Settings->Thread.Httpd > 1 && i < Settings->Thread.Httpd; i++) {
		thread_list *thread = new thread_list();
		if (!thread->start((void*)HTTP_Thread, thread))
			DEBUG("%s (%i) :: Could not start Httpd thread %i !\n", __FILE__, __LINE__, i);
		else
			DEBUG("%s (%i) :: Httpd thread %i started.\n", __FILE__, __LINE__, i);
	}
	return count;
}

void network::stop()
{
	connexions_cleanup();
	DEBUG("%s (%i) :: Network Stoped.\n", __FILE__, __LINE__);
}
