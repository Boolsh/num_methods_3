#include <iostream>
#include <Windows.h>
#include "Vector.h"
#include "SLAE_solver.h"
#include "Experiment.h"

int read_and_check(const char* message, int a, int b);
int main_menu();
void solve_system();

int main()
{
    SetConsoleOutputCP(1251);
    int choice;
    do
    {
        choice = main_menu();
        switch (choice)
        {
        case 1:
            solve_system();
            break;

        case 2: // Одиночный вычислительный эксперимент из файлов
        {
            double abs_err, rel_err;

            Experiment::run_single_experiment_from_files(
                "matrix.txt",   
                "vector.txt",   
                abs_err,
                rel_err,
                true    
            );

            std::cout << "\nАбсолютная погрешность: " << abs_err
                << "\nОтносительная погрешность: " << rel_err << '\n';
            break;
        }

        case 3: // Эксперимент по размеру системы
            Experiment::run_size_experiment();
            break;

        case 4: // Эксперимент по обусловленности системы
            Experiment::run_condition_experiments();
            break;
        }
    } while (choice != 5);

    return 0;
}

int read_and_check(const char* message, int a, int b)
{
    int x;
    std::cout << message;
    while (!(std::cin >> x && (x >= a && x <= b)))
    {
        std::cout << "Ошибка ввода!\n";
        std::cin.clear();
        std::cin.ignore(std::cin.rdbuf()->in_avail());
        std::cout << message;
    }
    return x;
}

int main_menu()
{
    std::cout << "\n=== Главное меню ===\n";
    std::cout << "1. Решить систему методом Холецкого\n";
    std::cout << "2. Одиночный вычислительный эксперимент (из файлов)\n";
    std::cout << "3. Эксперимент по размеру системы\n";
    std::cout << "4. Эксперимент по обусловленности системы\n";
    std::cout << "5. Выход\n";

    return read_and_check("Выберите действие: ", 1, 5);
}

void solve_system()
{
    std::cout << "Введите размер матрицы:\n";
    size_t n;
    std::cin >> n;

    Dense_matrix A(n);
    A.read_from_console();

    std::cout << "Введите вектор правой части:\n";
    Vector d(std::cin);

    Vector x = SLAE_solver::cholesky_method(A, d, true);

    std::cout << "Решение:\n";
    x.print(std::cout);
}
