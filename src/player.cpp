#include "Sharun.hpp"

player::player(uint id)
{
	Account_Id = id;
	Character_Id = 0;
	SUBID = 1000;
	Class_Id = 0;
	Race_Id = 0;
	Sex = 0;
	Level = 1;
	exp = 0;
	money = 0;
	memset(collect, 0, 3);
	memset(name, 0, sizeof(char16_t)*51);
	memset(title, 0, sizeof(char16_t)*51);
	loged = 0;
	exit_type = 0;
	exit_time = 0;
	Right = 0;
	end_pay = 0;
	OpCode_GID = -1;
}

player*	new_player(uint id)
{
	return new player(id);
}
