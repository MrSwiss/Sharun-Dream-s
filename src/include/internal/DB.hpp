#ifndef _DB_SETTING_HPP_
#define _DB_SETTING_HPP_

#include "internal/Fixed_Thread.hpp"
#include "internal/pthread_sub.hpp"

#include <mysql.h>

typedef struct database_setting {
	char	IP[8*4+7+1];
	ushort	Port;
	char*	Login;
	char*	Pass;
	char*	Name;
	std::string SQLite;
	std::string RAW_Files;
} database_setting;

extern database_setting DB_Set;

class SQL_QUEUE {
	protected:
		pthread_pause_t *resume;

	public:
		SQL_QUEUE(char *req, bool fast);
		~SQL_QUEUE();
		bool	is_Fast();
		void	wait();
		void	signal();
		bool	responce;
		bool	stored;
		char*	Query;
		void*	result;
};

void	MySQL_Add(SQL_QUEUE *SQL_Comm);
void	MySQL_Cleanup();
bool	MySQL_Init(MYSQL **conn);
void	MySQL_Close(MYSQL **conn);
void*	MySQL_Work(MYSQL *conn, char *Query, bool fast, bool stored, bool responce);
void	MySQL_Thread(thread_list *my_thread);

#endif // _DB_SETTING_HPP_
