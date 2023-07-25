#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
    printf("User ID (UID): %d\n", entry->pw_uid);
    printf("Group ID (GID): %d\n", entry->pw_gid);
    printf("Home directory: %s\n", entry->pw_dir);
    printf("Login shell: %s\n", entry->pw_shell);
    printf("-------------------------\n");
}
