/*

#include <ckit/args.h>
#include <ckit/utils.h>
#include <ckit/error.h>

typedef struct {
    const char** argv;
    int argc;
    int onlypositionals;
} Args;

#define ishelp(arg) (arg[1] == 'h' && arg[2] == 'e' && arg[3] == 'l' && arg[4] == 'p' && !arg[5])

static void help(Command* command);
static void unkown(const char* longname, char shortname);
static void throwusage(Command* command, Flag* flag);

static Command* matchcommand(const char* arg, Command* commands) {
    while (commands->longname) {
        if (!strcmp(arg, commands->longname)) { return commands; }
        commands++;
    }
    return NULL;
}

static int longflag(Command* command, Args* args) {
    const char* arg = *args->argv + 2;
    const char* equal = strchr(arg, '=');
    size_t len = strlen(arg);
    if (equal) { len = PTRPTR(equal, arg); }

    Flag* flag = command->flags;
    while (flag->type) {
        if (len != strlen(flag->longname)) { flag++; continue; }
        if (!strncmp(arg, flag->longname, len)) { break; }
        flag++;
    }

    if (!flag->type) { return ARGS_UNKOWN; }
    if (equal && flag->type == FLAG_BOOLEAN) { return ARGS_VALUE; }

    const char* value = NULL;
    if (flag->type == FLAG_BOOLEAN) { 
        flag->data.boolean = true;
        goto success;
    } else if (equal) { 
        value = equal + 1;
    } else { 
        args->argc--;
        args->argv++;
        value = *args->argv;
        if (!args->argc || !*args->argv) { return ARGS_MISSING; } 
    }

    if (flag->type == FLAG_STRING) {
        flag->data.string = value;
    } else if (flag->type == FLAG_INTEGER) {
        flag->data.integer = 0;
        if (!isdigit((unsigned char)*value)) { return ARGS_VALUE; }
        while (isdigit((unsigned char)*value)) {
            flag->data.integer *= 10;
            flag->data.integer += ((unsigned char)*value++ - '0');
        }
    }

success:
    flag->parsed = true;
    if (flag->callback) { flag->callback(flag); }
    return ARGS_OKAY;
}
static int shortflag(Command* command, Args* args) {
    int error = ARGS_OKAY;
    const char* arg = *args->argv;
    const char* value = NULL;
    while (*arg++) { // skip '-' in first loop
        Flag* flag = command->flags;
        while (flag->type) {
            if (!flag->shortname) { flag++; continue; }
            if (flag->shortname == 'h') { ithrow(ERROR_SOFT_USER, ERRMSG_RESERVED_SHORT_HELP); }
            if (flag->shortname == *arg) { break; }
        }
        if (!flag->type) { error = ARGS_MISSING; goto failed; }

        flag->ocurrences++;
        if (flag->type == FLAG_BOOLEAN) { 
            flag->data.boolean = true; 
        } else if (arg[1]) { 
            value = arg + 1; 
        } else {
            args->argc--;
            args->argv++;
            value = *args->argv;
            if (!args->argc || !*args->argv) { error = ARGS_MISSING; goto failed; } 
        }

        if (flag->type == FLAG_STRING) {
            flag->data.string = value;
        } else if (flag->type == FLAG_INTEGER) {
            flag->data.integer = 0;
            if (!isdigit((unsigned char)*value)) { error = ARGS_VALUE; goto failed; }
            while (isdigit((unsigned char)*value)) {
                flag->data.integer *= 10;
                flag->data.integer += ((unsigned char)*value++ - '0');
            }
        }
    }

failed:
    Flag* flags = command->flags;
    while (flags->type) { flags->ocurrences = 0; }
    return error;
}

static int parseflag(Command* command, Args* args) {
    const char* arg = *args->argv;
    if (!*++arg) { return ARGS_MISSING; } // "-" case
    if (*arg == 'h' || (*arg == '-' && ishelp(arg))) { help(command); return ARGS_HELP; } 

    if (!command->flags || !command->flags->type) { return ARGS_UNKOWN; }
    if (*arg != '-') { return shortflag(command, args); }
    if (!*arg) { args->onlypositionals = true; return ARGS_OKAY; } // stop flag parsing if "--"
    return longflag(command, args);
}
static int parsecommand(Command* command, Args* args) {
    if (!args->argv || !args->argc) { throwusage(command, NULL); return ARGS_MISSING; }

    while (args->argc) {
        const char* arg = *args->argv;
        if (args->onlypositionals) { goto positional; }

        if (*arg == '-') {
            int error = parseflag(command, args);
            if (error) { return error; }
            args->argc--;
            args->argv++;
            continue;
        }

        Command* sub = matchcommand(*args->argv, command->subcommands);
        if (sub) {
            args->argc--;
            args->argv++;
            return parsecommand(sub, args);
        }

    positional:
        if (command->countpositionals >= command->maxpositionals) { return ARGS_POSITIONAL; }
        command->positionals[command->countpositionals++] = arg;
        args->argc--;
        args->argv++;
    }

    if (command->countpositionals < command->minpositionals) { return ARGS_POSITIONAL; }
    
    command->parsed = true;
    if (command->callback) { command->callback(command); }
    return ARGS_OKAY;
}

int parseargv(Command* root, const char** argv, int argc) {
    ithrowif(!root, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(root));
    ithrowif(!argv, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(argv));
    ithrowif(!argc, ERROR_USER_ZERO_SIZE, ERRMSG_ZERO_SIZE(argc));

    Args args = {0};
    args.argc = --argc;
    args.argv = ++argv;
    return parsecommand(root, &args);
}

*/