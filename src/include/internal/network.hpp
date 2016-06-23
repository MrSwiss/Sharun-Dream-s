#ifndef _NETWORK_HPP_
#define _NETWORK_HPP_

#include "psocket.hpp"

class network {
	public:
		network();
		virtual ~network();
		int start();
		void stop();
};

extern network *Network;

#endif // _NETWORK_HPP_
