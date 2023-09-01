/*
 * This code is licensed under the Attribution-NonCommercial-NoDerivatives 4.0 International license.
 *
 * Author: D'Arcy Smith (ds@programming101.dev)
 *
 * You are free to:
 *   - Share: Copy and redistribute the material in any medium or format.
 *   - Under the following terms:
 *       - Attribution: You must give appropriate credit, provide a link to the license, and indicate if changes were made.
 *       - NonCommercial: You may not use the material for commercial purposes.
 *       - NoDerivatives: If you remix, transform, or build upon the material, you may not distribute the modified material.
 *
 * For more details, please refer to the full license text at:
 * https://creativecommons.org/licenses/by-nc-nd/4.0/
 */


#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>


void logMessages(void);


int main(void)
{
    int mask;

    openlog("log-demo", LOG_PID, LOG_USER);

    mask = LOG_MASK(LOG_EMERG) | LOG_MASK(LOG_ALERT) | LOG_MASK(LOG_CRIT) |
               LOG_MASK(LOG_ERR) | LOG_MASK(LOG_WARNING) | LOG_MASK(LOG_NOTICE) |
               LOG_MASK(LOG_INFO) | LOG_MASK(LOG_DEBUG);

    if(setlogmask(mask) == -1)
    {
        syslog(LOG_ERR, "Failed to set log mask");
        closelog();
        return EXIT_FAILURE;
    }

    printf("Logging PID %d\n", getpid());

    logMessages();

    mask = LOG_MASK(LOG_EMERG);

    if(setlogmask(mask) == -1)
    {
        syslog(LOG_ERR, "Failed to set log mask");
        closelog();
        return EXIT_FAILURE;
    }

    logMessages();
    closelog();
    return EXIT_SUCCESS;
}


void logMessages(void)
{
    syslog(LOG_EMERG, "This is an emergency message.");
    syslog(LOG_ALERT, "This is an alert message.");
    syslog(LOG_CRIT, "This is a critical message.");
    syslog(LOG_ERR, "This is an error message.");
    syslog(LOG_WARNING, "This is a warning message.");
    syslog(LOG_NOTICE, "This is a notice message.");
    syslog(LOG_INFO, "This is an informational message.");
    syslog(LOG_DEBUG, "This is a debug message.");
}
