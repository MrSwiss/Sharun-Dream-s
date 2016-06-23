#include "internal/network.hpp"
#include "Sharun.hpp"
#include "internal/thread.hpp"
#include "httpd_link.hpp"
#include "internal/TeraCrypt.hpp"

#include <cstring>
#include <cerrno>
#include <unistd.h>

void CloseSocket(SOCKET *sock)
{
	if (*sock >= 0) {
#ifdef WIN32
		closesocket(*sock);
#else
		shutdown(*sock, SHUT_RDWR);
		close(*sock);
#endif
	}
	*sock = INVALID_SOCKET;
}

void HttpD(connexion_list*);
void GameD(connexion_list*);

connexion_list* connexions = NULL;
connexion_list* connexions_last = NULL;
pthread_mutex_t connexions_mtx;
bool connexions_mtx_init = false;

void connexions_cleanup()
{
	if (connexions_mtx_init) {
		pthread_mutex_lock(&connexions_mtx);
		while (connexions) {
			pthread_mutex_unlock(&connexions_mtx);
			delete connexions;
			pthread_mutex_lock(&connexions_mtx);
		}
		pthread_mutex_unlock(&connexions_mtx);
		pthread_mutex_destroy(&connexions_mtx);
	}
	connexions_mtx_init = false;
}

connexion_list::connexion_list(SOCKET in, port_type p_t_t)
{
	if (!connexions_mtx_init) {
		connexions_mtx_init = true;
		pthread_mutex_init(&connexions_mtx, NULL);
	}
	p_t = p_t_t;
	sock = in;
	next = NULL;
	prev = NULL;
	teraCrypt = NULL;
	teraCrypt_Client = false;
	pthread_mutex_init(&teraCrypt_Mtx, NULL);
	pthread_mutex_lock(&connexions_mtx);
	if (!connexions)
		connexions = connexions_last = this;
	else {
		connexions_last->next = this;
		prev = connexions_last;
		connexions_last = this;
	}
	pthread_mutex_unlock(&connexions_mtx);
	thread_list *thread;
	
	switch ((short)p_t) {
		case PORT_TYPE_IN_HTTPD:
			mother = thread = new thread_list();
			if (!thread->start((void*)HttpD, this))
				delete this;
			break;
		case PORT_TYPE_IN_GAME:
			mother = thread = new thread_list();
			if (!thread->start((void*)GameD, this))
				delete this;
			break;
	}
}

connexion_list::~connexion_list()
{
	CloseSocket(&sock);
	pthread_mutex_lock(&connexions_mtx);
	if (prev)
		prev->next = next;
	else
		connexions = next;
	if (next)
		next->prev = prev;
	pthread_mutex_unlock(&connexions_mtx);
	if (teraCrypt) {
		Crypt::Session *Sess = (Crypt::Session *)teraCrypt;
		delete Sess;
	}
	pthread_mutex_destroy(&teraCrypt_Mtx);
}

void connexion_list::Key(byte *keys, bool i_m_client)
{
	teraCrypt_Client = i_m_client;
	if (!teraCrypt_Client)
		teraCrypt = new Crypt::Session(&keys[0*128], &keys[1*128], &keys[2*128], &keys[3*128]);
	else
		teraCrypt = new Crypt::Session(&keys[2*128], &keys[3*128], &keys[0*128], &keys[1*128]);
}

void connexion_list::Encode(packet *packet_l)
{
	if (!teraCrypt)
		return;
	Crypt::Session *Sess = (Crypt::Session *)teraCrypt;
	if (!teraCrypt_Client)
		Sess->Encrypt(packet_l->raw, packet_l->size);
	else
		Sess->Decrypt(packet_l->raw, packet_l->size);
}

void connexion_list::Decode(packet *packet_l, bool head)
{
	if (!teraCrypt)
		return;
	Crypt::Session *Sess = (Crypt::Session *)teraCrypt;
	if (!teraCrypt_Client)
		Sess->Decrypt(packet_l->raw + (head ? 0 : 4), (head ? 4: packet_l->size - 4));
	else
		Sess->Encrypt(packet_l->raw + (head ? 0 : 4), (head ? 4: packet_l->size - 4));
}

int connexion_list::Send(packet *packet_l)
{
	pthread_mutex_lock(&teraCrypt_Mtx);
	Encode(packet_l);
	int len = Send((char*)packet_l->raw, packet_l->size);
	pthread_mutex_unlock(&teraCrypt_Mtx);
	return len;
}

