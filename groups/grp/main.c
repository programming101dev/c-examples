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

#include <grp.h>
#include <stdio.h>
#include <stdlib.h>

static void print_entries(void);
static void print_entry(const struct group *entry);

int main(void)
{
    const struct group *entry;

    // Use getgrent to retrieve all group information
    printf("All group accounts:\n");
    print_entries();

    // Rewind the group database
    setgrent();
    entry = getgrent();    // cppcheck-suppress getgrentCalled

    if(entry != NULL)
    {
        print_entry(entry);
    }

    endgrent();

    return EXIT_SUCCESS;
}

static void print_entries(void)
{
    const struct group *entry;

    while((entry = getgrent()) != NULL)    // cppcheck-suppress getgrentCalled
    {
        print_entry(entry);
    }
}

static void print_entry(const struct group *entry)
{
    printf("Group name: %s\n", entry->gr_name);
    printf("Group ID (GID): %u\n", entry->gr_gid);
    printf("Group Members:\n");

    if(entry->gr_mem != NULL)
    {
        for(int i = 0; entry->gr_mem[i] != NULL; i++)
        {
            printf(" - %s\n", entry->gr_mem[i]);
        }

        printf("-------------------------\n");
    }
    else
    {
        printf("No members found.\n");
    }
}
