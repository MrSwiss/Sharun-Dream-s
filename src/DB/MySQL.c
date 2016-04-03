#include "SQL_int.h"
#include <mysql.h>

#ifndef WIN32
	#define HANDLE void*
	#define DWORD void*
#endif

MYSQL **conn = NULL;
void  **MYSQL_Mutex = NULL;
short MySQL_Fast_thread = 0;
short MySQL_Slow_Thread = 0;
HANDLE *MySQL_Thread_id = NULL;
bool MySQL_Thread_Run = false;

SQL_QUEUE *MySQL_Slow = NULL;
SQL_QUEUE *MySQL_Slow_l = NULL;
SQL_QUEUE *MySQL_Fast = NULL;
SQL_QUEUE *MySQL_Fast_l = NULL;

void MySQL_Thread_API(ushort *MT_id)
{
	MYSQL_RES *res;
	bool Fast = true;
	SQL_QUEUE *sql_q;
	short thread_id = *MT_id;
	free(MT_id);
	if (thread_id < MySQL_Slow_Thread) Fast = false;
	DEBUG("%s (%i) :: %s %i Started.\n", __func__, __LINE__, Fast ? "Faster" : "Slower", thread_id);

	while (MySQL_Thread_Run) {
		SQL_Mutex_Lock(Fast);
		if (Fast) {
			sql_q = MySQL_Fast;
			if (sql_q) MySQL_Fast = sql_q->Next;
		} else {
			sql_q = MySQL_Slow;
			if (sql_q) MySQL_Slow = sql_q->Next;
		}
		SQL_Mutex_Unlock(Fast);
		if (sql_q) {
DEBUG("%s (%i) :: \"%s\"\n", __func__, __LINE__, sql_q->Query);
			mysql_query(conn[thread_id], sql_q->Query);
			if (mysql_error(conn[thread_id])[0])
				DEBUG("%s (%i) :: %s : \"%s\"\n", __func__, __LINE__, mysql_error(conn[thread_id]), sql_q->Query);
DEBUG("%s (%i) :: \"%s\"\n", __func__, __LINE__, sql_q->Query);
			if (!Fast || sql_q->result == 2)
				res = mysql_use_result(conn[thread_id]);
			else
				res = mysql_store_result(conn[thread_id]);
			if (!Fast) {
				if (sql_q->Query) free(sql_q->Query);
				free(sql_q);
				if (res) mysql_free_result(res);
			} else {
				if (sql_q->result)
					sql_q->Response = res;
				else
					if (res) mysql_free_result(res);
			}
			if (sql_q && sql_q->finished) 
				Mutex_Unlock(sql_q->finished);
		} else Mutex_Pause(MYSQL_Mutex[thread_id]);
	}
	DEBUG("%s (%i) :: %s %i Stopped.\n", __func__, __LINE__, Fast ? "Faster" : "Slower", thread_id);
	c_THREAD_free(NULL);
}

int MySQL_Init(const char *host, short port, const char *name, const char *pass, const char *db, short thr, short sthr)
{
    if (SQL_Mutex_Init()) return 1;
	MySQL_Slow = NULL;
	MySQL_Fast = NULL;
	MySQL_Fast_thread = thr;
	MySQL_Slow_Thread = sthr;
	MySQL_Thread_id = malloc(sizeof(HANDLE)*(MySQL_Fast_thread+MySQL_Slow_Thread));
	MYSQL_Mutex = malloc(sizeof(void *)*(MySQL_Fast_thread+MySQL_Slow_Thread));
	conn = malloc(sizeof(MYSQL *)*(MySQL_Fast_thread+MySQL_Slow_Thread));
	for (int i=0 ; i < MySQL_Fast_thread+MySQL_Slow_Thread ; i++) {
		MySQL_Thread_id[i] = NULL;
		MYSQL_Mutex[i] = Mutex_Init();
		conn[i] = NULL;
	}

	for (int i=0 ; i < MySQL_Fast_thread+MySQL_Slow_Thread ; i++) {
		conn[i] = mysql_init(NULL);
		if (!mysql_real_connect(conn[i], host, name, pass,
				db, port, NULL, CLIENT_SSL)) {
			DEBUG("%s (%i) :: %s\n", __func__, __LINE__, mysql_error(conn[i]));
			return 0;
		}
		bool reconnect = true;
		mysql_options(conn[i], MYSQL_OPT_RECONNECT, &reconnect);
//#ifdef mysql_set_character_set
		if (mysql_set_character_set(conn[i], "utf8")) {
			DEBUG("%s (%i) :: %s\n", __func__, __LINE__, mysql_error(conn[i]));
			return 0;
		}
//#endif
	}

	MySQL_Thread_Run = true;
	for (long i=0 ; i < MySQL_Fast_thread+MySQL_Slow_Thread ; i++) {
		ushort *MT_id = malloc(sizeof(ushort));
		*MT_id = i;
		MySQL_Thread_id[i] = CreateThread(NULL, 0, (void*)MySQL_Thread_API, MT_id, 0, NULL);
		usleep(1);
	}

	DEBUG("%s (%i) :: MySQL Started.\n", __func__, __LINE__);
	return MySQL_Fast_thread;
}

void MySQL_Run_Thread(void *argv)
{
	for (ushort i=0; i<MySQL_Fast_thread; i++)
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
DEBUG("%s (%i) :: \"%s\"\n", __func__, __LINE__, sql_q->Query);
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
	for (ushort i=0; i<MySQL_Slow_Thread+MySQL_Fast_thread ; i++)
		Mutex_Resume(MYSQL_Mutex[i]);
	if (MySQL_Thread_id) WaitForMultipleObjects(2, MySQL_Thread_id, TRUE, INFINITE);
	for (int i=0 ; i < MySQL_Fast_thread+MySQL_Slow_Thread; i++) {
		CloseHandle(MySQL_Thread_id[i]);
		MySQL_Thread_id[i] = NULL;
		if (MYSQL_Mutex[i]) Mutex_Close(MYSQL_Mutex[i]);
		MYSQL_Mutex[i] = NULL;
		if (conn[i]) mysql_close(conn[i]);
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
}
