#include "SLAE_solver.h"
#include <cmath>
#include <limits>
#include <iostream>

Vector SLAE_solver::cholesky_method(const Dense_matrix& A, const Vector& d, bool verbose)
{
    size_t n = A.size();
    if (d.size() != n)
        throw std::runtime_error("Размер вектора не совпадает");

    Dense_matrix L(n);

    // A = L * L^T
    for (size_t i = 1; i <= n; ++i)
    {
        for (size_t j = 1; j <= i; ++j)
        {
            double sum = 0.0;
            for (size_t k = 1; k < j; ++k)
                sum += L(i, k) * L(j, k);

            if (i == j)
            {
                double val = A(i, i) - sum;
                if (val <= 0.0)
                    throw std::runtime_error("Матрица не является SPD");
                L(i, j) = std::sqrt(val);
            }
            else
            {
                L(i, j) = (A(i, j) - sum) / L(j, j);
            }
        }
    }

    // Прямой ход: L y = d
    Vector y(n);
    for (size_t i = 1; i <= n; ++i)
    {
        double sum = 0.0;
        for (size_t j = 1; j < i; ++j)
            sum += L(i, j) * y[j];
        y[i] = (d[i] - sum) / L(i, i);
    }

    // Обратный ход: L^T x = y
    Vector x(n);
    for (int i = static_cast<int>(n); i >= 1; --i)
    {
        double sum = 0.0;
        for (size_t j = i + 1; j <= n; ++j)
            sum += L(j, i) * x[j];
        x[i] = (y[i] - sum) / L(i, i);
    }

    if (verbose)
    {
        std::cout << "Матрица L:\n";
        L.print(std::cout);
    }

    return x;
}
