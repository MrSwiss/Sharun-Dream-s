#include "SQL_int.h"
#include <mysql.h>
#include <dlfcn.h>

#ifndef WIN32
	#define HANDLE void*
	#define DWORD void*
#endif

MYSQL **conn = NULL;
void  **MYSQL_Mutex = NULL;
short MySQL_Fast_Thread = 0;
short MySQL_Slow_Thread = 0;
HANDLE *MySQL_Thread_id = NULL;
bool MySQL_Thread_Run = false;

SQL_QUEUE *MySQL_Slow = NULL;
SQL_QUEUE *MySQL_Slow_l = NULL;
SQL_QUEUE *MySQL_Fast = NULL;
SQL_QUEUE *MySQL_Fast_l = NULL;

const char *	libmysql_dll_list[] = { "%s/libmysql.dll", "libmysql.dll", NULL};

void *		libmysql_dll = NULL;
void		(*l_mysql_close)(MYSQL *sock) = NULL;
void		(*l_mysql_data_seek)(MYSQL_RES *result, my_ulonglong offset) = NULL;
const char *	(*l_mysql_error)(MYSQL *mysql) = NULL;
MYSQL_ROW	(*l_mysql_fetch_row)(MYSQL_RES *result) = NULL;
void		(*l_mysql_free_result)(MYSQL_RES *result) = NULL;
MYSQL *		(*l_mysql_init)(MYSQL *mysql) = NULL;
my_ulonglong	(*l_mysql_num_rows)(MYSQL_RES *res) = NULL;
int		(*l_mysql_options)(MYSQL *mysql, enum mysql_option option, const void *arg) = NULL;
int		(*l_mysql_query)(MYSQL *mysql, const char *q) = NULL;
MYSQL *		(*l_mysql_real_connect)(MYSQL *mysql, const char *host,
					   const char *user,
					   const char *passwd,
					   const char *db,
					   const unsigned int port,
					   const char *unix_socket,
					   unsigned long clientflag) = NULL;
MYSQL_ROW_OFFSET (*l_mysql_row_seek)(MYSQL_RES *result, MYSQL_ROW_OFFSET offset) = NULL;
int		(*l_mysql_set_character_set)(MYSQL *mysql, const char *csname) = NULL;
MYSQL_RES *     (*l_mysql_store_result)(MYSQL *mysql) = NULL;
MYSQL_RES *     (*l_mysql_use_result)(MYSQL *mysql) = NULL;

void MySQL_Thread_API(ushort *MT_id)
{
	MYSQL_RES *res;
	bool Fast;
	SQL_QUEUE *sql_q;
	short thread_id = *MT_id;
	free(MT_id);
	Fast = true;
	if (thread_id >= MySQL_Fast_Thread) Fast = false;
	DEBUG("%s (%i) :: %s %i Started.\n", __func__, __LINE__, Fast ? "Faster" : "Slower", Fast ? thread_id : thread_id-MySQL_Fast_Thread);

	while (MySQL_Thread_Run) {
		sql_q = NULL;
		Fast = true;
		if (thread_id >= MySQL_Fast_Thread) Fast = false;
		if (Fast) {
			SQL_Mutex_Lock(Fast);
			sql_q = MySQL_Fast;
			if (sql_q) MySQL_Fast = sql_q->Next;
			SQL_Mutex_Unlock(Fast);
		}
		if (!Fast || !sql_q) {
			Fast = false;
			SQL_Mutex_Lock(Fast);
			sql_q = MySQL_Slow;
			if (sql_q) MySQL_Slow = sql_q->Next;
			SQL_Mutex_Unlock(Fast);
		}
		if (sql_q) {
			l_mysql_query(conn[thread_id], sql_q->Query);
			if (l_mysql_error(conn[thread_id])[0])
				DEBUG("%s (%i) :: %s : \"%s\"\n", __func__, __LINE__, l_mysql_error(conn[thread_id]), sql_q->Query);
			if (!Fast || sql_q->result == 2)
				res = l_mysql_use_result(conn[thread_id]);
			else
				res = l_mysql_store_result(conn[thread_id]);
			if (!Fast) {
				if (sql_q->Query) free(sql_q->Query);
				free(sql_q);
				if (res) l_mysql_free_result(res);
			} else {
				if (sql_q->result)
					sql_q->Response = res;
				else
					if (res) l_mysql_free_result(res);
			}
			if (sql_q && sql_q->finished) 
				Mutex_Unlock(sql_q->finished);
		} else Mutex_Pause(MYSQL_Mutex[thread_id]);
	}
	Fast = true;
	if (thread_id >= MySQL_Fast_Thread) Fast = false;
	DEBUG("%s (%i) :: %s %i Stopped.\n", __func__, __LINE__, Fast ? "Faster" : "Slower", Fast ? thread_id : thread_id-MySQL_Fast_Thread);
	c_THREAD_free(NULL);
}

