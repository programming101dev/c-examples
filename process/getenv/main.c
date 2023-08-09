#include <stdio.h>
#include <stdlib.h>


static void print_env_variable(const char* variable);


int main(void)
{
    const char* env_variables[] =
    {
        "ARFLAGS", "IFS", "MAILPATH", "PS1", "CC", "LANG", "MAILRC", "PS2", "CDPATH", "LC_ALL",
        "MAKEFLAGS", "PS3", "CFLAGS", "LC_COLLATE", "MAKESHELL", "PS4", "CHARSET", "LC_CTYPE",
        "MANPATH", "PWD", "COLUMNS", "LC_MESSAGES", "MBOX", "RANDOM", "DATEMSK", "LC_MONETARY",
        "MORE", "SECONDS", "DEAD", "LC_NUMERIC", "MSGVERB", "SHELL", "EDITOR", "LC_TIME",
        "NLSPATH", "TERM", "ENV", "LDFLAGS", "NPROC", "TERMCAP", "EXINIT", "LEX", "OLDPWD",
        "TERMINFO", "FC", "LFLAGS", "OPTARG", "TMPDIR", "FCEDIT", "LINENO", "OPTERR", "TZ",
        "FFLAGS", "LINES", "OPTIND", "USER", "GET", "LISTER", "PAGER", "VISUAL", "GFLAGS",
        "LOGNAME", "PATH", "YACC", "HISTFILE", "LPDEST", "PPID", "YFLAGS", "HISTORY", "MAIL",
        "PRINTER", "HISTSIZE", "MAILCHECK", "PROCLANG", "HOME", "MAILER", "PROJECTDIR"
    };

    for(size_t i = 0; i < sizeof(env_variables) / sizeof(env_variables[0]); i++)
    {
        print_env_variable(env_variables[i]);
    }

    return EXIT_SUCCESS;
}

static void print_env_variable(const char* variable)
{
    const char* value = getenv(variable);

    if(value != NULL)
    {
        printf("%s: %s\n", variable, value);
    }
    else
    {
        printf("%s: (not set)\n", variable);
    }
}
