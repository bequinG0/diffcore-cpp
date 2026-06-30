# diffcore-cpp

Библиотека численных методов на C++ для решения обыкновенных дифференциальных уравнений и краевых задач.

## Возможности

- **Задача Коши** — явные методы (Эйлер, Рунге-Кутта 2/4 порядка) с автоматическим выбором направления шага
- **Краевые задачи** — интегро-интерполяционный метод, второй порядок аппроксимации, поддержка граничных условий 1-го (Дирихле), 2-го (Неймана) и 3-го (Робина) рода
- **Интерполяция** — полином Лагранжа, полином Ньютона (разделённые разности), кубический сплайн
- **Задача на собственные значения** — приведение обобщённой задачи к стандартной, симметризация, поиск минимальных собственных чисел и собственных функций (через Eigen)
- **Визуализация** — построение графиков через Gnuplot
- **Анализ сходимости** — правило Рунге, оценка порядка точности, чебышёвская норма погрешности

## Зависимости

- **C++20** (gcc 12+ или clang 16+)
- **Eigen 3.4+** — линейная алгебра и разреженные решатели
- **Gnuplot** — для построения графиков (опционально)

### Установка зависимостей

```bash
# Arch Linux
sudo pacman -S eigen gnuplot

# Ubuntu/Debian
sudo apt install libeigen3-dev gnuplot

# macOS
brew install eigen gnuplot
```

## Структура проекта и сборка

```
diffcore-cpp/
├── include/          # Заголовочные файлы
│   ├── diff_scheme.h     # Задача Коши
│   ├── bvp.h             # Краевые задачи
│   ├── condition.h       # Граничные условия
│   ├── interpolate.h     # Интерполяция
│   ├── utils.h           # Вспомогательные функции
│   └── plot.h            # Графики (Gnuplot)
├── src/              # Реализация
│   ├── bvp.cpp
│   ├── interpolate.cpp
│   ├── condition_dirichlet.cpp
│   ├── utils.cpp
│   └── plot.cpp
├── test/             # Тесты
├── main.cpp          # Точка входа
├── CMakeLists.txt
├── Makefile
├── LICENSE
└── README.md
```

```bash
git clone https://github.com/bequ1n/diffcore-cpp.git
cd diffcore-cpp
mkdir -p build && cd build
cmake ..
make
./diffcore
```

## Быстрый старт

### Задача Коши

```cpp
#include "diff_scheme.h"

auto rhs = [](double x, double y) { return y; };         // y' = y
auto euler = [](double x, double y, double h, auto f) {
    return y + h * f(x, y);
};

DiffScheme scheme(0.01, euler);
auto result = scheme.execute(0.0, 1.0, 0.0, 1.0, rhs);
// result — вектор пар {x_i, y_i}
```

### Краевая задача

```cpp
#include "bvp.h"
#include "condition.h"
#include "plot.h"

auto k = [](double x) { return 4.0 - x; };
auto q = [](double x) { return 2.0; };
auto f = [](double x) { return (10.0*(x*x - 5.0*x + 9.0))/exp(x); };

auto left  = std::make_unique<DirichletCondition>(0, 2.0);
auto right = std::make_unique<DirichletCondition>(N, 20.0*exp(-2.0));

DiffScheme scheme(h, euler, DSK2);
BVP task(a, b, h, k, q, f, std::move(left), std::move(right), std::move(scheme));
auto [x, u] = task.solve();

Plot::draw(x, {u}, "D1 solution", {"u(x)"});
```

### Собственные значения:

```cpp 
auto [lambdas, vectors] = task.eigenSolve();
auto [lam1, lam2] = lambdas;
auto [u1, u2] = vectors;```

### Интерполяция:

```Interpolator interp({{0,2}, {1,3}, {2,6}});
double y = interp.lagrange(1.5);```

## Автор

bequ1n

## Лицензия

MIT. См. файл [LICENSE](LICENSE).