int MySQL_Init(const char *host, const ushort port, const char *name, const char *pass, const char *db, short thr, short sthr)
{
	ushort MySQL_Fast_Thread_count = 0;
	MySQL_Fast_Thread = thr;
	MySQL_Slow_Thread = sthr;
#ifndef WIN32
	l_mysql_close = mysql_close;
	l_mysql_data_seek = mysql_data_seek;
	l_mysql_error = mysql_error;
	l_mysql_fetch_row = mysql_fetch_row;
	l_mysql_free_result = mysql_free_result;
	l_mysql_init = mysql_init;
	l_mysql_num_rows = mysql_num_rows;
	l_mysql_options = mysql_options;
	l_mysql_query = mysql_query;
	l_mysql_real_connect = mysql_real_connect;
	l_mysql_row_seek = mysql_row_seek;
	l_mysql_set_character_set = mysql_set_character_set;
	l_mysql_store_result = mysql_store_result;
	l_mysql_use_result = mysql_use_result;
#else
	char File[strlen(Cfg_Server_DirSave)+13+1];
	l_mysql_close = NULL;
	for (short i=0; !l_mysql_close && libmysql_dll_list[i]; i++) {
		sprintf(File, libmysql_dll_list[i], Cfg_Server_DirSave);
		DEBUG("%s (%i) :: Try to open \"%s\" detected.\n", __func__, __LINE__, File);
		libmysql_dll = dlopen(File, RTLD_LAZY);
		if (libmysql_dll) {
			DEBUG("%s (%i) :: \"libmysql.dll\" detected.\n", __func__, __LINE__);
			l_mysql_close = dlsym(libmysql_dll, "mysql_close");
			if (!l_mysql_close) {
				DEBUG("%s (%i) :: Bad \"libmysql.dll\" detected !\n", __func__, __LINE__);
				dlclose(libmysql_dll);
				libmysql_dll = NULL;
			}
		} else
			DEBUG("%s (%i) :: No \"%s\" detected !\n", __func__, __LINE__, File);
	}
	if (l_mysql_close) {
		l_mysql_data_seek = dlsym(libmysql_dll, "mysql_data_seek");
		l_mysql_error = dlsym(libmysql_dll, "mysql_error");
		l_mysql_fetch_row = dlsym(libmysql_dll, "mysql_fetch_row");
		l_mysql_free_result = dlsym(libmysql_dll, "mysql_free_result");
		l_mysql_init = dlsym(libmysql_dll, "mysql_init");
		l_mysql_num_rows = dlsym(libmysql_dll, "mysql_num_rows");
		l_mysql_options = dlsym(libmysql_dll, "mysql_options");
		l_mysql_query = dlsym(libmysql_dll, "mysql_query");
		l_mysql_real_connect = dlsym(libmysql_dll, "mysql_real_connect");
		l_mysql_row_seek = dlsym(libmysql_dll, "mysql_row_seek");
		l_mysql_set_character_set = dlsym(libmysql_dll, "mysql_set_character_set");
		l_mysql_store_result = dlsym(libmysql_dll, "mysql_store_result");
		l_mysql_use_result = dlsym(libmysql_dll, "mysql_use_result");
	} else {
		DEBUG("%s (%i) :: No \"libmysql.dll\" detected !\n", __func__, __LINE__);
		return 0;
	}
	// If we are a win32 exe, we will use libmysql like a plugin and can't have more of 1 mysql connexion !
	MySQL_Fast_Thread = 1;
	MySQL_Slow_Thread = 0;
#endif

    if (SQL_Mutex_Init()) return 1;
	MySQL_Slow = NULL;
	MySQL_Fast = NULL;
	MySQL_Thread_Run = true;
	MySQL_Thread_id = malloc(sizeof(HANDLE)*(MySQL_Fast_Thread+MySQL_Slow_Thread));
	MYSQL_Mutex = malloc(sizeof(void *)*(MySQL_Fast_Thread+MySQL_Slow_Thread));
	conn = malloc(sizeof(MYSQL *)*(MySQL_Fast_Thread+MySQL_Slow_Thread));
	for (ushort i=0 ; i < MySQL_Fast_Thread+MySQL_Slow_Thread ; i++) {
		MySQL_Thread_id[i] = NULL;
		MYSQL_Mutex[i] = Mutex_Init();
		conn[i] = l_mysql_init(NULL);
		if (!conn[i])
			DEBUG("%s (%i) :: can't init ! %s %i\n", __func__, __LINE__, i < MySQL_Fast_Thread ? "Faster" : "Slower", i < MySQL_Fast_Thread ? i : i-MySQL_Fast_Thread);

DEBUG("%s (%i) :: Ht:%s Nm:%s Ps:%s Db:%s Pt:%i - %s %i\n", __func__, __LINE__, host, name, pass, db, port, i < MySQL_Fast_Thread ? "Faster" : "Slower", i < MySQL_Fast_Thread ? i : i-MySQL_Fast_Thread);
		if (conn[i] && !l_mysql_real_connect(conn[i], host, name, pass,
				db, port, NULL, CLIENT_SSL)) {
			DEBUG("%s (%i) :: %s - %s %i\n", __func__, __LINE__, l_mysql_error(conn[i]), i < MySQL_Fast_Thread ? "Faster" : "Slower", i < MySQL_Fast_Thread ? i : i-MySQL_Fast_Thread);
			/* Crash here if can't connect
			l_mysql_close(conn[i]);*/
			DEBUG("%s (%i) :: %s %i\n", __func__, __LINE__, i < MySQL_Fast_Thread ? "Faster" : "Slower", i < MySQL_Fast_Thread ? i : i-MySQL_Fast_Thread);
			conn[i] = NULL;
		} else {


l_mysql_query(conn[i], "SELECT COUNT(*) FROM `characters`");
if (l_mysql_error(conn[i])[0])
	DEBUG("%s (%i) :: %s : \"%s\"\n", __func__, __LINE__, l_mysql_error(conn[i]), "SELECT COUNT(*) FROM `characters`");
void *res = l_mysql_use_result(conn[i]);
char **row = l_mysql_fetch_row(res);
	DEBUG("%s (%i) :: \"%s\"\n", __func__, __LINE__, row[0]);
l_mysql_free_result(res);

			bool reconnect = true;
			l_mysql_options(conn[i], MYSQL_OPT_RECONNECT, &reconnect);
			if (l_mysql_set_character_set(conn[i], "utf8"))
				DEBUG("%s (%i) :: %s %s %i\n", __func__, __LINE__, l_mysql_error(conn[i]), i < MySQL_Fast_Thread ? "Faster" : "Slower", i < MySQL_Fast_Thread ? i : i-MySQL_Fast_Thread);

	DEBUG("%s (%i)\n", __func__, __LINE__);

			ushort *MT_id = malloc(sizeof(ushort));
			*MT_id = i;
			MySQL_Thread_id[i] = CreateThread(NULL, 0, (void*)MySQL_Thread_API, MT_id, 0, NULL);
			if (i < MySQL_Fast_Thread) MySQL_Fast_Thread_count++;
			usleep(1);
		}
	}

	DEBUG("%s (%i) :: MySQL Started.\n", __func__, __LINE__);
	return MySQL_Fast_Thread_count;
}

