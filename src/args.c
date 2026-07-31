#include <ckit/args.h>
#include <ckit/utils.h>
#include <ckit/error.h>

typedef struct {
    int argc;
    const char** argv;
} Args;

static void help(Command* command);
static void throwusage(Command* command, Flag* flag);

static bool ishelp(const char* arg) {
    if (*arg++ != '-') { return false; }
    if (*arg == 'h') { return *++arg == '\0'; }
    if (*arg++ != '-') { return false; }

    return  *arg++ == 'h' &&
            *arg++ == 'e' &&
            *arg++ == 'l' &&
            *arg++ == 'p' &&
            *arg   == '\0';
}

static Command* matchcommand(const char* arg, Command* commands) {
    while (commands->longname) {
        if (!strcmp(arg, commands->longname)) { return commands; }
        commands++;
    }
    return NULL;
}

static int parseflag(Command* command, const char* arg) {
    if (*arg++ != '-') { return -4; }
    if (!*arg) { return -2; }

    int len = 0;
    Flag* current = command->flags;
    while (current->type) {
        len = 1;
        if (*arg != '-' && arg[1] == current->shortname && current->shortname) { break; }
        
        int len = strlen(current->longname);
        if (!strncmp(++arg, current->longname, len)) { break; }
    }
}

static int parsecommand(Command* command, Args* args) {
    if (!args->argv || !args->argc) { throwusage(command, NULL); return -2; }
    
    Command* sub = matchcommand(*args->argv, command->subcommands);
    if (sub) {
        args->argc--;
        args->argv++;
        return parsecommand(sub, args);
    }

    while (args->argc) {
        const char* arg = *args->argv;
        if (ishelp(arg)) { help(command); return 1; }

        if (*arg == '-') {
            int error = parseflag(command, arg);
            if (error) { return error; }
        }
    }
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