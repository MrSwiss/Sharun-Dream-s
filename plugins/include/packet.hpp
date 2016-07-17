#ifndef _PACKET_HPP_
#define _PACKET_HPP_

#include "data_type.hpp"

#define PACKET_MAX_SIZE 0xffff

class packet
{
	protected:
		ushort	pos_switch;
		ushort	size_max;
		void	write_len(ushort);

	public:
		packet();
		virtual ~packet();
		ushort	id;	// OpCode ID
		ushort	size;
		ushort	pos;
		byte*	raw;

		virtual int	seek(int, int);
		virtual bool	eof();

		virtual bool	swap();
		virtual bool	swap(ushort);
		virtual packet*	clone();
		virtual ushort	full_size();
		virtual void	resize(int);
		virtual void	get_header();
		virtual void	set_header(int);

		virtual ushort	write(void*, ushort);
		virtual ushort	write_C(char*);
		virtual ushort	write_S(char16_t*);
		virtual ushort	write_1(uchar);
		virtual ushort	write_2(ushort);
		virtual ushort	write_4(uint);
		virtual ushort	write_8(ulong);
		virtual ushort	write_F(float);

		virtual ushort	read(void*, ushort);
		virtual char*	read_C(char*);
		virtual char16_t*	read_S(char16_t*);
		virtual uchar	read_1();
		virtual ushort	read_2();
		virtual uint	read_4();
		virtual ulong	read_8();
		virtual float	read_F();
};

#ifdef DLL_VERSION
extern packet*	(*new_packet)();
#else
extern "C" {
packet*	new_packet();
}
#endif // DLL_VERSION

#endif // _PACKET_HPP_
