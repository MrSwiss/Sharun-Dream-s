#ifndef _DB_HPP_
#define _DB_HPP_

class database {
	public:
		void start();
		uint	Max_Row(void *Result);
		void	Seek(void *Result, ulong pos);
		char**	Next_Row(void *Result);
		void	Result_Clear(void *Result);
		virtual void*	Query_Fast(char *Query, bool responce);
		virtual void	Query_Slow(char *Query);
		virtual void*	Query_Fast2(char *Query, bool responce, ...);
		virtual void	Query_Slow2(char *Query, ...);
		int	Query_int(char *Query, int *ret);
		long	Query_long(char *Query, long *ret);
		float	Query_float(char *Query, float *ret);
		char16_t*	Query_S(char *Query, char16_t *ret);
		char*	Query_char(char *Query, char *ret);
};

#endif // _DB_HPP_
