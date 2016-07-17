#ifndef _PLUGINS_HPP_
#define _PLUGINS_HPP_

typedef struct plugin_opcode_t {
	OpCode_Enum	id;
	const char*	name;
	void*	(*func)(void**);
} plugin_opcode_t;

typedef struct plugin_t {
	void*	handle;
	void	(*init)();
	void	(*uninit)();
	plugin_opcode_t	*recv;
	plugin_opcode_t	*send;
} plugin_t;

#endif // _PLUGINS_HPP_
