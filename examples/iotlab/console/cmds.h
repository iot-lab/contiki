#ifndef __COMMANDS__
#define __COMMANDS__

typedef int (*ExecCommand) (char* params[]);

typedef struct {
	const char * name;
	ExecCommand exec;
} Command;

#endif
