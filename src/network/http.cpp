#include "internal/psocket.hpp"
#include "Sharun.hpp"
#include "internal/version.hpp"

#include "httpd_link.h"
#include "mime_type.h"
#include "can_transform.h"

#include <cstdarg>
#include <sys/stat.h>
#include <ctime>
#include <limits.h>

static const char wday_name[][4] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};
static const char mon_name[][4] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

long write_head_printf(connexion_list *connex, const char *__msg, ...)
{
	char tmp[1024];
	va_list __local_argv;
	va_start(__local_argv, __msg );
	vsprintf(tmp, __msg, __local_argv );
	va_end(__local_argv );
	return connex->Send(tmp);
}

void HTTP_DateTime_Char(connexion_list *connex, bool transformed, const char *file)
{
	struct stat st;
	time_t rawtime;
 	struct tm * t_info;
 	char buffer [80];

	stat(file, &st);
	t_info = localtime (&st.st_ctime);
	strftime (buffer, 80, "Date:    , %d     %Y %H:%M:%S %Z\r\n", t_info);
	strncpy(buffer+2+4, wday_name[t_info->tm_wday], 3);
	strncpy(buffer+10+4, mon_name[t_info->tm_mon], 3);
	connex->Send(buffer);

  	if (transformed) {
		time (&rawtime);
		t_info = localtime (&rawtime);
	} else
		t_info = localtime (&st.st_mtime);
	strftime (buffer, 80, "Last-Modified:    , %d     %Y %H:%M:%S %Z\r\n", t_info);
	strncpy(buffer+2+13, wday_name[t_info->tm_wday], 3);
	strncpy(buffer+10+13, mon_name[t_info->tm_mon], 3);
	connex->Send(buffer);
}

char *HTTP_Transform_Players_List()
{
	char *ret, *tmp = l_malloc(1);
	tmp[0] = 0;
/*	for (long i=0; i < Player_Max; i++) {
		if (Players[i] && Players[i]->loged > 0) {
			tmp = l_realloc(tmp, strlen(tmp)+4+(Players[i]->Right >= 5 ? 5 : 0)+strlen(Players[i]->name)+5+1);
			sprintf(tmp + strlen(tmp), "<li>%s%s</li>", Players[i]->Right >= 5 ? "[GM] " : "", Players[i]->name);
		}
	}*/
	ret = l_mem2mem(tmp);
	l_free(tmp);
	return ret;
}

char HTTP_Transform_Adjust(char **tmp, const char *src, char *dst, long dstLong)
{
	char *p = strstr(*tmp, src);
	if (!p) return 0;
	bool in_dst = false;
	if (!strcmp(src, "%PLAYERS_LIST%")) {
		dst = HTTP_Transform_Players_List();
		in_dst = true;
	}
	if (!dst) {
		dst = (char*)malloc(10);
		sprintf(dst, "%ld", dstLong);
		in_dst = true;
	}
	char tmpCPY[strlen(p)-strlen(src)+1];
	strcpy(tmpCPY, p + strlen(src));
	long size = strlen(*tmp);
	size += strlen(dst);
	size -= strlen(src);
	*tmp = l_realloc(*tmp, size+1+100);
	p = strstr(*tmp, src);
	strncpy(p, dst, strlen(dst));
	strncpy(p + strlen(dst), tmpCPY, strlen(tmpCPY)+1);
	if (in_dst) free(dst);
	return 1;
}

