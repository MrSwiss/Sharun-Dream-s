#include "SQL_int.h"
#include <mysql.h>
#include <stdarg.h>


extern void		(*l_mysql_close)(MYSQL *sock);
extern void		(*l_mysql_data_seek)(MYSQL_RES *result, my_ulonglong offset);
extern const char *	(*l_mysql_error)(MYSQL *mysql);
extern MYSQL_ROW	(*l_mysql_fetch_row)(MYSQL_RES *result);
extern void		(*l_mysql_free_result)(MYSQL_RES *result);
extern MYSQL *		(*l_mysql_init)(MYSQL *mysql);
extern my_ulonglong	(*l_mysql_num_rows)(MYSQL_RES *res);
extern int		(*l_mysql_options)(MYSQL *mysql, enum mysql_option option, const void *arg);
extern int		(*l_mysql_query)(MYSQL *mysql, const char *q);
extern MYSQL *		(*l_mysql_real_connect)(MYSQL *mysql, const char *host,
					   const char *user,
					   const char *passwd,
					   const char *db,
					   const unsigned int port,
					   const char *unix_socket,
					   unsigned long clientflag);
extern MYSQL_ROW_OFFSET (*l_mysql_row_seek)(MYSQL_RES *result, MYSQL_ROW_OFFSET offset);
extern int		(*l_mysql_set_character_set)(MYSQL *mysql, const char *csname);
extern MYSQL_RES *     (*l_mysql_store_result)(MYSQL *mysql);
extern MYSQL_RES *     (*l_mysql_use_result)(MYSQL *mysql);


int		MySQL_Init(const char *host, ushort port, const char *name, const char *pass, const char *db, short thr, short sthr);
char ***MySQL_Query_Fast(char *Query, int result);
void	MySQL_Query_Slow(char *Query);
void	MySQL_Close();


void SQL_toFloat(char *val)
{
  char *tmp = strchr(val, ',');
  if (tmp) tmp[0] = '.';
}

SQL_QUEUE *SQL_NewQueue(char *Query)
{
	SQL_QUEUE *sql_q = malloc(sizeof(SQL_QUEUE));
	sql_q->Query = Query;
	sql_q->Response = NULL;
//	sql_q->finished = 0;
	sql_q->finished = NULL;
	sql_q->result = 0;
	sql_q->Next = NULL;
	return sql_q;
}

int SQL_Init(const char *host, const ushort port, const char *name, const char *pass, const char *db, short thr, short sthr)
{
	int ret = 0;
	ret = MySQL_Init(host, port, name, pass, db, thr, sthr);
	return ret;
}

void SQL_Close()
{
	MySQL_Close();
}

uint SQL_Max_Row(void *Result)
{
	return l_mysql_num_rows(Result);
}

void SQL_Seek(void *Result, ulong pos)
{
	l_mysql_data_seek(Result, pos);
}

void *SQL_Next_Row(void *Result)
{
	MYSQL_ROW row;
	if (Result && (row = l_mysql_fetch_row(Result)) != NULL)
		return row;
	return NULL;
}

void SQL_Result_Clear(void *Result)
{
	if (Result) {
		l_mysql_free_result(Result);
	}
}

void *SQL_Query_Fast(char *Query, char result)
{
	void *ret = NULL;
	ret = MySQL_Query_Fast(Query, result);
	return ret;
}
void *SQL_Query_Fast2(char *Query, char result, ...)
{
	void *ret = NULL;
	char req[1024];
	va_list __local_argv;
	va_start(__local_argv, result );
	vsprintf(req, Query, __local_argv );
	va_end(__local_argv );
	ret = SQL_Query_Fast(req, result);
	return ret;
}

void SQL_Query_Slow(char *Query)
{
	MySQL_Query_Slow(strdump(Query));
}
void SQL_Query_Slow2(char *Query, ...)
{
	char req[1024];
	va_list __local_argv;
	va_start(__local_argv, Query );
	vsprintf(req, Query, __local_argv );
	va_end(__local_argv );
	SQL_Query_Slow(req);
}



short SQL_short(char *req)
{
	short i = 0;
	void *Res = SQL_Query_Fast(req, 1);
	char **res = SQL_Next_Row(Res);
	if (res && res[0])
		i = atoi(res[0]);
	SQL_Result_Clear(Res);
	return i;
}

void SQL_short2(char *req, short *prv)
{
	*prv = SQL_short(req);
}

int SQL_int(char *req)
{
	int i=0;
	void *Res = SQL_Query_Fast(req, 1);
	char **res = SQL_Next_Row(Res);
	if (res && res[0])
		i = atoi(res[0]);
	SQL_Result_Clear(Res);
	return i;
}