void MySQL_Run_Thread(void *argv)
{
	for (ushort i=0; i<MySQL_Fast_Thread; i++)
		Mutex_Resume(MYSQL_Mutex[MySQL_Slow_Thread+i]);
	c_THREAD_free(NULL);
}

char ***MySQL_Query_Fast(char *Query, char result)
{
	SQL_QUEUE *sql_q = SQL_NewQueue(Query);
	sql_q->result = result;
	sql_q->finished = Mutex_Init();
	SQL_Mutex_Lock(true);
	if (!MySQL_Fast) MySQL_Fast = sql_q;
	else {
		MySQL_Fast_l->Next = sql_q;
	}
	MySQL_Fast_l = sql_q;
	SQL_Mutex_Unlock(true);
	HANDLE *MySQL_Run = CreateThread(NULL, 0, (void*)MySQL_Run_Thread, NULL, 0, NULL);
	Mutex_SPause(sql_q->finished);
	Mutex_Close(sql_q->finished);
	CloseHandle(MySQL_Run);
	char ***Response = sql_q->Response; 
	free(sql_q);
	return Response;
}

void MySQL_Query_Slow(char *Query)
{
	SQL_QUEUE *sql_q = SQL_NewQueue(Query);
	SQL_Mutex_Lock(false);
	if (!MySQL_Slow) MySQL_Slow = sql_q;
	else {
		MySQL_Slow_l->Next = sql_q;
	}
	MySQL_Slow_l = sql_q;
	SQL_Mutex_Unlock(false);
	for (ushort i=0; i<MySQL_Slow_Thread; i++)
		Mutex_Resume(MYSQL_Mutex[i]);
}

