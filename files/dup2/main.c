/*
 * This code is licensed under the Attribution-NonCommercial-NoDerivatives 4.0 International license.
 * Author: D'Arcy Smith (ds@programming101.dev)
 */

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void)
{
    /* Declarations only. */
    char        templ[PATH_MAX];
    int         temp_fd;         /* fd returned by mkstemp */
    int         saved_stdout_fd; /* duplicate of original stdout */
    int         have_temp_fd;
    int         have_saved_stdout_fd;
    int         stdout_redirected;
    int         rc;
    int         r_print;
    int         r_dup2;
    int         r_flush;
    int         r_close;
    int         r_unlink;
    int         failed;
    struct stat sb;

    /* Immediate initializations to satisfy Clang's -Wconditional-uninitialized. */
    temp_fd              = -1;
    saved_stdout_fd      = -1;
    have_temp_fd         = 0;
    have_saved_stdout_fd = 0;
    stdout_redirected    = 0;
    failed               = 0;

    /* Prepare mkstemp template. */
    rc = snprintf(templ, sizeof(templ), "%s", "/tmp/tempfileXXXXXX");
    if(rc < 0)
    {
        fprintf(stderr, "snprintf failed\n");
        failed = 1;
        goto cleanup;
    }
    if((size_t)rc >= sizeof(templ))
    {
        fprintf(stderr, "template truncated\n");
        failed = 1;
        goto cleanup;
    }

    /* Create the temporary file securely. */
    temp_fd = mkstemp(templ);
    if(temp_fd == -1)
    {
        perror("mkstemp failed");
        failed = 1;
        goto cleanup;
    }
    have_temp_fd = 1;

    /* Size before write. */
    rc = fstat(temp_fd, &sb);
    if(rc == -1)
    {
        perror("fstat before write failed");
        failed = 1;
        goto cleanup;
    }
    rc = fprintf(stderr, "Size before write: %lld bytes\n", (long long)sb.st_size);
    if(rc < 0)
    {
        perror("fprintf stderr failed");
        failed = 1;
        goto cleanup;
    }

    /* Save current stdout. */
    saved_stdout_fd = dup(STDOUT_FILENO); /* NOLINT(android-cloexec-dup) */
    if(saved_stdout_fd == -1)
    {
        perror("dup(STDOUT_FILENO) failed");
        failed = 1;
        goto cleanup;
    }
    have_saved_stdout_fd = 1;

    /* Redirect stdout to the temp file. */
    r_dup2 = dup2(temp_fd, STDOUT_FILENO);
    if(r_dup2 == -1)
    {
        perror("dup2 to stdout failed");
        failed = 1;
        goto cleanup;
    }
    stdout_redirected = 1;

    /* Write and flush through redirected stdout. */
    r_print = printf("This will be written to the temporary file using printf.\n");
    if(r_print < 0)
    {
        perror("printf failed");
        failed = 1;
        goto cleanup;
    }
    r_flush = fflush(stdout);
    if(r_flush != 0)
    {
        perror("fflush(stdout) failed");
        failed = 1;
        goto cleanup;
    }

    /* Size after write. */
    rc = fstat(temp_fd, &sb);
    if(rc == -1)
    {
        perror("fstat after write failed");
        failed = 1;
        goto cleanup;
    }
    rc = fprintf(stderr, "Size after write: %lld bytes\n", (long long)sb.st_size);
    if(rc < 0)
    {
        perror("fprintf stderr failed");
        failed = 1;
        goto cleanup;
    }

cleanup:
    /* Restore stdout if redirected and we have the saved fd. */
    if(stdout_redirected && have_saved_stdout_fd)
    {
        r_dup2 = dup2(saved_stdout_fd, STDOUT_FILENO);
        if(r_dup2 == -1)
        {
            perror("final restore of stdout failed");
            failed = 1;
        }
    }

    /* Close saved stdout fd if open. */
    if(have_saved_stdout_fd)
    {
        r_close = close(saved_stdout_fd);
        if(r_close == -1)
        {
            perror("close(saved_stdout_fd) failed");
            failed = 1;
        }
    }

    /* Close temp file if open. */
    if(have_temp_fd)
    {
        r_close = close(temp_fd);
        if(r_close == -1)
        {
            perror("close(temp_fd) failed");
            failed = 1;
        }
    }

    /* Unlink the pathname if created. */
    if(templ[0] != '\0')
    {
        r_unlink = unlink(templ);
        if(r_unlink == -1 && errno != ENOENT)
        {
            perror("unlink(temp file) failed");
            failed = 1;
        }
    }

    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
