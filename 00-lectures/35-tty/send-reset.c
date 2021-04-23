#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <termios.h>

/*
usage: send-reset TTY-FILE
 */
int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "tty special file expected\n");
        return 1;
    }

    // O_NOCTTY для того, чтобы открываемый файл не становился управляющим терминалом процесса
    int fd = open(argv[1], O_RDWR | O_NOCTTY);
    if (fd < 0) {
        fprintf(stderr, "open failed: %s\n", strerror(errno));
        return 1;
    }

    struct termios ti = {};
    if (tcgetattr(fd, &ti) < 0) {
        fprintf(stderr, "tcgetattr failed: %s\n", strerror(errno));
        return 1;
    }

    // настраиваем терминал исходя из того, что текущее состояние неизвестно
    // скорость ввода-вывода - 38400 (стандартная)
    cfsetospeed(&ti, B38400);
    cfsetispeed(&ti, B38400);

    // 8 bit, no parity, 1 stop bit
    ti.c_cflag &= ~(CSIZE | CSTOPB | PARENB | PARODD | HUPCL | CLOCAL | CMSPAR | CRTSCTS);
    ti.c_cflag |= CS8;

    // сбрасываем ненужное
    ti.c_iflag &= ~(BRKINT | PARMRK | INPCK | ISTRIP | INLCR | IGNCR | ICRNL | IUCLC | IXON | IXANY | IUTF8);

    ti.c_oflag &= ~(OPOST | OLCUC | ONLCR | OCRNL | ONOCR | ONLRET | OFILL | OFDEL | NLDLY | CRDLY | TABDLY | BSDLY | VTDLY | FFDLY);

    ti.c_lflag &= ~(ISIG | ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOCTL | ECHOPRT | ECHOKE | NOFLSH | TOSTOP | IEXTEN);

    // устанавливаем стандартный режим блокирующего чтения
    ti.c_cc[VMIN] = 1;
    ti.c_cc[VTIME] = 0;

    // настраиваем терминал
    if (tcsetattr(fd, TCSAFLUSH, &ti) < 0) {
        fprintf(stderr, "tcsetattr: %s\n", strerror(errno));
        return 1;
    }

    static const char resetcmd[] = "AT+RESET \r\n";
    int sz = write(fd, resetcmd, sizeof(resetcmd) - 1);
    if (sz < 0) {
        fprintf(stderr, "write: %s\n", strerror(errno));
        return 1;
    }
    printf("written: %d\n", sz);

    char buf[1024];
    while (1) {
        sz = read(fd, buf, sizeof(buf) - 1);
        if (sz < 0) {
            fprintf(stderr, "read: %s\n", strerror(errno));
            return 1;
        }
        buf[sz] = 0;
        printf("%d: <%s>\n", sz, buf);
    }
}