void MySQL_Close()
{
	while (MySQL_Slow) sleep(1);
	while (MySQL_Fast) sleep(1);
	MySQL_Thread_Run = false;
	for (ushort i=0; i<MySQL_Slow_Thread+MySQL_Fast_Thread ; i++)
		Mutex_Resume(MYSQL_Mutex[i]);
	if (MySQL_Thread_id) WaitForMultipleObjects(2, MySQL_Thread_id, TRUE, INFINITE);
	for (int i=0 ; i < MySQL_Fast_Thread+MySQL_Slow_Thread; i++) {
		CloseHandle(MySQL_Thread_id[i]);
		MySQL_Thread_id[i] = NULL;
		if (MYSQL_Mutex[i]) Mutex_Close(MYSQL_Mutex[i]);
		MYSQL_Mutex[i] = NULL;
		if (conn[i]) l_mysql_close(conn[i]);
		conn[i] = NULL;
	}
	if (MySQL_Thread_id) free(MySQL_Thread_id);
	MySQL_Thread_id = NULL;
	free(MYSQL_Mutex);
	MYSQL_Mutex = NULL;
	free(conn);
	conn = NULL;
	DEBUG("%s (%i) :: MySQL Stopped.\n", __func__, __LINE__);
	SQL_Mutex_Close();
	l_mysql_close = NULL;
	l_mysql_data_seek = NULL;
	l_mysql_error = NULL;
	l_mysql_fetch_row = NULL;
	l_mysql_free_result = NULL;
	l_mysql_init = NULL;
	l_mysql_num_rows = NULL;
	l_mysql_options = NULL;
	l_mysql_query = NULL;
	l_mysql_real_connect = NULL;
	l_mysql_row_seek = NULL;
	l_mysql_set_character_set = NULL;
	l_mysql_store_result = NULL;
	l_mysql_use_result = NULL;
#ifdef WIN32
	if (libmysql_dll)
		dlclose(libmysql_dll);
#endif
	libmysql_dll = NULL;
}
