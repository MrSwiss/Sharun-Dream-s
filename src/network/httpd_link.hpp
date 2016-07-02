#define httpd_link_Head_Size 512

typedef struct httpd_link_t
{
	char	Head[httpd_link_Head_Size];
	int	Size;
	bool	noHead;
	connexion_list* connex;
} httpd_link_t;


void HTTP_Work(httpd_link_t*);
void HttpD_Add(httpd_link_t*);
