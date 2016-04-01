#ifndef NO_HTTP

#include "Player.h"
#include "mime_type.h"
#include "can_transform.h"

#include <stdarg.h>
#include <sys/stat.h>

#define write_head(sock, text) write(sock, text, strlen(text))

long write_head_printf(SOCKET sock, const char *__msg, ...)
{
	char tmp[1024];
	va_list __local_argv;
	va_start(__local_argv, __msg );
	vsprintf(tmp, __msg, __local_argv );
	va_end(__local_argv );
	return write_head(sock, tmp);
}

void HTTP_DateTime_Char(SOCKET sock, bool transformed, const char *file)
{
	struct stat st;
	time_t rawtime;
 	struct tm * t_info;
 	char buffer [80];

	stat(file, &st);
	t_info = localtime (&st.st_ctime);
	strftime (buffer, 80, "Date:    , %g     %Y %T %Z\n", t_info);
	strncpy(buffer+2+4, wday_name[t_info->tm_wday], 3);
	strncpy(buffer+10+4, mon_name[t_info->tm_mon], 3);
	write_head(sock, buffer);

  	if (transformed) {
		time (&rawtime);
		t_info = localtime (&rawtime);
	} else
		t_info = localtime (&st.st_mtime);
	strftime (buffer, 80, "Last-Modified:    , %g     %Y %T %Z\n", t_info);
	strncpy(buffer+2+13, wday_name[t_info->tm_wday], 3);
	strncpy(buffer+10+13, mon_name[t_info->tm_mon], 3);
	write_head(sock, buffer);
}

char *HTTP_Transform_Players_List()
{
	char *ret, *tmp = l_malloc(1);
	tmp[0] = 0;
	for (long i=0; i < Player_Max; i++) {
		if (Players[i] && Players[i]->loged > 0) {
			tmp = l_realloc(tmp, strlen(tmp)+4+(Players[i]->Right >= 5 ? 5 : 0)+strlen(Players[i]->name)+5+1);
			sprintf(tmp + strlen(tmp), "<li>%s%s</li>", Players[i]->Right >= 5 ? "[GM] " : "", Players[i]->name);
		}
	}
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
		dst = malloc(10);
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
		if (!p) p = HTTP_Transform_Adjust(tmp, "%SERVER_NAME%", Cfg_Server_Name, 0);
		if (!p) p = HTTP_Transform_Adjust(tmp, "%SERVER_START_TIME%", Start_Time, 0);
		if (!p) p = HTTP_Transform_Adjust(tmp, "%SERVER_START_COUNT%", NULL, Start_Count);
		if (!p) p = HTTP_Transform_Adjust(tmp, "%SYSTEM_NAME%", SYSTEM_NAME, 0);
		if (!p) p = HTTP_Transform_Adjust(tmp, "%SYSTEM_VERSION%", SYSTEM_VERSION, 0);
		if (!p && html_file) p = HTTP_Transform_Adjust(tmp, "</body>", "\t%SERVER_SIGN%\n\t\t<sup>%SYSTEM_NAME% (%SYSTEM_VERSION%), a Tera server. Http server side.</sup>\n\t</BODY>", 0);
		if (!p && html_file) p = HTTP_Transform_Adjust(tmp, "<body>", "<BODY>", 0);
		if (!p && html_file) p = HTTP_Transform_Adjust(tmp, "</head>", "\t<!-- Created by Clofriwen, inspired from Tera Shock. http://forum.ragezone.com/f796/dev-sharun-dreams-1085405/ -->\n\t\t%SERVER_COPYRIGHT%\n\t</HEAD>", 0);
		if (!p && html_file) p = HTTP_Transform_Adjust(tmp, "<head>", "<HEAD>", 0);
		if (!p) p = HTTP_Transform_Adjust(tmp, "%SERVER_COPYRIGHT%", SERVER_COPYRIGHT, 0);
		if (!p) p = HTTP_Transform_Adjust(tmp, "%SERVER_SIGN%", SERVER_SIGN, 0);
		if (!p) p = HTTP_Transform_Adjust(tmp, "%PLAYERS_COUNT%", NULL, PLAYER_COUNT);
		if (!p) p = HTTP_Transform_Adjust(tmp, "%PLAYERS_LIST%", NULL, 0);
	} while (p);
}

