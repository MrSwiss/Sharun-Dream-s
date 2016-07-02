#include "Sharun.hpp"
#include "internal/Fixed_Thread.hpp"
#include "internal/DB.hpp"

#include <queue>

std::queue<SQL_QUEUE*> MySQL_Fast_queue;
Fixed_thread_W_t	MySQL_Fast_thread;

std::queue<SQL_QUEUE*> MySQL_Slow_queue;
Fixed_thread_W_t	MySQL_Slow_thread;

MYSQL *MySQL_g_conn = NULL;
pthread_mutex_t MySQL_g_mtx;

void MySQL_Add(SQL_QUEUE *SQL_Comm)
{
	if (SQL_Comm->is_Fast()) {
		pthread_mutex_lock(&MySQL_Fast_thread.mtx);
		MySQL_Fast_queue.push(SQL_Comm);
		pthread_mutex_unlock(&MySQL_Fast_thread.mtx);
		Fixed_thread_Signal(&MySQL_Fast_thread);
	} else  {
		pthread_mutex_lock(&MySQL_Slow_thread.mtx);
		MySQL_Slow_queue.push(SQL_Comm);
		pthread_mutex_unlock(&MySQL_Slow_thread.mtx);
		Fixed_thread_Signal(&MySQL_Slow_thread);
		Fixed_thread_Signal(&MySQL_Fast_thread);
	}
}

void MySQL_Cleanup()
{
	Fixed_thread_Cleanup(&MySQL_Fast_thread);
	while (!MySQL_Fast_queue.empty())
		MySQL_Fast_queue.pop();
	Fixed_thread_Cleanup(&MySQL_Slow_thread);
	while (!MySQL_Slow_queue.empty())
		MySQL_Slow_queue.pop();
}

bool MySQL_Init(MYSQL **conn)
{
	if (!conn) {
		conn = &MySQL_g_conn;
		pthread_mutex_init(&MySQL_g_mtx, NULL);
	}
	do
		*conn = mysql_init(NULL);
	while (!*conn);
	if (!*conn || !mysql_real_connect(*conn, DB_Set.IP, DB_Set.Login, DB_Set.Pass,
			DB_Set.Name, DB_Set.Port, NULL, CLIENT_SSL)) {
		DEBUG("%s (%i) :: %s\n", __func__, __LINE__, mysql_error(*conn));
		return false;
	}
	bool reconnect = true;
	mysql_options((*conn), MYSQL_OPT_RECONNECT, &reconnect);
//#ifdef mysql_set_character_set
	if (mysql_set_character_set((*conn), "utf8"))
		DEBUG("%s (%i) :: %s\n", __func__, __LINE__, mysql_error(*conn));
//#endif
	return true;
}

void MySQL_Close(MYSQL **conn)
{
	if (!conn) {
		conn = &MySQL_g_conn;
		pthread_mutex_destroy(&MySQL_g_mtx);
	}
	mysql_close(*conn);
	*conn = NULL;
}

void * MySQL_Work(MYSQL *conn, char *Query, bool fast, bool stored, bool responce)
{
	if (!conn) {
		conn = MySQL_g_conn;
		pthread_mutex_lock(&MySQL_g_mtx);
	}
	if (!conn)
		return NULL;
	MYSQL_RES *res = NULL;
	mysql_query(conn, Query);
	if (mysql_error(conn)[0])
		DEBUG("%s (%i) :: %s : \"%s\"\n", __func__, __LINE__, mysql_error(conn), Query);
	if (fast && stored)
		res = mysql_store_result(conn);
	else
		res = mysql_use_result(conn);
	if (!fast) {
		if (Query) free(Query);
		if (res) mysql_free_result(res);
		res = NULL;
	} else {
		if (!responce && res) {
			mysql_free_result(res);
			res = NULL;
		}
	}
	if (conn == MySQL_g_conn)
		pthread_mutex_unlock(&MySQL_g_mtx);
	return (void*)res;

}

void MySQL_Thread(thread_list *my_thread)
{
	MYSQL *conn = NULL;
	bool fast = false;
	if (my_thread->sub_param)
		fast = true;

	if (!MySQL_Init(&conn)) {
		DEBUG("%s (%i) :: MySQL %s not started !\n", __FILE__, __LINE__, fast ? "Faster" : "Slower");
		delete my_thread;
	} else {
		DEBUG("%s (%i) :: MySQL %s Started.\n", __FILE__, __LINE__, fast ? "Faster" : "Slower");

		SQL_QUEUE* SQL_Comm = NULL;
		Fixed_thread_t *MySQL_thread_l1 = NULL;
		Fixed_thread_t *MySQL_thread_l2 = NULL;

		if (fast) {
			MySQL_thread_l2 = Fixed_thread_Add(MySQL_Cleanup, &MySQL_Slow_thread, my_thread);
			MySQL_thread_l2->thread = NULL;	// Dont kill thread, already done with MySQL_thread_l1
			MySQL_thread_l1 = Fixed_thread_Add(MySQL_Cleanup, &MySQL_Fast_thread, my_thread);
		} else
			MySQL_thread_l1 = Fixed_thread_Add(MySQL_Cleanup, &MySQL_Slow_thread, my_thread);

		while (my_thread->run) {
			SQL_Comm = NULL;
			if (fast) pthread_mutex_lock(&MySQL_Fast_thread.mtx);
			pthread_mutex_lock(&MySQL_Slow_thread.mtx);
			if ((fast && MySQL_Fast_queue.empty() && MySQL_Slow_queue.empty()) || (!fast && MySQL_Slow_queue.empty())) {
				if (fast) pthread_mutex_unlock(&MySQL_Fast_thread.mtx);
				pthread_mutex_unlock(&MySQL_Slow_thread.mtx);
				my_thread->wait();
			} else {
				pthread_mutex_unlock(&MySQL_Slow_thread.mtx);
				if (!MySQL_Fast_queue.empty()) {
					SQL_Comm = MySQL_Fast_queue.front();
					MySQL_Fast_queue.pop();
				}
				if (fast) pthread_mutex_unlock(&MySQL_Fast_thread.mtx);
				pthread_mutex_lock(&MySQL_Slow_thread.mtx);
				if (!MySQL_Slow_queue.empty()) {
					SQL_Comm = MySQL_Slow_queue.front();
					MySQL_Slow_queue.pop();
				}
				pthread_mutex_unlock(&MySQL_Slow_thread.mtx);
				if (SQL_Comm) {
					SQL_Comm->result = MySQL_Work(conn, SQL_Comm->Query, SQL_Comm->is_Fast(), SQL_Comm->stored, SQL_Comm->responce);
					if (SQL_Comm->is_Fast())
						SQL_Comm->signal();
					else
						delete SQL_Comm;
				}
			}
		}

		MySQL_Close(&conn);
		if (fast)
			Fixed_thread_Del(&MySQL_Fast_thread, MySQL_thread_l1);
		else
			Fixed_thread_Del(&MySQL_Slow_thread, MySQL_thread_l1);
		if (MySQL_thread_l2)
			Fixed_thread_Del(&MySQL_Slow_thread, MySQL_thread_l2);
		DEBUG("%s (%i) :: MySQL %s Stopped.\n", __FILE__, __LINE__, fast ? "Faster" : "Slower");
	}
	pthread_exit(NULL);
}
