
# Системный вызов `mmap`

[Лекция](lecture23.pdf)

[Пример](mmap1.c) использования mmap в режиме MAP_SHARED - можно запустить параллельно несколько процессов, и они будут видеть изменения других процессов.

[Пример](mmap2.c) использования mmap в режиме MAP_ANONYMOUS | MAP_SHARED -
можно запустить параллельно несколько процессов, но каждый процесс будет работать со своими данными.

[Пример](mmap3.c) использования mmap в режиме MAP_ANONYMOUS | MAP_SHARED -
будет создан сыновний процесс, и родитель и сын будут читать и модифицировать общие данные.

[Пример](mmap4.c) использования mmap в режиме MAP_ANONYMOUS | MAP_PRIVATE
будет создан сыновний процесс, родитель и сын будут работать со своими данными.

[Пример](mmap5.c) использования mmap в режиме MAP_PRIVATE - изменения в одном процессе не будут видны в других процессах. Обратите внимание, что файл открыт O_RDONLY, а mmap выполняется с правами PROT_READ | PROT_WRITE.

[Пример](mmap6.c) использования mmap в режиме MAP_PRIVATE - изменения в одном процессе не будут видны в других процессах. Обратите внимание, что неспотря на то, что файл открыт O_RDWR, но измененные данные в файл не записываются.

[Пример](mmap7.c) использования mmap в режиме MAP_SHARED - файл открыт в режиме "только для чтения", а mmap пытается подключить его как "чтение-запись", и mmap завершится с ошибкой EPERM - Permission denied.

# Файловая система /proc

[Описание /proc/\[pid\]/maps](http://stackoverflow.com/questions/1401359/understanding-linux-proc-id-maps/1401595#1401595) и [справка по procfs](http://man7.org/linux/man-pages/man5/proc.5.html).

# elf

[man 5 elf](http://ru.manpages.org/elf/5)

Поясняющие картинки by corkami (кликабельно):

[<img src="http://i.imgur.com/esWHRRe.png" width="400"/>](http://i.imgur.com/esWHRRe.png) [<img src="http://i.imgur.com/GZ5a0sb.png" width="400"/>](http://i.imgur.com/GZ5a0sb.png)
