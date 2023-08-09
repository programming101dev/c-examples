#include <stdio.h>
#include <stdlib.h>


static void print_env_variable(const char *variable);


int main(void)
{
    const char *env_variables[] =
    {
        "ARFLAGS",
        "CC",
        "COLUMNS",
        "CDPATH",
        "CFLAGS",
        "CHARSET",
        "DATEMSK",
        "DEAD",
        "EDITOR",
        "ENV",
        "EXINIT",
        "FC",
        "FCEDIT",
        "FFLAGS",
        "GFLAGS",
        "GET",
        "HISTFILE",
        "HISTSIZE",
        "HISTORY",
        "HOME",
        "IFS",
        "LANG",
        "LDFLAGS",
        "LEX",
        "LFLAGS",
        "LC_ALL",
        "LC_COLLATE",
        "LC_CTYPE",
        "LC_MESSAGES",
        "LC_MONETARY",
        "LC_NUMERIC",
        "LC_TIME",
        "LINENO",
        "LINES",
        "LISTER",
        "LOGNAME",
        "LPDEST",
        "MAIL",
        "MAILCHECK",
        "MAILER",
        "MAILPATH",
        "MAILRC",
        "MAKEFLAGS",
        "MAKESHELL",
        "MANPATH",
        "MBOX",
        "MORE",
        "MSGVERB",
        "NPROC",
        "NLSPATH",
        "OLDPWD",
        "OPTARG",
        "OPTERR",
        "OPTIND",
        "PAGER",
        "PATH",
        "PPID",
        "PRINTER",
        "PROCLANG",
        "PROJECTDIR",
        "PS1",
        "PS2",
        "PS3",
        "PS4",
        "PWD",
        "RANDOM",
        "SECONDS",
        "SHELL",
        "TERM",
        "TERMCAP",
        "TERMINFO",
        "TMPDIR",
        "TZ",
        "USER",
        "VISUAL",
        "YACC",
        "YFLAGS",
    };

    for(size_t i = 0; i < sizeof(env_variables) / sizeof(env_variables[0]); i++)
    {
        print_env_variable(env_variables[i]);
    }

    return EXIT_SUCCESS;
}

static void print_env_variable(const char *variable)
{
    const char *value = getenv(variable);

    if(value != NULL)
    {
        printf("%s: %s\n", variable, value);
    }
    else
    {
        printf("%s: (not set)\n", variable);
    }
}
