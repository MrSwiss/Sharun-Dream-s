#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

class player {
	protected:
		uint	Account_Id;

	public:
		player(uint);
		~player();
		uint	Character_Id;
		uint	SUBID;
		uint	Class_Id;
		uint	Race_Id;
		uint	Sex;
		uint	Level;
		ulong	exp;
		ulong	money;
		uchar	collect[3];
		char16_t	name[51];
		char16_t	title[51];
		long	loged;
		uchar	exit_type;
		long	exit_time;
		ushort	Right;
		ulong	end_pay;
		short	OpCode_GID;
};

#endif // _PLAYER_HPP_