# Нити C++ 2011

## Пререквезиты

Материал данного семинара подразумевает углубленным владением материала
по языку C++ в рамках следующих тем:

 1. [Move-семантика](cxx11_move.md)
 2. [Вызываемые объекты](cxx11_callable.md)
 3. [Анонимные функции](cxx11_lambda.md)


## Класс std::thread

Класс [std::thread](http://www.cplusplus.com/reference/thread/thread/),
объявленный в заголовочном файле `<thread>` реализует нить выполнения.

Конструктор класса принимает переменное количество аргументов, первым из
которых является [вызываемый объект](cxx11_callable.md), остальные аргументы
передаются в качестве аргументов вызова.

Вызов конструктора `std::thread` эквивалентен вызову функции `pthread_create`,
то запуск новой нити происходит **при создании переменной класса std::thread**.

Ожидание завершения работы нити осуществляется вызовом метода `.join()`.

```cpp
#include <iostream>
#include <thread>

void threadFunction()
{
  std::cout << "I'm a function from other thread" << std::endl;
  std::cout.flush();
}

int main()
{
  std::thread thr(threadFunction);
  thr.join();
  return 0;
}
```

Передавать аргументы в вызываемую функцию можно тремя способами:

 1. Передачей аргументов в конструктор `std::thread(...)`
 2. Созданием связанной с помощью `std::bind()` функции. См.
 [дополнительный материал](cxx11_callable.md)
 3. Использованием анонимных функций. См.
 [дополнительный материал](cxx11_lambda.md).

Пример передачи аргументов через конструктор:

```cpp
#include <functional>
#include <iostream>
#include <string>
#include <thread>

void hello(const std::string & name, int & result)
{
  const std::string message = "Hello, " + name + "!";
  std::cout << message << std::endl;
  std::cout.flush();
  result = message.length();
}

int main()
{
  static const std::string name = "World";
  int message_length;
  std::thread thr(hello, name, std::ref(message_length));
  thr.join();
  std::cout << "Message length: " << message_length << std::endl;
  return 0;
}
```

## Классы `std::mutex` и `std::lock_guard`

Простой (*не рекурсивный*) *мьютекс* - это объект блокировки,
который может находиться в одном из двух состояний: заблокирован
или разблокирован.

Если мьютекс заблокирован, то попытка его повторной блокировки приводит
к приостановке работы текущей нити до тех пор, пока он не будет
разблокирован той нитью, которая его ранее заблокировала.

Разблокировать мьютекс обязана **та же самая нить**, которая его заблокировала.

Мьютексы используются для "защиты" совместно используемых различными нитями
статических переменных и блоков данных.

При работе с мьютексами рекомендуется, во избежание простоя других нитей,
минимизировать время, когда мьютекс находится в заблокированном состоянии.
То есть, расстояние в программе между вызовами блокировки и разблокировки
должно быть как можно меньше.

В стандартной библиотеке C++ 2011 мьютекс реализован с помощью класса
[std::mutex](http://www.cplusplus.com/reference/mutex/mutex/), который
объявлен в заголовочном файле `<mutex>`.

Основные методы, которые реализует объект этого класса:

 1. `void .lock()` - выполняет блокировку (захват) мьютекса
 2. `void .unlock()` - выполняет разблокировку (освобождение) мьютекса
 3. `bool .try_lock()` - выполняет блокировку, но в случае не успеха не
 приостанавливает выполнение текущей нити. Обычно используется для "сброса"
 состояния:

```cpp
#include <mutex>

std::mutex mx;

void resetMutexState()
{
  mx.try_lock();
  mx.unlock();
}
```

Ситуация, когда мьютекс заблокирован, а в последствии не разблокирован
является трудно отлавливаемой ошибкой. Обычно такие проблемы возникают,
когда мьютекс блокируется в начале функции, обрабатывающей критическую
секцию, и разблокируется в конце, но не разблокируется в случае
принудительного выхода с помощью `return`:

Пример **некорректной** программы:
```cpp
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <mutex>
#include <string>
#include <vector>

std::mutex        mx;
std::vector<int>  shared_data_to_protect;

void criticalSectionProcessing(const std::string &filename)
{
  // Начало критической секции
  mx.lock();

  std::ifstream in(filename);
  if (!in.is_open()) {
    std::cerr << "Can't open file: " << filename << std::endl;
    return; // ОШИБКА: мьютекс остается заблокирован
  }

  std::copy(
    std::istream_iterator<int>(in),
    std::istream_iterator<int>(),
    std::back_inserter(shared_data_to_protect)
  );

  in.close();

  // Конец критической секции
  mx.unlock();
}
```

Для предотвращения подобных ситуаций необходимо гарантировать, что мьютекс
будет освобожден при выходе из функции. Это можно сделать,
если использовать некоторый впомогательный объект, который при создании
захватывает мьюьтекс, и освобождает мьютекс из своего деструктора.

В стандартной библиотеке такой объект уже объявлен и называется
[std::lock_guard](http://www.cplusplus.com/reference/mutex/lock_guard/).

С использованием `lock_guard` приведенный выше пример может быть
реализован следующим образом:

```cpp
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <mutex>
#include <string>
#include <vector>

std::mutex        mx;
std::vector<int>  shared_data_to_protect;

void criticalSectionProcessing(const std::string &filename)
{
  // Начало критической секции
  std::lock_guard<std::mutex> lock(mx);

  std::ifstream in(filename);
  if (!in.is_open()) {
    std::cerr << "Can't open file: " << filename << std::endl;
    return; // OK. Мьютекс будет разблокирован вследтствии
            // удаления переменной lock
  }

  std::copy(
    std::istream_iterator<int>(in),
    std::istream_iterator<int>(),
    std::back_inserter(shared_data_to_protect)
  );

  in.close();

  // Конец критической секции
  // Ничего разблокировать явным образом не нужно:
  // это происходит при нявном вызове деструктора
  // переменной lock
}
```
