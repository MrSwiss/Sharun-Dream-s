#include "internal/network.hpp"
#include "Sharun.hpp"
#include "internal/thread.hpp"
#include "httpd_link.hpp"
#include "internal/TeraCrypt.hpp"

#include <cstring>
#include <cerrno>
#include <unistd.h>

#include <uchar.h>
#define _O_U16TEXT 0x20000

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

void connexion_list::Key(const char *keys, bool i_m_client)
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

void connexion_list::Set_timeout(ushort sec)
{
	struct timeval tv;
	tv.tv_usec = 0;
	tv.tv_sec = sec;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
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
	packet_l->get_header();
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
			httpd_link->connex = new connexion_list(csock, PORT_TYPE_OUT);
			httpd_link->Size = httpd_link->connex->Recv(httpd_link->Head, httpd_link_Head_Size);
			if (Sharun->Settings.Thread.Httpd > 1)
				HttpD_Add(httpd_link);
			else
				HTTP_Work(httpd_link);
		}
	}
	thread->internal_delete();
	DEBUG("%s (%i) :: Network Stopped (%s).\n", __FILE__, __LINE__, Sharun->Settings.Net.localhost ? "localhost" : "ANY");
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
			httpd_link_t *httpd_link = NULL;
			connexion_list *connex_t = new connexion_list(csock, PORT_TYPE_OUT);
DEBUG("%s (%i) :: Someone connected\n", __func__, __LINE__);
			if (Sharun->Settings.Net.Ports.Game == Sharun->Settings.Net.Ports.Httpd) {

DEBUG("%s (%i) :: Detect a browser...\n", __func__, __LINE__);
				httpd_link = new httpd_link_t;
				httpd_link->noHead = false;
				httpd_link->connex = connex_t;
				connex_t->Set_timeout(2);
				httpd_link->Size = httpd_link->connex->Recv(httpd_link->Head, httpd_link_Head_Size);
				connex_t->Set_timeout(0);
				if (httpd_link->Size > 0) {
					if (Sharun->Settings.Thread.Httpd > 1)
						HttpD_Add(httpd_link);
					else
						HTTP_Work(httpd_link);
					continue;
				}
			}
			if (httpd_link)
				delete httpd_link;
			connex_t->Send((const char[4]){0x01, 0x00, 0x00, 0x00}, 4);
			char *Key_l = new char[4*128];
			for (int i=0; i<2; i++)
				for (int j=0; j<128; j++)
					Key_l[(2+i)*128+j] = rand() % 0xff;
			bool rrun = true;
			for (int i=0; i<2 && rrun; i++) {
				int ret = connex_t->Recv(&Key_l[(0+i)*128], 128);
	hexdump(__func__, "Client Key", &Key_l[(0+i)*128], 128);
				if (ret != 128) {
					DEBUG("%s (%i) :: Client Key %i ! (%i<128)\n", __func__, __LINE__, i+1, ret);
					rrun = false;
					delete connex_t;
					delete Key_l;
					continue;
				}
				if (rrun) {
					ret = connex_t->Send(&Key_l[(2+i)*128], 128);
	hexdump(__func__, "Server Key", &Key_l[(2+i)*128], 128);
				}
			if (rrun  && ret != 128) {
					DEBUG("%s (%i) :: Server Key %i ! (%i<128)\n", __func__, __LINE__, i+1, ret);
					rrun = false;
					delete connex_t;
					delete Key_l;
					continue;
				}
			}
			connex_t->Key(Key_l, false);
			delete Key_l;
			packet *packet_l = new packet();
			connex_t->Recv(packet_l);
	hexdump(__func__, "Packet", packet_l->raw, packet_l->size);
			delete packet_l;
			packet_l = new packet();
			connex_t->Recv(packet_l);
	hexdump(__func__, "Packet", packet_l->raw, packet_l->size);
	hexdump(__func__, "Packet + 23", packet_l->raw + 23, packet_l->size - 23);
			char16_t *Name = (char16_t*)&packet_l->raw[23];
char *tmp = str_str(Name);
printf("alpha is:\n\t%s\n", tmp);
printf("strcmp16 is:\n\t%i\n", strcmp16(Name, u"clofriwen"));
free(tmp);
		}
	}
	thread->internal_delete();
	DEBUG("%s (%i) :: Network Stopped (%s).\n", __FILE__, __LINE__, Sharun->Settings.Net.localhost ? "localhost" : "ANY");
	pthread_exit(NULL);
}