void HTTP_Transform_Change(char **tmp, bool html_file)
{
	char p;
	do {
		p = 0;
		if (!p) p = HTTP_Transform_Adjust(tmp, "%SERVER_NAME%", Settings->Server_Name, 0);
		if (!p) p = HTTP_Transform_Adjust(tmp, "%SERVER_START_TIME%", Settings->Start_Time, 0);
		if (!p) p = HTTP_Transform_Adjust(tmp, "%SERVER_START_COUNT%", NULL, Settings->Start_Count);
		if (!p) p = HTTP_Transform_Adjust(tmp, "%SYSTEM_NAME%", (char*)SYSTEM_NAME, 0);
		if (!p) p = HTTP_Transform_Adjust(tmp, "%SYSTEM_VERSION%", (char*)SYSTEM_VERSION, 0);
		if (!p && html_file) p = HTTP_Transform_Adjust(tmp, "</body>", (char*)"\t%SERVER_SIGN%\n\t\t<sup>%SYSTEM_NAME% (%SYSTEM_VERSION%), a Tera server. Http server side.</sup>\n\t</BODY>", 0);
		if (!p && html_file) p = HTTP_Transform_Adjust(tmp, "<body>", (char*)"<BODY>", 0);
		if (!p && html_file) p = HTTP_Transform_Adjust(tmp, "</head>", (char*)"\t<!-- Created by Clofriwen, inspired from Tera Shock. http://forum.ragezone.com/f796/dev-sharun-dreams-1085405/ -->\n\t\t%SERVER_COPYRIGHT%\n\t</HEAD>", 0);
		if (!p && html_file) p = HTTP_Transform_Adjust(tmp, "<head>", (char*)"<HEAD>", 0);
		if (!p) p = HTTP_Transform_Adjust(tmp, "%SERVER_COPYRIGHT%", Settings->SERVER_COPYRIGHT, 0);
		if (!p) p = HTTP_Transform_Adjust(tmp, "%SERVER_SIGN%", Settings->SERVER_SIGN, 0);
		if (!p) p = HTTP_Transform_Adjust(tmp, "%PLAYERS_COUNT%", NULL, Settings->PLAYER_COUNT);
		if (!p) p = HTTP_Transform_Adjust(tmp, "%PLAYERS_LIST%", NULL, 0);
	} while (p);
}

// Only on texts (html, js, css, ...) !!!
void HTTP_Transform(connexion_list *connex, bool noHead, FILE *fp, long size, bool http_head, const char *src)
{
	const char *ext = strrchr(src, '.');
	bool html_file = false;
	if (!strcmp(ext, ".html")) html_file = true;
	char *tmp = l_malloc(size+1);
	fread(tmp, 1, size, fp);
	tmp[size] = 0;
	HTTP_Transform_Change(&tmp, html_file);
	if (!noHead) {
		write_head_printf(connex, "Content-length: %ld\r\n", strlen(tmp));
		connex->Send("\r\n");
	}
	if (!http_head)
		connex->Send(tmp, strlen(tmp));
	l_free(tmp);
}

void HTTP_Direct(connexion_list *connex, bool noHead, FILE *fp, long start_pos, long size, bool http_head)
{
	fseek(fp, 0, SEEK_END);
	if (!noHead) {
		write_head_printf(connex, "Content-length: %ld\r\n", size);
		connex->Send("\r\n");
	}
	fseek(fp, start_pos, SEEK_SET);
	char tmp[1024];
	while (!http_head && !feof(fp)) {
		short r = fread(tmp, 1, 1024, fp);
		short s = connex->Send(tmp, r);
		if (s <= 0) return;
	}
}

void HTTP_MimeType(connexion_list *connex, char *src)
{
	char *ext = strrchr(src, '.');
	for (short i=0; mime_type[i][0]; i++) {
		if (!strcmp(ext, mime_type[i][0])) {
			write_head_printf(connex, "Content-Type: %s\r\n", mime_type[i][1]);
			return;
		}
	}
	write_head_printf(connex, "Content-Type: %s\r\n", mime_type[0][1]);
}

bool HTTP_Can_Transform(char *src)
{
	char *ext = strrchr(src, '.');
	for (short i=0; Can_Transform[i]; i++) {
		if (!strcmp(ext, Can_Transform[i]))
			return true;
	}
	return false;
}

