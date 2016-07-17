#include "Sharun.hpp"
#include "DB.hpp"

#include <cstdarg>

extern void	(*MySQL_Data_Seek_PLG)(void*, ulong);
extern char**	(*MySQL_Fetch_Row_PLG)(void*);
extern void	(*MySQL_Free_Result_PLG)(void*);
extern void	(*MySQL_Add_PLG)(SQL_QUEUE*);

SQL_QUEUE::SQL_QUEUE(char *req, bool fast)
{
	stored = true;
	responce = true;
	Query = req;
	result = NULL;
	resume = NULL;
	if (fast) {
		resume = new pthread_pause_t;
		pthread_pause_init(resume, NULL);
	}
}

SQL_QUEUE::~SQL_QUEUE()
{
	if (resume) {
		pthread_pause_destroy(resume);
		delete resume;
	} else
		free(Query);
}

bool SQL_QUEUE::is_Fast()
{
	if (resume)
		return true;
	return false;
}

void SQL_QUEUE::wait()
{
	pthread_pause_wait(resume);
}

bool is_MySQL_threaded()
{
	if (Sharun->Settings.Thread.DataBase.Fast > 0)
		return true;
	return false;
}

void database::Seek(void *Result, ulong pos)
{
	if (!Result)
		return;
	if (is_MySQL_threaded())
		MySQL_Data_Seek_PLG(Result, pos);
}

char **database::Next_Row(void *Result)
{
	if (!Result)
		return NULL;
	if (is_MySQL_threaded()) {
		char ** row;
		if ((row = MySQL_Fetch_Row_PLG(Result)) != NULL)
			return row;
	}
	return NULL;
}

void database::Result_Clear(void *Result)
{
	if (!Result)
		return;
	if (is_MySQL_threaded())
		MySQL_Free_Result_PLG(Result);
}

void* database::Query_Fast(char *Query, bool responce)
{
	if (!Query)
		return NULL;
	void *ret = NULL;
	if (is_MySQL_threaded()) {
		SQL_QUEUE *SQL_Comm = new SQL_QUEUE(Query, true);
		SQL_Comm->responce = responce;
		MySQL_Add_PLG(SQL_Comm);
		SQL_Comm->wait();
		ret = SQL_Comm->result;
		delete SQL_Comm;
	}
	return ret;
}
void *database::Query_Fast2(char *Query, bool responce, ...)
{
	void *ret = NULL;
	char req[1024];
	va_list __local_argv;
	va_start(__local_argv, responce );
	vsprintf(req, Query, __local_argv );
	va_end(__local_argv );
	ret = Query_Fast(req, responce);
	return ret;
}

void database::Query_Slow(char *Query)
{
	if (!Query)
		return;
	if (is_MySQL_threaded()) {
		SQL_QUEUE *SQL_Comm = new SQL_QUEUE(Query, false);
		SQL_Comm->responce = false;
		MySQL_Add_PLG(SQL_Comm);
	}
}
void database::Query_Slow2(char *Query, ...)
{
	char req[1024];
	va_list __local_argv;
	va_start(__local_argv, Query );
	vsprintf(req, Query, __local_argv );
	va_end(__local_argv );
	Query_Slow(req);
}

int database::Query_int(char *Query, int *ret)
{
	if (!Query)
		return 0;
	int value;
	if (!ret)
		ret = &value;
	*ret = 0;
	if (is_MySQL_threaded()) {
		SQL_QUEUE *SQL_Comm = new SQL_QUEUE(Query, true);
		SQL_Comm->stored = false;
		MySQL_Add_PLG(SQL_Comm);
		SQL_Comm->wait();
		char ** Row = Next_Row(SQL_Comm->result);
		if (Row && Row[0])
			*ret = atoi(Row[0]);
		Result_Clear(SQL_Comm->result);
		delete SQL_Comm;
	}
	return *ret;
}

long database::Query_long(char *Query, long *ret)
{
	if (!Query)
		return 0;
	long value;
	if (!ret)
		ret = &value;
	*ret = 0;
	if (is_MySQL_threaded()) {
		SQL_QUEUE *SQL_Comm = new SQL_QUEUE(Query, true);
		SQL_Comm->stored = false;
		MySQL_Add_PLG(SQL_Comm);
		SQL_Comm->wait();
		char ** Row = Next_Row(SQL_Comm->result);
		if (Row && Row[0])
			*ret = atol(Row[0]);
		Result_Clear(SQL_Comm->result);
		delete SQL_Comm;
	}
	return *ret;
}

float database::Query_float(char *Query, float *ret)
{
	if (!Query)
		return 0;
	float value;
	if (!ret)
		ret = &value;
	*ret = 0.0;
	if (is_MySQL_threaded()) {
		SQL_QUEUE *SQL_Comm = new SQL_QUEUE(Query, true);
		SQL_Comm->stored = false;
		MySQL_Add_PLG(SQL_Comm);
		SQL_Comm->wait();
		char ** Row = Next_Row(SQL_Comm->result);
		if (Row && Row[0])
			*ret = atof2(Row[0]);
		Result_Clear(SQL_Comm->result);
		delete SQL_Comm;
	}
	return *ret;
}

char16_t* database::Query_S(char *Query, char16_t *ret)
{
	if (!Query)
		return 0;
	if (is_MySQL_threaded()) {
		SQL_QUEUE *SQL_Comm = new SQL_QUEUE(Query, true);
		SQL_Comm->stored = false;
		MySQL_Add_PLG(SQL_Comm);
		SQL_Comm->wait();
		char ** Row = Next_Row(SQL_Comm->result);
		if (Row && Row[0]) {
			if (ret)
				strcpy8_16(ret, Row[0]);
			else
				ret = str_str8_16(Row[0]);
		}
		Result_Clear(SQL_Comm->result);
		delete SQL_Comm;
	}
	return ret;
}

char* database::Query_char(char *Query, char *ret)
{
	if (!Query)
		return 0;
	if (is_MySQL_threaded()) {
		SQL_QUEUE *SQL_Comm = new SQL_QUEUE(Query, true);
		SQL_Comm->stored = false;
		MySQL_Add_PLG(SQL_Comm);
		SQL_Comm->wait();
		char ** Row = Next_Row(SQL_Comm->result);
		if (Row && Row[0]) {
			if (ret)
				strcpy(ret, Row[0]);
			else
				ret = strdump(Row[0]);
		}
		Result_Clear(SQL_Comm->result);
		delete SQL_Comm;
	}
	return ret;
}
