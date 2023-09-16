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
#include <pwd.h>


static void print_entries(void);
static void print_entry(const struct passwd *entry);


int main(void)
{
    struct passwd *entry;

    // Use getpwent to retrieve all user account information
    printf("All user accounts:\n");
    print_entries();

    // Rewind the user database
    setpwent();
    entry = getpwent();

    if(entry != NULL)
    {
        print_entry(entry);
    }

    // Close the user database after usage
    endpwent();

    return EXIT_SUCCESS;
}


static void print_entries(void)
{
    struct passwd *entry;

    while((entry = getpwent()) != NULL)
    {
        print_entry(entry);
    }
}


static void print_entry(const struct passwd *entry)
{
    printf("Username: %s\n", entry->pw_name);
    printf("User ID (UID): %u\n", entry->pw_uid);
    printf("Group ID (GID): %u\n", entry->pw_gid);
    printf("Home directory: %s\n", entry->pw_dir);
    printf("Login shell: %s\n", entry->pw_shell);
    printf("-------------------------\n");
}