void HTTP_Work(httpd_link_t *httpd_link)
{
	connexion_list *connex = httpd_link->connex;
	bool http_head = false;
	long f_size = 0;
	FILE *fp = NULL;
	char *WWW_PATH = l_realpath(Settings->Dirs.WWW.c_str());
	WWW_PATH = (char*)realloc(WWW_PATH, strlen(WWW_PATH)+2);
	WWW_PATH[strlen(WWW_PATH)+1] = 0;
	WWW_PATH[strlen(WWW_PATH)] = '/';

	char HTML_Err_tmp[strlen(WWW_PATH)+9];

	if (!memcmp(httpd_link->Head, "HEAD ", 5)) http_head = true;

	ushort e, b = 5;
	if (!memcmp(httpd_link->Head, "POST /", 6) || !memcmp(httpd_link->Head, "HEAD ", 5)) b = 6;
	if (!memcmp(httpd_link->Head, "TRACE /", 7)) b = 7;
	e = strstr(httpd_link->Head, " HTTP/") - httpd_link->Head;
	char *URL_PATH = (char*)malloc(e-b+1);
	memset(URL_PATH, 0, e-b+1);
	strncpy(URL_PATH, httpd_link->Head+b, e-b);

	if (!URL_PATH[0]) {
		URL_PATH = (char*)realloc(URL_PATH, 11);
		sprintf(URL_PATH, "index.html");
	}

	char FILE_PATH_TMP[strlen(WWW_PATH)+strlen(URL_PATH)+1];
	sprintf(FILE_PATH_TMP, "%s%s", WWW_PATH, URL_PATH);
	char *FILE_PATH = l_realpath(FILE_PATH_TMP);

	if (!memcmp(httpd_link->Head, "POST ", 5) || !memcmp(httpd_link->Head, "TRACE ", 6))
		goto HTML_501;

	if (FILE_PATH) {
		if (strcmp(WWW_PATH, FILE_PATH) <= 0) {
			bool CanTrans = HTTP_Can_Transform(FILE_PATH_TMP);
			if (!httpd_link->noHead) {
				connex->Send("HTTP/1.0 200 OK\r\n");
				connex->Send("Connection: close\r\n");
				write_head_printf(connex, "Server: %s\r\n", DEFAULT_SERVER_NAME);
				HTTP_MimeType(connex, FILE_PATH_TMP);
				HTTP_DateTime_Char(connex, CanTrans, FILE_PATH_TMP);
			}
			f_size = file_size(FILE_PATH_TMP);
			fp = fopen(FILE_PATH_TMP, "rb");
			if (fp) {
				if (CanTrans)
					HTTP_Transform(connex, httpd_link->noHead, fp, f_size, http_head, FILE_PATH_TMP);
				else
					HTTP_Direct(connex, httpd_link->noHead, fp, 0, f_size, http_head);
				fclose(fp);
				fp = NULL;
				goto HTML_DONE;
			} else goto HTML_404;
		} else goto HTML_403;
	} else goto HTML_404;

HTML_403:
	sprintf(HTML_Err_tmp, "%s403.html", WWW_PATH);
	if (!httpd_link->noHead) {
		connex->Send("HTTP/1.0 403 OK\r\n");
		connex->Send("Connection: close\r\n");
		write_head_printf(connex, "Server: %s\r\n", DEFAULT_SERVER_NAME);
		HTTP_MimeType(connex, HTML_Err_tmp);
		HTTP_DateTime_Char(connex, true, HTML_Err_tmp);
	}
	f_size = file_size(HTML_Err_tmp);
	fp = fopen(HTML_Err_tmp, "r");
	if (fp) {
		HTTP_Transform(connex, httpd_link->noHead, fp, f_size, http_head, HTML_Err_tmp);
		fclose(fp);
		fp = NULL;
		goto HTML_DONE;
	}

HTML_404:
	sprintf(HTML_Err_tmp, "%s404.html", WWW_PATH);
	if (!httpd_link->noHead) {
		connex->Send("HTTP/1.0 404 OK\r\n");
		connex->Send("Connection: close\r\n");
		write_head_printf(connex, "Server: %s\r\n", DEFAULT_SERVER_NAME);
		HTTP_MimeType(connex, HTML_Err_tmp);
		HTTP_DateTime_Char(connex, true, HTML_Err_tmp);
	}
	f_size = file_size(HTML_Err_tmp);
	fp = fopen(HTML_Err_tmp, "r");
	if (fp) {
		HTTP_Transform(connex, httpd_link->noHead, fp, f_size, http_head, HTML_Err_tmp);
		fclose(fp);
		fp = NULL;
		goto HTML_DONE;
	}

HTML_501:
	sprintf(HTML_Err_tmp, "%s501.html", WWW_PATH);
	if (!httpd_link->noHead) {
		connex->Send("HTTP/1.0 501 OK\r\n");
		connex->Send("Connection: close\r\n");
		write_head_printf(connex, "Server: %s\r\n", DEFAULT_SERVER_NAME);
		HTTP_MimeType(connex, HTML_Err_tmp);
		HTTP_DateTime_Char(connex, true, HTML_Err_tmp);
	}
	f_size = file_size(HTML_Err_tmp);
	fp = fopen(HTML_Err_tmp, "r");
	if (fp) {
		HTTP_Transform(connex, httpd_link->noHead, fp, f_size, http_head, HTML_Err_tmp);
		fclose(fp);
		fp = NULL;
		goto HTML_DONE;
	}

HTML_DONE:
	if (FILE_PATH) free(FILE_PATH);
	if (URL_PATH) free(URL_PATH);

	delete httpd_link;
	delete connex;
}