// Only on texts (html, js, css, ...) !!!
void HTTP_Transform(SOCKET sock, bool noHead, FILE *fp, bool http_head, const char *src)
{
	char *ext = strrchr(src, '.');
	bool html_file = false;
	if (!strcmp(ext, ".html")) html_file = true;
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *tmp = l_malloc(size+1);
	fread(tmp, 1, size, fp);
	tmp[size] = 0;
	HTTP_Transform_Change(&tmp, html_file);
	if (!noHead) {
		write_head_printf(sock, "Content-length: %ld\n", strlen(tmp));
		write_head(sock, "\n");
	}
	if (!http_head)
		write(sock, tmp, strlen(tmp));
	l_free(tmp);
}

void HTTP_Direct(SOCKET sock, bool noHead, FILE *fp, long start_pos, bool http_head)
{
	if (!noHead) {
		fseek(fp, 0, SEEK_END);
		write_head_printf(sock, "Content-length: %ld\n", ftell(fp));
		write_head(sock, "\n");
	}
	fseek(fp, start_pos, SEEK_SET);
	while (!http_head && !feof(fp)) {
		char tmp[128];
		short r = fread(tmp, 1, 128, fp);
		short s = write(sock, tmp, r);
		if (r > s) return;
	}
}

void HTTP_MimeType(SOCKET sock, char *src)
{
	char *ext = strrchr(src, '.');
	for (short i=0; mime_type[i][0]; i++) {
		if (!strcmp(ext, mime_type[i][0])) {
			write_head_printf(sock, "Content-Type: %s\n", mime_type[i][1]);
			return;
		}
	}
	write_head_printf(sock, "Content-Type: %s\n", mime_type[0][1]);
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

void HTTP_Work(netlink_t *NetLink)
{
	bool http_head = false;
	FILE *fp = NULL;
	ushort filename_s = strlen(Cfg_Server_DirSave)+3+1;
	char filename[filename_s];
	sprintf(filename, "%swww/", Cfg_Server_DirSave);
	char *WWW_PATH = realpath(filename, NULL);
	WWW_PATH = realloc(WWW_PATH, strlen(WWW_PATH)+2);
	WWW_PATH[strlen(WWW_PATH)+1] = 0;
	WWW_PATH[strlen(WWW_PATH)] = '/';

	char HTML_Err_tmp[strlen(WWW_PATH)+9];

//hexdump(__func__, "Incomming", NetLink->Head, NetLink->Size);

	if (!memcmp(NetLink->Head, "HEAD ", 5)) http_head = true;

	ushort e, b = 5;
	if (!memcmp(NetLink->Head, "POST /", 6) || !memcmp(NetLink->Head, "HEAD ", 5)) b = 6;
	if (!memcmp(NetLink->Head, "TRACE /", 7)) b = 7;
	e = strstr(NetLink->Head, " HTTP/") - NetLink->Head;
	char *URL_PATH = malloc(e-b+1);
	memset(URL_PATH, 0, e-b+1);
	strncpy(URL_PATH, NetLink->Head+b, e-b);

	if (!URL_PATH[0]) {
		URL_PATH = realloc(URL_PATH, 11);
		sprintf(URL_PATH, "index.html");
	}

	char FILE_PATH_TMP[strlen(WWW_PATH)+strlen(URL_PATH)+1];
	sprintf(FILE_PATH_TMP, "%s%s", WWW_PATH, URL_PATH);
	char *FILE_PATH = realpath(FILE_PATH_TMP, NULL);

	if (!memcmp(NetLink->Head, "POST ", 5) || !memcmp(NetLink->Head, "TRACE ", 6))
		goto HTML_501;

	if (FILE_PATH) {
		if (strcmp(WWW_PATH, FILE_PATH) <= 0) {
			bool CanTrans = HTTP_Can_Transform(FILE_PATH_TMP);
			if (!NetLink->noHead) {
				write_head(NetLink->sock, "HTTP/1.0 200 OK\n");
				write_head(NetLink->sock, "Connection: close\n");
				write_head_printf(NetLink->sock, "Server: %s\n", DEFAULT_SERVER_NAME);
				HTTP_MimeType(NetLink->sock, FILE_PATH_TMP);
				HTTP_DateTime_Char(NetLink->sock, CanTrans, FILE_PATH_TMP);
			}
			fp = fopen(FILE_PATH_TMP, "r");
			if (fp) {
				if (CanTrans)
					HTTP_Transform(NetLink->sock, NetLink->noHead, fp, http_head, FILE_PATH_TMP);
				else
					HTTP_Direct(NetLink->sock, NetLink->noHead, fp, 0, http_head);
				fclose(fp);
				fp = NULL;
				goto HTML_DONE;
			} else goto HTML_404;
		} else goto HTML_403;
	} else goto HTML_404;

HTML_403:
	sprintf(HTML_Err_tmp, "%s403.html", WWW_PATH);
	if (!NetLink->noHead) {
		write_head(NetLink->sock, "HTTP/1.0 403 OK\n");
		write_head(NetLink->sock, "Connection: close\n");
		write_head_printf(NetLink->sock, "Server: %s\n", DEFAULT_SERVER_NAME);
		HTTP_MimeType(NetLink->sock, HTML_Err_tmp);
		HTTP_DateTime_Char(NetLink->sock, true, HTML_Err_tmp);
	}
	fp = fopen(HTML_Err_tmp, "r");
	if (fp) {
		HTTP_Transform(NetLink->sock, NetLink->noHead, fp, http_head, HTML_Err_tmp);
		fclose(fp);
		fp = NULL;
		goto HTML_DONE;
	}

HTML_404:
	sprintf(HTML_Err_tmp, "%s404.html", WWW_PATH);
	if (!NetLink->noHead) {
		write_head(NetLink->sock, "HTTP/1.0 404 OK\n");
		write_head(NetLink->sock, "Connection: close\n");
		write_head_printf(NetLink->sock, "Server: %s\n", DEFAULT_SERVER_NAME);
		HTTP_MimeType(NetLink->sock, HTML_Err_tmp);
		HTTP_DateTime_Char(NetLink->sock, true, HTML_Err_tmp);
	}
	fp = fopen(HTML_Err_tmp, "r");
	if (fp) {
		HTTP_Transform(NetLink->sock, NetLink->noHead, fp, http_head, HTML_Err_tmp);
		fclose(fp);
		fp = NULL;
		goto HTML_DONE;
	}

HTML_501:
	sprintf(HTML_Err_tmp, "%s501.html", WWW_PATH);
	if (!NetLink->noHead) {
		write_head(NetLink->sock, "HTTP/1.0 501 OK\n");
		write_head(NetLink->sock, "Connection: close\n");
		write_head_printf(NetLink->sock, "Server: %s\n", DEFAULT_SERVER_NAME);
		HTTP_MimeType(NetLink->sock, HTML_Err_tmp);
		HTTP_DateTime_Char(NetLink->sock, true, HTML_Err_tmp);
	}
	fp = fopen(HTML_Err_tmp, "r");
	if (fp) {
		HTTP_Transform(NetLink->sock, NetLink->noHead, fp, http_head, HTML_Err_tmp);
		fclose(fp);
		fp = NULL;
		goto HTML_DONE;
	}

HTML_DONE:
	if (FILE_PATH) free(FILE_PATH);
	if (URL_PATH) free(URL_PATH);

	if (NetLink) {
		NetLink_close(NetLink);
		free(NetLink);
	}

	c_THREAD_free(NULL);
}
#endif

