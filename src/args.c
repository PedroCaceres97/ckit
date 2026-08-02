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
    const char* equal = strrchr(arg, '=');
    size_t len = strlen(arg);
    if (equal) { len = PTRPTR(equal, arg); }

    Flag* flag = command->flags;
    while (flag->type) {
        if (!strncmp(arg, flag->longname, len)) { break; }
        flag++;
    }

    if (!flag->type) { return ARGS_UNKOWN; }
    if (equal && flag->type == FLAG_BOOLEAN) {  }
}
static int shortflag(Command* command, Args* args) {
    Flag* table[256] = {0};
    Flag* flags = command->flags;
    while (flags->type) {
        if (!flags->shortname) { continue; }
        if (flags->shortname == 'h') { ithrow(ERROR_SOFT_USER, ERRMSG_RESERVED_SHORT_HELP); }
        table[(unsigned char)flags->shortname] = flags;
    }

    bool value = false;
    const char* arg = *args->argv;
    while (*arg++) { // skip '-' in first loop
        Flag* flag = table[(unsigned char)*arg];
        if (!flag) { continue; }

        if (flag->type == FLAG_BOOLEAN) { 
            flag->parsed = true;
            flag->data.boolean = true; 
        } else if (flag->type == FLAG_INTEGER) {
            arg++;
            value = true;
            if (!isdigit(*arg) && *arg) { throwusage(NULL, flag); return }
        }
    }

}

static int parseflag(Command* command, Args* args) {
    const char* arg = *args->argv;
    if (!*++arg) { return ARGS_MISSING; } // "-" case
    if (*arg == 'h' || (*arg == '-' && ishelp(arg))) { help(command); return ARGS_HELP; } 

    if (!command->flags || !command->flags->type) { return ARGS_UNKOWN; }
    if (*arg != '-') { return shortflag(command, args); }
    if (!*arg) { args->onlypositionals = true; return ARGS_OKAY; }
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