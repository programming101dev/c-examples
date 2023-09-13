#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void)
{
    FILE *input_devices;
    char line[1024];
    char *event_file;
    char *device_name;
    bool inside_device_section;

    input_devices = fopen("/proc/bus/input/devices", "r");

    if(input_devices == NULL)
    {
        perror("Error opening /proc/bus/input/devices");
        return EXIT_FAILURE;
    }

    printf("Event files and names for devices with 'sysrq' and 'kbd' but not 'mouse':\n");
    device_name = NULL;
    inside_device_section = 0;

    while(fgets(line, sizeof(line), input_devices) != NULL)
    {
        if(strstr(line, "N: Name=") != NULL)
        {
            char *name;

            name = strstr(line, "N: Name=") + strlen("N: Name=");
            name[strlen(name) - 1] = '\0';
            device_name = strdup(name);
        }
        else if(strstr(line, "Handlers=sysrq kbd") != NULL && strstr(line, "mouse") == NULL)
        {
            if(device_name != NULL)
            {
                event_file = strstr(line, "event");

                if(event_file != NULL)
                {
                    char event_number[16];
                    int i = 0;

                    while(event_file[i] != ' ' && event_file[i] != '\n' && event_file[i] != '\0')
                    {
                        event_number[i] = event_file[i];
                        i++;
                    }

                    event_number[i] = '\0';
                    printf("/dev/input/event%s: %s\n", event_number, device_name);
                    device_name = NULL;
                }
            }
        }
        else if (line[0] == '\n')
        {
            inside_device_section = false;
        }
    }

    fclose(input_devices);

    return EXIT_SUCCESS;
}
