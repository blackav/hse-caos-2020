## Сокеты

[Лекция 33 - socket](../00-lectures/32-signal/lecture32.pdf)

[Конспект материала](sem-socket.pdf)

### Использование функции getaddrinfo

Функция getaddrinfo выполняет преобразование адреса хоста и имени сервиса из строкового представление во внутренний
формат, пригодный для передачи в функции `connect` или `bind`. Поддерживается числовая запись IP-адреса и запись
имени сервиса как номера порта.
Она заменяет собой функции `gethostbyname`, `gethostbyaddr`, которые в настоящее время считаются устаревшими.

Для использования функции необходимо подключение заголовочных файлов
```
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
```

Функция работает со структурой `struct addrinfo`, описанной следующим образом:

```
struct addrinfo
{
    int              ai_flags;     // флаги для использования в hints
    int              ai_family;    // для TCP/IP v4 здесь AF_INET, используется в hints
    int              ai_socktype;  // для TCP здесь SOCK_STREAM, используется в hints
    int              ai_protocol;  // уточнение протокола, обычно 0
    socklen_t        ai_addrlen;   // длина адреса для передачи в параметре addrlen в connect или bind
    struct sockaddr *ai_addr;      // адрес для передачи в connect или bind
    char            *ai_canonname;
    struct addrinfo *ai_next;      // указатель на следующий элемент
};
```

Функция `getaddrinfo` определена следующим образом:

```
int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
```

Здесь параметр `node` - имя хоста, `service` - имя сервиса или номер порта.

В параметре `hints` передаются параметры для преобразования имени в адрес. В параметре `hints` необходимо задать
поля `ai_flags`, `ai_family`, `ai_socktype`, `ai_protocol`. Поле `ai_flags` можно установить равным 0,
а полях `ai_family`, `ai_socktype`, `ai_protocol` нужно передать те же самые параметры, что и в системный вызов `socket`.

Таким образом фрагмент инициализации переменной `hints` может выглядеть следующим образом:
```
    struct addrinfo hints = {}; // обнуляем структуру
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
```

Или с использованием designated initializers C11 следующим образом:
```
    struct addrinfo hints = { .ai_family = AF_INET, .ai_socktype = SOCK_STREAM };
```

Функция `getaddrinfo` возвращает 0 при успешном преобразовании и ненулевой код ошибки при ошибке.
Этот код ошибки можно передать в функцию `gai_strerror` для преобразования в строку для вывода сообщения об ошибке.

```
    int err = getaddrinfo(host, service, &hints, &result);
    if (err) {
        fprintf(stderr, "error: %s\n", gai_strerror(err));
    }
```

В случае успешного выполнения (когда `err == 0`) в переменную `result` записывается указатель на первый элемент
списка адресов, соответствующих хосту. Одному и тому же хосту может соответствовать как один IP-адрес (в типичной
ситуации), так и несколько IP-адресов для высоконагруженных серверов (например, google.com).
Перемещаться по этому списку можно с помощью поля `ai_next`.

Если необходимо обратиться к полям адреса для получения IP-адреса или номера порта, необходимо
преобразовать обобщенный указатель типа `struct sockaddr` в указатель `struct sockaddr_in`.
Затем уже можно работать с полями `sin_addr` и `sin_port`. Например, для вывода информации об адресе можно
использовать следующий фрагмент:

```
    struct sockaddr_in *ain = (struct sockaddr_in *) result->ai_addr;
    printf("IP = %s, PORT = %s\n", inet_ntoa(ain->sin_addr), ntohs(ain->sin_port));
```

Для передачи результата работы `getaddrinfo` в системный вызов `connect` поля `ai_addrlen` и `ai_addr`
передаются без изменений. Например,

```
    connect(fd, result->ai_addr, result->ai_addrlen);
```

После завершения работы со списком необходимо не забыть освободить занимаемую память
с помощью `freeaddrinfo`:

```
    freeaddrinfo(result);
```

### Настройки серверного порта

Чтобы программу, открывающую сокет в режиме прослушивания, можно было запустить немедленно после завершения
с тем же самым номером прослушивающего порта, в программу должна быть добавлена следующая последовательность
вызовов, настраивающих опции сокета:

```
    int val = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val));
```

Эта последовательность должна находиться после системного вызова `socket`, но до системного вызова `bind`.
