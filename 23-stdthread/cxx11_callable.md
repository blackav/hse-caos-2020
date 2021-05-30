# Вызываемы объекты

Вызываемые объекты - это функции модуля, методы класса или объекта, а также
экземпляры любого класса, для которых определен оператор `()`.

## Метод `operator()(...)`

Любому объекту класса можно определить оператор выполнения. После этого, запись
имени переменной класса, после которой следуют скобки, (и возможно, аргументы),
означает выполнение некоторого кода.

```cpp
#include <iostream>

class Callable {
public:
  int field;
  void operator()() {
    std::cout << "I'm callable with value " << field << std::endl;
  }

  // Можно объявлять аргументы
  void operator()(int value) {
    std::cout << value << " + " << field << " = " << (value+field) << std::endl;
  }
};

int main()
{
  Callable call_me;
  call_me.field = 123; // call_me - это обычный объект класса
  call_me();     // вызов объекта ничем не отличается от вызова функции
  call_me(456);  // вывод: 579
  return 0;
}
```

## Использование вызываемых объектов в качестве аргументов других функций

Функции, который принимают в качестве аргументов другие функции, либо
вызываемые объекты, называются функциями *высшего порядка*.

Использование функций в качестве аргументов может применяться для:

 1. Выполнения в отдельной нити
 2. Указания callback-обработчиков

## Функции и статические методы без аргументов

Проще всего использовать передачу в качестве аргумента функций, которые
не принимают никаких аргументов, и не связаны с конкретным экземмпляром класса:

```cpp
#include <thread>
#include <iostream>

void func()
{
  std::cout << "Hello!" << std::endl;
}

int main()
{
  std::thread(func) // выполняется func в отдельной нити
    .join();
  return 0;
}
```

Методы класса ничем не отличаются от обычных функций в том случае, если
они не связаны с каким-либо объектом, то есть являются *статическими*.

```cpp
#include <thread>
#include <iostream>

class A {
public:
  static void method();
};

void A::method()
{
  std::cout << "Hello!" << std::endl;
}

int main()
{
  std::thread(A::method) // выполняется метод method класса A в отдельной нити
    .join();
  return 0;
}
```

## Связывание функции с аргументами

Функция `std::bind` возвращает вызываемый объект, который получается связыванием
некоторой другой функции (или вызываемого объекта) с заданными аргументами.

```cpp
#include <thread>
#include <functional>
#include <iostream>

void func(int a, int b)
{
  std::cout << (a+b) << std::endl;
  std::cout.flush();
}

int main()
{
  // func_10_20 и func_10_5 -- это два функциональных объекта
  // которые можно вызывать
  auto func_10_20 = std::bind(func, 10, 20);
  auto func_10_5  = std::bind(func, 10, 5);

  func_10_20();
  func_10_5();

  // Запуск нити, которая выполняет func(10, 20)
  std::thread a(func_10_20);

  // Запуск нити, которая выполняет func(10, 5);
  std::thread b(func_10_5);

  a.join();
  b.join();
  return 0;
}
```

Можно связывать не все аргументы, а только часть из них:

```cpp
#include <functional>
#include <iostream>

int add(int x, int y) { return x + y; }
int add_mul(int x, int y, int scale) { return (x+y) * scale; }

int main()
{
  // Имена _1, _2 и т. д. (см. их использование ниже)
  using namespace std::placeholders;

  // increment -- это функция, которая вызывает add, передавая в качестве
  // первого параметра значение 1, а в качестве второго -- значение своего
  // первого аргумента
  auto increment = std::bind(add, 1, _1);

  // функфия add_5 прибавляет число 5 к своему аргументу
  auto add_5 = std::bind(add, 5, _1);

  // функция scaled_summ возвращает сумму, умноженную на 10
  auto scaled_summ = std::bind(add_mul, _1, _2, 10);

  std::cout << increment(1) << std::endl;       // 2
  std::cout << add_5(10) << std::endl;          // 15
  std::cout << scaled_summ(1, 2) << std::endl;  // 30
  return 0;
}
```

Нестатический метод класса можно превратить в функцию, которая в качестве
первого аргумента принимает объект класса. Это преобразование осуществляется
с помощью `std::mem_fn`:

```cpp
#include <iostream>
#include <thread>
#include <functional>

class Runnable {
  int field_;
public:
  explicit Runnable(int arg): field_(arg) {}
  void run() {
    std::cout << "I'm runnable with field = " << field_ << std::endl;
    std::cout.flush();
  }
};

int main()
{
  Runnable instance_1(10);
  Runnable instance_2(15);

  // Запуск в отдельных нитях обработчиков для instance_1 и instance_2
  std::thread a(std::bind(std::mem_fn(&Runnable::run), instance_1));
  std::thread b(std::bind(std::mem_fn(&Runnable::run), instance_2));

  a.join();
  b.join();

  return 0;
}
```

## Аргументы-ссылки

Для связывания параметров-ссылок, их нужно предварительно упаковать в специальную
структуру `std::ref`, либо `std::cref` - для константных ссылок.

```cpp
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <string>
#include <thread>
#include <vector>

// Вычисление частичной суммы чисел в заданном диапазоне.
// Первый и последний аргументы передаются по ссылке:
//   1) сам вектор передается по (константной) ссылке для того, чтобы
//      не копировать все его элементы при вызове функции;
//   2) последний параметр передается по ссылке, так как является
//      параметром-результатом.
void process(const std::vector<int> &vec, size_t from, size_t to, int &result)
{
  result = std::accumulate(vec.cbegin()+from, vec.cbegin()+to, 0);
}

// Реализация суммирования чисел, используя заданное через аргумент
// командной строки количество нитей. Гарантируется, что число нитей не превышает
// количество чисел во входных данных.
// На экран выводится сумма всех чисел, затем - частичные суммы.
int main(int argc, char *argv[])
{
  using namespace std;

  // Чтение чисел с потока stdin
  vector<int> input_data;
  copy(istream_iterator<int>(cin), istream_iterator<int>(), back_inserter(input_data));

  // Определение числа нитей из первого аргумента командной строки
  const int num_threads = stoi(argv[1]);
  const size_t nums_per_thread = input_data.size() / num_threads;
  list<thread> threads;
  vector<int> partial_sums(num_threads, 0);

  for (int i=0; i<num_threads; ++i) {
    const size_t from = i * nums_per_thread;
    const size_t to = num_threads-1!=i ? (i+1)*nums_per_thread : input_data.size();

    // Обратите внимание:
    // 1. Аргумент типа const vector& упаковывается в std::cref(...)
    // 2. Аргумент типа int& упаковывается в std::ref(...)
    threads.push_back(thread(bind(process, cref(input_data), from, to, ref(partial_sums[i]))));
  }

  for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
  cout << accumulate(partial_sums.cbegin(), partial_sums.cend(), 0) << endl;
  copy(partial_sums.cbegin(), partial_sums.cend(), ostream_iterator<int>(cout,"\n"));

  return 0;
}
```
