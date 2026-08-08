#ifndef __CKIT_ARGS_H__
#define __CKIT_ARGS_H__

#include <ckit/info.h>
#include <ckit/config.h>

typedef enum {
    FLAG_NULL,       // Flag* must end with this type
    FLAG_BOOLEAN,
    FLAG_INTEGER,
    FLAG_STRING
} FlagType;

typedef struct Flag {
    union {
        int integer;
        bool boolean;
        const char* string;
    } data;
    FlagType type;
    const char shortname; // 'h' IS RESERVED
    const char* longname; // "help" IS RESERVED, must not contain "--"
    const char* description;
    int (*callback)(struct Flag* flag);
    int ocurrences;
} Flag;

typedef struct Command {
    Flag* flags;
    int (*callback)(struct Command* command);
    const char* longname; // Command* must end with this field set to NULL
    const char* description;
    struct Command* subcommands; 
    const char** positionals;
    int minpositionals;
    int maxpositionals; // capacity of positionals
    int countpositionals; // positionals parsed
    bool parsed;
} Command;

#define ARGS_HELP           1
#define ARGS_OKAY           0
#define ARGS_CODER          -1
#define ARGS_MISSING        -2
#define ARGS_POSITIONAL     -3
#define ARGS_UNKOWN         -4
#define ARGS_VALUE          -5

/* 
 * subcommands must start at commands[1] as commands[0]
 * is the default workflow.
 *  
 * Return value:
 *      1 if -h or --help was parsed
 *      0 in case of no error
 *      -1 in case of bad data such as argv == NULL, argc == 0 or 
 *      -2 in case of missing command/flag
 *      -3 in case of missing or extra argument/positional
 *      -4 in case of unkown command/flag
 *      -5 in case of error at parsing flag value
 */
int parseargv(Command* root, const char** argv, int argc);

#endif /* __CKIT_ARGS_H__ */