void SQL_int2(char *req, int *prv)
{
	*prv = SQL_int(req);
}

long SQL_long(char *req)
{
	long i=0;
	void *Res = SQL_Query_Fast(req, 1);
	char **res = SQL_Next_Row(Res);
	if (res && res[0])
		i = atoi(res[0]);
	SQL_Result_Clear(Res);
	return i;
}

void SQL_long2(char *req, long *prv)
{
	*prv = SQL_long(req);
}

char *SQL_char(char *req)
{
	char *ret = NULL;
	void *Res = SQL_Query_Fast(req, 1);
	char **res = SQL_Next_Row(Res);
	if (res)
		ret = strdump(res[0]);
	SQL_Result_Clear(Res);
	return ret;
}

void SQL_charcpy(char *req, char *prv, int size)
{
	void *Res = SQL_Query_Fast(req, 1);
	char **res = SQL_Next_Row(Res);
	if (res) {
		memset(prv, 0, size);
		if (strlen(res[0])+1 < size) size = strlen(res[0]);
		strncpy(prv, res[0], size);
	}
	SQL_Result_Clear(Res);
}

char *SQL_freechar(char *req, char *prv)
{
	void *Res = SQL_Query_Fast(req, 1);
	char **res = SQL_Next_Row(Res);
	if (res) {
		if (prv) free(prv);
		prv = strdump(res[0]);
	}
	SQL_Result_Clear(Res);
	return prv;
}

char *SQL_rechar(char *req, char *prv)
{
	void *Res = SQL_Query_Fast(req, 1);
	char **res = SQL_Next_Row(Res);
	if (res) {
		prv = realloc(prv, strlen(res[0])+1);
		strcpy(prv, res[0]);
	}
	SQL_Result_Clear(Res);
	return prv;
}


int SQL_Check_Char(char *data)
{
  int i=0;
  char* F = data-1;
  do
  {
    F++;
    F = strchr(F, '\'');
    if (F) i++;
  } while (F);
  return i;
}

void SQL_Format_Char(char *data)
{
  char* F = data-1;
  do
  {
    F++;
    F = strchr(F, '\'');
    if (F) {
      char tmp[strlen(F)+1];
      strcpy(tmp, F);
      strcpy(&F[1], tmp);
      F++;
    }
  } while (F);
}

#ifdef WIN32
HANDLE sql_mtx[2];
int SQL_Mutex_Init()
{
	for (int i=0; i<2; i++) {
		sql_mtx[i] = CreateMutex(NULL, FALSE, NULL);
		if (!sql_mtx[i]) {
			DEBUG("%s (%i) :: CreateMutex %i error (%d) !\n", __func__, __LINE__, i, GetLastError());
			return 1;
		} else DEBUG("%s (%i) :: SQL Mutex %i Loaded.\n", __func__, __LINE__, i);
	}
	DEBUG("%s (%i) :: SQL Mutex 1 & 2 Loaded.\n", __func__, __LINE__);
	return 0;
}
void SQL_Mutex_Close()
{
	for (int i=0; i<2; i++) {
		CloseHandle(sql_mtx[i]);
		DEBUG("%s (%i) :: SQL Mutex %i Cleaned.\n", __func__, __LINE__, i);
	}
}
void SQL_Mutex_Lock(bool fast)
{
	WaitForSingleObject(sql_mtx[fast], INFINITE);
}
void SQL_Mutex_Unlock(bool fast)
{
	ReleaseMutex(sql_mtx[fast]);
}
#else
pthread_mutex_t sql_mtx[2];
int SQL_Mutex_Init()
{
	for (int i=0; i<2; i++) {
		int ret = pthread_mutex_init(&sql_mtx[i], NULL);
		if (ret) {
			DEBUG("%s (%i) :: CreateMutex %i error (%d) !\n", __func__, __LINE__, i, ret);
			return 1;
		} else DEBUG("%s (%i) :: SQL Mutex %i Loaded.\n", __func__, __LINE__, i);
	}
	return 0;
}
void SQL_Mutex_Close()
{
	for (int i=0; i<2; i++) {
		pthread_mutex_destroy(&sql_mtx[i]);
		DEBUG("%s (%i) :: SQL Mutex %i Cleaned.\n", __func__, __LINE__, i);
	}
}
void SQL_Mutex_Lock(bool fast)
{
	pthread_mutex_lock(&sql_mtx[fast]);
}
void SQL_Mutex_Unlock(bool fast)
{
	pthread_mutex_unlock(&sql_mtx[fast]);
}
#endif
