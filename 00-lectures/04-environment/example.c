#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // argv[0] - это имя программы, которое использовалось в командной строке запуска
    for (int i = 0; i < argc; ++i) {
        printf("[%d]: %s\n", i, argv[i]);
    }

    if (argc < 2) {
        // сообщения об ошибках должны выводиться на stderr
        // в конце сообщения об ошибке должен быть символ \n
        fprintf(stderr, "not enough args\n");
        // в случае ошибки должен быть ненулевой код завершения
        // константа EXIT_FAILURE равна 1 и может использоваться для повышения читаемости программы
        exit(EXIT_FAILURE);
    }
    FILE *f = fopen(argv[1], "r");
    // необходимо проверять результат выполнения всех библиотечных функций и системных вызовов
    if (!f) {
        fprintf(stderr, "fopen failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // открытые файлы и аналогичные ресурсы должны закрываться перед завершением программы (кроме std потоков)
    fclose(f);
    char *s = getenv("PATH");
    if (s) {
        printf("PATH=%s\n", s);
    }

    char *y = strdup(s);

    // динамическая память должна освобождаться
    free(y);
}
