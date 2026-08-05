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

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>

static void print_entries(void);
static void print_entry(const struct passwd *entry);

int main(void)
{
    const struct passwd *entry;

    // Use getpwent to retrieve all user account information
    printf("All user accounts:\n");
    print_entries();

    // Rewind the user database
    setpwent();
    entry = getpwent();    // cppcheck-suppress getpwentCalled

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
    const struct passwd *entry;

    while((entry = getpwent()) != NULL)    // cppcheck-suppress getpwentCalled
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
