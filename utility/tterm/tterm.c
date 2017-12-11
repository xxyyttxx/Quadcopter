/*
    an easy POSIX series program.
    author: (C)tjua, ES-SS-UESTC, 171129.
    without any lisence.
*/

#ifndef static_Band
#error static_Band should be defined
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>

#define exit_ifn(x) ((void)((x)||(fprintf(stderr, "%s line %d:  ", argv[0], __LINE__), perror(""), exit(EXIT_FAILURE), 0)))

int main(int argc, char *argv[])
{
    int fd;
    struct termios st;
    char buf[1000];
    fd_set readfds;

#ifndef static_termfile
#define termfile_path argv[1]
    if (argc != 2) {
        puts("usage: ./tterm path");
        return 0;
    }
#else
#define termfile_path static_termfile
#endif

    {/* 打开串口并初始化 */
        exit_ifn(-1 != (fd = open(termfile_path, O_RDWR)));

        tcgetattr(fd, &st);

        cfsetispeed(&st, B38400);
        cfsetospeed(&st, B38400);
        st.c_cflag &= ~CSTOPB&~PARENB;
        st.c_lflag &= ~ICANON&~ECHO&~ECHOE&~ISIG;

        tcsetattr(fd, TCSANOW, &st);
    }

    /* 死循环+多路复用 */
    FD_ZERO(&readfds);
    for (;;) {
        int flags = 0;
        FD_SET(0, &readfds);
        FD_SET(fd, &readfds);
        exit_ifn(-1 != select(fd+1, &readfds, NULL, NULL, NULL));

        if (FD_ISSET(0, &readfds)) {
            exit_ifn(read(0, buf, 1) == 1);
            write(fd, buf, 1);
            flags = 1;
            FD_CLR(0, &readfds);
        }
        if (FD_ISSET(fd, &readfds)) {
            exit_ifn(read(fd, buf, 1) == 1);
            write(1, buf, 1);
            flags = 1;
            FD_CLR(fd, &readfds);
        }
        exit_ifn(flags);
    }
}
