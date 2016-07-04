#ifndef _CONNEXION_LIST_HPP_
#define _CONNEXION_LIST_HPP_

#include <pthread.h>

void CloseSocket(SOCKET*);

enum port_type {
	PORT_TYPE_IN_GAME = 0,
	PORT_TYPE_IN_HTTPD,
	PORT_TYPE_IN_BRIDGE,
	PORT_TYPE_OUT,
};

class connexion_list {
	protected:
		port_type p_t;
		void*	teraCrypt;
		bool	teraCrypt_Client;
		pthread_mutex_t	teraCrypt_Mtx;
		void	Encode(packet*);
		void	Decode(packet*, bool);

	public:
		connexion_list(SOCKET, port_type p_t_t);
		virtual ~connexion_list();
		SOCKET	sock;
		void*	mother;
		connexion_list* next;
		connexion_list* prev;
		void	Key(const char*, bool);
		void	Set_timeout(ushort);
		virtual int Send(packet*);	// Auto-delete packet !
		virtual int Send(const char*, int);
		virtual int Send(const char*);
		virtual int Recv(packet*);
		virtual int Recv(char*, int);
		virtual int Recv(char*, int*);
};

void connexions_cleanup();

#endif // _CONNEXION_LIST_HPP_
