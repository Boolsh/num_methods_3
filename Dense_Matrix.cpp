#include "Dense_matrix.h"
#include "Vector.h"

Dense_matrix::Dense_matrix(size_t n) : n(n)
{
    data.resize(n + 1, std::vector<double>(n + 1, 0.0));
}

size_t Dense_matrix::size() const
{
    return n;
}

double& Dense_matrix::operator()(size_t i, size_t j)
{
    if (i < 1 || i > n || j < 1 || j > n)
        throw std::out_of_range("Неверный индекс матрицы");
    return data[i][j];
}

const double& Dense_matrix::operator()(size_t i, size_t j) const
{
    if (i < 1 || i > n || j < 1 || j > n)
        throw std::out_of_range("Неверный индекс матрицы");
    return data[i][j];
}

Dense_matrix Dense_matrix::operator+(const Dense_matrix& other) const
{
    if (n != other.n)
        throw std::runtime_error("Размеры матриц не совпадают");

    Dense_matrix res(n);
    for (size_t i = 1; i <= n; ++i)
        for (size_t j = 1; j <= n; ++j)
            res(i, j) = data[i][j] + other(i, j);

    return res;
}

Dense_matrix Dense_matrix::operator-(const Dense_matrix& other) const
{
    if (n != other.n)
        throw std::runtime_error("Размеры матриц не совпадают");

    Dense_matrix res(n);
    for (size_t i = 1; i <= n; ++i)
        for (size_t j = 1; j <= n; ++j)
            res(i, j) = data[i][j] - other(i, j);

    return res;
}

Vector Dense_matrix::operator*(const Vector& x) const
{
    if (x.size() != n)
        throw std::runtime_error("Размер вектора не совпадает");

    Vector res(n);
    for (size_t i = 1; i <= n; ++i)
    {
        double sum = 0.0;
        for (size_t j = 1; j <= n; ++j)
            sum += data[i][j] * x[j];
        res[i] = sum;
    }
    return res;
}

void Dense_matrix::fill_random(double min, double max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(min, max);

    for (size_t i = 1; i <= n; ++i)
        for (size_t j = 1; j <= n; ++j)
            data[i][j] = dist(gen);
}

void Dense_matrix::read_from_file(std::ifstream& in)
{
    for (size_t i = 1; i <= n; ++i)
        for (size_t j = 1; j <= n; ++j)
            in >> data[i][j];
}

void Dense_matrix::read_from_console()
{
    std::cout << "Введите матрицу " << n << "x" << n << ":\n";
    for (size_t i = 1; i <= n; ++i)
        for (size_t j = 1; j <= n; ++j)
            std::cin >> data[i][j];
}

void Dense_matrix::print(std::ostream& out) const
{
    for (size_t i = 1; i <= n; ++i)
    {
        for (size_t j = 1; j <= n; ++j)
            out << std::setw(10) << data[i][j];
        out << '\n';
    }
}