int connexion_list::Send(const char *data, int len)
{
	if (sock == INVALID_SOCKET)
		return -1;
	int ret = send(sock, data, len, 0);
	if (ret == SOCKET_ERROR) {
		CloseSocket(&sock);
		ret = -1;
	}
	return ret;
}

int connexion_list::Send(const char *data)
{
	return Send(data, strlen(data));
}

int connexion_list::Recv(packet *packet_l)
{
	pthread_mutex_lock(&teraCrypt_Mtx);
	int len = Recv((char*)packet_l->raw, 4);
	Decode(packet_l, true);
	packet_l->resize(packet_l->size);
	do
		len += Recv((char*)&packet_l->raw[len], packet_l->size - len);
	while (packet_l->size > len);
	Decode(packet_l, false);
	pthread_mutex_unlock(&teraCrypt_Mtx);
	return len;
}

int connexion_list::Recv(char *data, int len)
{
	if (sock == INVALID_SOCKET)
		return -1;
	int ret = recv(sock, data, len, 0);
	if (ret == SOCKET_ERROR) {
		CloseSocket(&sock);
		ret = -1;
	}
	return ret;
}

int connexion_list::Recv(char *data, int *len)
{
	*len = Recv(data, *len);
	return *len;
}

void HttpD(connexion_list *connex)
{
	thread_list *thread = (thread_list*)connex->mother;
	while (connex->sock != INVALID_SOCKET) {
		struct sockaddr_in csin = { 0 };
		socklen_t csin_s = sizeof(csin);
		SOCKET csock = accept(connex->sock, (struct sockaddr *)&csin, &csin_s);
		if(csock == INVALID_SOCKET) {
			if (connex->sock != INVALID_SOCKET)
#ifdef WIN32
				DEBUG("%s (%i) :: Client (HttpD) connexion Error (%d %d)\n", __func__, __LINE__, WSAGetLastError(), csock);
#else
				DEBUG("%s (%i) :: Client (HttpD) connexion Error (%d %d)\n", __func__, __LINE__, errno, csock);
#endif
		} else {
			httpd_link_t *httpd_link = new httpd_link_t;
			httpd_link->noHead = false;
			httpd_link->connex = new connexion_list(csock, PORT_TYPE_HTTPD);
			httpd_link->Size = httpd_link->connex->Recv(httpd_link->Head, httpd_link_Head_Size);
			if (Settings->Thread.Httpd > 1)
				HTTP_Add(httpd_link);
			else
				HTTP_Work(httpd_link);
		}
	}
	thread->internal_delete();
	DEBUG("%s (%i) :: Network Stopped (%s).\n", __FILE__, __LINE__, Settings->Net.localhost ? "localhost" : "ANY");
	pthread_exit(NULL);
}

void GameD(connexion_list *connex)
{
	thread_list *thread = (thread_list*)connex->mother;
	while (connex->sock != INVALID_SOCKET) {
		struct sockaddr_in csin = { 0 };
		socklen_t csin_s = sizeof(csin);
		SOCKET csock = accept(connex->sock, (struct sockaddr *)&csin, &csin_s);
		if(csock == INVALID_SOCKET) {
			if (connex->sock != INVALID_SOCKET)
#ifdef WIN32
				DEBUG("%s (%i) :: Client (GameD) connexion Error (%d %d)\n", __func__, __LINE__, WSAGetLastError(), csock);
#else
				DEBUG("%s (%i) :: Client (GameD) connexion Error (%d %d)\n", __func__, __LINE__, errno, csock);
#endif
		} else {
			
			httpd_link_t *httpd_link = new httpd_link_t;
			httpd_link->noHead = false;
			httpd_link->connex = new connexion_list(csock, PORT_TYPE_HTTPD);
			httpd_link->Size = httpd_link->connex->Recv(httpd_link->Head, httpd_link_Head_Size);
			if (Settings->Thread.Httpd > 1)
				HTTP_Add(httpd_link);
			else
				HTTP_Work(httpd_link);
		}
	}
	thread->internal_delete();
	DEBUG("%s (%i) :: Network Stopped (%s).\n", __FILE__, __LINE__, Settings->Net.localhost ? "localhost" : "ANY");
	pthread_exit(NULL);
}
