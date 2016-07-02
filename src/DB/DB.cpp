#include "Sharun.hpp"
#include "internal/DB.hpp"

#include <cstdarg>

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

void SQL_QUEUE::signal()
{
	pthread_pause_signal(resume);
}

bool is_MySQL_threaded()
{
	if (Sharun->Settings.Thread.DataBase.Fast > 0)
		return true;
	return false;
}

void database::start()
{
	thread_list *thread = NULL;
	if (is_MySQL_threaded()) {
		for (int i=0; DB_Set.Login && i < Sharun->Settings.Thread.DataBase.Fast; i++) {
			thread = new thread_list();
			thread->sub_param = 1;
			if (!thread->start((void*)MySQL_Thread, thread))
				DEBUG("%s (%i) :: Could not start MySQL thread %i !\n", __FILE__, __LINE__, i);
			else
				DEBUG("%s (%i) :: MySQL fast thread %i started.\n", __FILE__, __LINE__, i);
		}
		for (int i=0; DB_Set.Login && i < Sharun->Settings.Thread.DataBase.Slow; i++) {
			thread = new thread_list();
			if (!thread->start((void*)MySQL_Thread, thread))
				DEBUG("%s (%i) :: Could not start MySQL thread %i !\n", __FILE__, __LINE__, i);
			else
				DEBUG("%s (%i) :: MySQL slow thread %i started.\n", __FILE__, __LINE__, i);
		}
	}
}

void database::Seek(void *Result, ulong pos)
{
	if (!Result)
		return;
	if (is_MySQL_threaded())
		mysql_data_seek((MYSQL_RES*)Result, pos);
}

char **database::Next_Row(void *Result)
{
	if (!Result)
		return NULL;
	if (is_MySQL_threaded()) {
		MYSQL_ROW row;
		if ((row = mysql_fetch_row((MYSQL_RES*)Result)) != NULL)
			return row;
	}
	return NULL;
}

void database::Result_Clear(void *Result)
{
	if (!Result)
		return;
	if (is_MySQL_threaded())
		mysql_free_result((MYSQL_RES*)Result);
}

void* database::Query_Fast(char *Query, bool responce)
{
	if (!Query)
		return NULL;
	void *ret = NULL;
	if (is_MySQL_threaded()) {
		SQL_QUEUE *SQL_Comm = new SQL_QUEUE(Query, true);
		SQL_Comm->responce = responce;
		MySQL_Add(SQL_Comm);
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
		MySQL_Add(SQL_Comm);
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
		MySQL_Add(SQL_Comm);
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
		MySQL_Add(SQL_Comm);
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
		MySQL_Add(SQL_Comm);
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
		MySQL_Add(SQL_Comm);
		SQL_Comm->wait();
		char ** Row = Next_Row(SQL_Comm->result);
		if (Row && Row[0]) {
			if (ret)
				strcpy16(ret, Row[0]);
			else
				ret = str_str(Row[0]);
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
		MySQL_Add(SQL_Comm);
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
