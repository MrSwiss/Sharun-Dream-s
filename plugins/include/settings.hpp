#ifndef _SETTINGS_HPP_
#define _SETTINGS_HPP_

class settings {
	public:
		settings();
		~settings();
		char*	Server_Name;
		char	Start_Time[20];
		char*	SERVER_SIGN;
		char*	SERVER_COPYRIGHT;
		ushort	Start_Count;
		ushort	PLAYER_COUNT;
		ushort	Player_Max;
		bool	Packets_Fill_Size;
		bool	AutoAccount;
		bool	newHP;
		bool	Stamina4Speed;
		bool	main_run;
		struct Dirs {
			std::string Saves;
			std::string Logs;
			std::string WWW;
			std::string OpCodes;
			std::string Plugins;
			std::string Guild_Logo;
			struct Settings {
				std::string Account;
				std::string Chat;
				std::string User;
			} Settings;
		} Dirs;
		struct Thread {
			ushort	Broadcast;
			ushort	Httpd;
			struct DataBase {
				ushort Fast;
				ushort Slow;
			} DataBase;
		} Thread;
		struct Net {
			bool	localhost;
			struct Ports {
				ushort	Game;
				ushort	Httpd;
				ushort	Bridge;
			} Ports;
		} Net;
};

#endif // _SETTINGS_HPP_
