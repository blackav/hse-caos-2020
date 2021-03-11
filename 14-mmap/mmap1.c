#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

/*
пример использования mmap в режиме MAP_SHARED
можно запустить параллельно несколько процессов, они
будут видеть изменения других процессов
 */

/*
использование ./mmap1 FILE PAGE
FILE - имя файла
PAGE - номер страницы в файле, с которой будем работать
 */
int main(int argc, char *argv[])
{
    int page = strtoll(argv[2], NULL, 0);
    int pagesize = getpagesize();
    printf("page size: %d\n", pagesize);
    off_t offset = (off_t) page * pagesize;

    int fd = open(argv[1], O_RDWR, 0);
    printf("fd: %d\n", fd);
    void *mp = mmap(NULL, pagesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
    printf("ptr: %p\n", mp);
    printf("pid: %d\n", getpid());
    int *data = mp;
    while (1) {
        printf("%d, %d, %d\n", data[0], data[1], data[2]);
        struct timeval tt;
        gettimeofday(&tt, NULL);
        data[0] = getpid();
        data[1] = tt.tv_sec;
        data[2] = tt.tv_usec;
        sleep(1);
    }
}
