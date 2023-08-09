#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>


static void print_flags(const char *flag_name, tcflag_t flags, tcflag_t flag_to_check);


int main(void)
{
    int fd = STDIN_FILENO; // Use stdin file descriptor for this example
    struct termios term_attr;

    // Get the current terminal attributes
    if(tcgetattr(fd, &term_attr) == -1)
    {
        perror("tcgetattr");
        return 1;
    }

    printf("Terminal attributes:\n");

    // Input flags
    printf("Input flags:  %X\n", term_attr.c_iflag);
    print_flags("BRKINT", term_attr.c_iflag, BRKINT);
    print_flags("ICRNL", term_attr.c_iflag, ICRNL);
    print_flags("IGNBRK", term_attr.c_iflag, IGNBRK);
    print_flags("IGNCR", term_attr.c_iflag, IGNCR);
    print_flags("IGNPAR", term_attr.c_iflag, IGNPAR);
    print_flags("INLCR", term_attr.c_iflag, INLCR);
    print_flags("INPCK", term_attr.c_iflag, INPCK);
    print_flags("ISTRIP", term_attr.c_iflag, ISTRIP);
    print_flags("IXANY", term_attr.c_iflag, IXANY);
    print_flags("IXOFF", term_attr.c_iflag, IXOFF);
    print_flags("IXON", term_attr.c_iflag, IXON);
    print_flags("PARMRK", term_attr.c_iflag, PARMRK);
    printf("\n");

    // Output flags
    printf("Output flags: %X\n", term_attr.c_oflag);
    print_flags("OPOST", term_attr.c_oflag, OPOST);
    print_flags("ONLCR", term_attr.c_oflag, ONLCR);
    print_flags("OCRNL", term_attr.c_oflag, OCRNL);
    print_flags("ONOCR", term_attr.c_oflag, ONOCR);
    print_flags("ONLRET", term_attr.c_oflag, ONLRET);
    print_flags("OFDEL", term_attr.c_oflag, OFDEL);
    print_flags("OFILL", term_attr.c_oflag, OFILL);
    print_flags("NLDLY", term_attr.c_oflag, NLDLY);
    print_flags("CRDLY", term_attr.c_oflag, CRDLY);
    print_flags("TABDLY", term_attr.c_oflag, TABDLY);
    print_flags("BSDLY", term_attr.c_oflag, BSDLY);
    print_flags("VTDLY", term_attr.c_oflag, VTDLY);
    print_flags("FFDLY", term_attr.c_oflag, FFDLY);
    printf("\n");

    // Control flags
    printf("Control flags: %X\n", term_attr.c_cflag);
    print_flags("CSIZE CS5", term_attr.c_cflag, CS5);
    print_flags("CSIZE CS6", term_attr.c_cflag, CS6);
    print_flags("CSIZE CS7", term_attr.c_cflag, CS7);
    print_flags("CSIZE CS8", term_attr.c_cflag, CS8);
    print_flags("CSTOPB", term_attr.c_cflag, CSTOPB);
    print_flags("CREAD", term_attr.c_cflag, CREAD);
    print_flags("PARENB", term_attr.c_cflag, PARENB);
    print_flags("PARODD", term_attr.c_cflag, PARODD);
    print_flags("HUPCL", term_attr.c_cflag, HUPCL);
    print_flags("CLOCAL", term_attr.c_cflag, CLOCAL);
    printf("\n");

    // Local flags
    printf("Local flags: %X\n", term_attr.c_lflag);
    print_flags("ECHO", term_attr.c_lflag, ECHO);
    print_flags("ECHOE", term_attr.c_lflag, ECHOE);
    print_flags("ECHOK", term_attr.c_lflag, ECHOK);
    print_flags("ECHONL", term_attr.c_lflag, ECHONL);
    print_flags("ICANON", term_attr.c_lflag, ICANON);
    print_flags("IEXTEN", term_attr.c_lflag, IEXTEN);
    print_flags("ISIG", term_attr.c_lflag, ISIG);
    print_flags("NOFLSH", term_attr.c_lflag, NOFLSH);
    print_flags("TOSTOP", term_attr.c_lflag, TOSTOP);
    printf("\n");

    // Print control characters
    printf("Control characters:\n");
    printf("VEOF:    %d\n", term_attr.c_cc[VEOF]);
    printf("VEOL:    %d\n", term_attr.c_cc[VEOL]);
    printf("VERASE:  %d\n", term_attr.c_cc[VERASE]);
    printf("VINTR:   %d\n", term_attr.c_cc[VINTR]);
    printf("VKILL:   %d\n", term_attr.c_cc[VKILL]);
    printf("VMIN:    %d\n", term_attr.c_cc[VMIN]);
    printf("VQUIT:   %d\n", term_attr.c_cc[VQUIT]);
    printf("VSTART:  %d\n", term_attr.c_cc[VSTART]);
    printf("VSTOP:   %d\n", term_attr.c_cc[VSTOP]);
    printf("VSUSP:   %d\n", term_attr.c_cc[VSUSP]);
    printf("VTIME:   %d\n", term_attr.c_cc[VTIME]);

    return EXIT_SUCCESS;
}


static void print_flags(const char *flag_name, tcflag_t flags, tcflag_t flag_to_check)
{
    if(flags & flag_to_check)
    {
        printf("\t%s: Set\n", flag_name);
    }
    else
    {
        printf("\t%s: Not Set\n", flag_name);
    }
}
