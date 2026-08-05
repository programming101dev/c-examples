/*
 * Copyright 2026 D'Arcy Smith
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>

static void print_env_variable(const char *variable);

int main(void)
{
    const char *env_variables[] = {
        "ARFLAGS",  "CC",     "COLUMNS",   "CDPATH",    "CFLAGS",  "CHARSET", "DATEMSK",    "DEAD",     "EDITOR",      "ENV",         "EXINIT",     "FC",      "FCEDIT", "FFLAGS", "GFLAGS", "GET",     "HISTFILE", "HISTSIZE", "HISTORY",   "HOME",
        "IFS",      "LANG",   "LDFLAGS",   "LEX",       "LFLAGS",  "LC_ALL",  "LC_COLLATE", "LC_CTYPE", "LC_MESSAGES", "LC_MONETARY", "LC_NUMERIC", "LC_TIME", "LINENO", "LINES",  "LISTER", "LOGNAME", "LPDEST",   "MAIL",     "MAILCHECK", "MAILER",
        "MAILPATH", "MAILRC", "MAKEFLAGS", "MAKESHELL", "MANPATH", "MBOX",    "MORE",       "MSGVERB",  "NPROC",       "NLSPATH",     "OLDPWD",     "OPTARG",  "OPTERR", "OPTIND", "PAGER",  "PATH",    "PPID",     "PRINTER",  "PROCLANG",  "PROJECTDIR",
        "PS1",      "PS2",    "PS3",       "PS4",       "PWD",     "RANDOM",  "SECONDS",    "SHELL",    "TERM",        "TERMCAP",     "TERMINFO",   "TMPDIR",  "TZ",     "USER",   "VISUAL", "YACC",    "YFLAGS",
    };

    for(size_t i = 0; i < sizeof(env_variables) / sizeof(env_variables[0]); i++)
    {
        print_env_variable(env_variables[i]);
    }

    return EXIT_SUCCESS;
}

static void print_env_variable(const char *variable)
{
    const char *value;

    value = getenv(variable);

    if(value != NULL)
    {
        printf("%s: %s\n", variable, value);
    }
    else
    {
        printf("%s: (not set)\n", variable);
    }
}
