#include "Experiment.h"

void Experiment::run(const Dense_matrix& A, const Vector& x_exact,
                     double& abs_err, double& rel_err,
                     bool verbose)
{
    // Правая часть
    Vector d = A * x_exact;

    if (verbose)
    {
        std::cout << "Правая часть d:\n";
        d.print(std::cout);
    }

    // Решение методом Холецкого
    Vector x = SLAE_solver::cholesky_method(A, d, verbose);

    if (verbose)
    {
        std::cout << "Найденное решение:\n";
        x.print(std::cout);
    }

    auto err = compute_errors(x, x_exact);
    abs_err = err.first;
    rel_err = err.second;

    if (verbose)
    {
        std::cout << "\nАбсолютная погрешность: " << abs_err << '\n';
        std::cout << "Относительная погрешность: " << rel_err << '\n';
    }
}

void Experiment::run_single_experiment(size_t n,
    double& abs_err,
    double& rel_err)
{
    Dense_matrix B(n);
    B.fill_random(-1.0, 1.0);

    Dense_matrix A(n);

    // A = B^T * B + n * I
    for (size_t i = 1; i <= n; ++i)
        for (size_t j = 1; j <= n; ++j)
        {
            double sum = 0.0;
            for (size_t k = 1; k <= n; ++k)
                sum += B(k, i) * B(k, j);
            A(i, j) = sum;
        }

    for (size_t i = 1; i <= n; ++i)
        A(i, i) += n;

    Vector x_exact(n);
    x_exact.fill_random(-10.0, 10.0);

    run(A, x_exact, abs_err, rel_err, false);
}

void Experiment::run_single_experiment_from_files(
    const std::string& matrix_file,
    const std::string& x_file,
    double& abs_err,
    double& rel_err,
    bool verbose)
{
    std::ifstream mat_in(matrix_file);


    size_t n;
    mat_in >> n;

    Dense_matrix A(n);
    A.read_from_file(mat_in);

    std::ifstream vec_in(x_file);

    Vector x_exact(vec_in);

    if (verbose)
    {
        std::cout << "Матрица A:\n";
        A.print(std::cout);

        std::cout << "Точное решение x_exact:\n";
        x_exact.print(std::cout);
    }

    // Используем уже существующую логику
    run(A, x_exact, abs_err, rel_err, verbose);
}

void Experiment::run_size_experiment()
{
    std::cout << "\nЭксперимент: зависимость погрешности от размера\n\n";

    std::vector<size_t> sizes = { 64, 128, 256, 512, 1024 };

    std::cout << std::setw(10) << "Размер"
        << std::setw(25) << "Абс. погрешность"
        << std::setw(25) << "Отн. погрешность\n";

    std::cout << std::string(60, '-') << '\n';

    for (size_t n : sizes)
    {
        double abs_err = 0.0, rel_err = 0.0;
        const int experiments = 5;

        for (int i = 0; i < experiments; ++i)
        {
            double a, r;
            run_single_experiment(n, a, r);
            abs_err = std::max(abs_err, a);
            rel_err = std::max(rel_err, r);
        }

        std::cout << std::setw(10) << n
            << std::setw(25) << abs_err
            << std::setw(25) << rel_err << '\n';
    }
}

void Experiment::run_condition_experiments()
{
    std::cout << "\n=== Эксперимент: влияние обусловленности на точность ===\n\n";
    std::cout << std::setw(10) << "n"
        << std::setw(20) << "abs (good cond)"
        << std::setw(20) << "rel (good cond)"
        << std::setw(20) << "abs (bad cond)"
        << std::setw(20) << "rel (bad cond)"
        << '\n';

    std::cout << std::string(90, '-') << '\n';
    //std::vector<size_t> sizes = { 10, 100, 300, 500};
    std::vector<size_t> sizes = { 10};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-1.0, 1.0);

    for (size_t n : sizes)
    {
        Vector x_exact(n);
        x_exact.fill_random(-10.0, 10.0);

        // ХОРОШО ОБУСЛОВЛЕННАЯ 
        Dense_matrix B(n);
        for (size_t i = 1; i <= n; ++i)
            for (size_t j = 1; j <= n; ++j)
                B(i, j) = dist(gen);

        Dense_matrix A_good(n);

        for (size_t i = 1; i <= n; ++i)
            for (size_t j = 1; j <= n; ++j)
            {
                double sum = 0.0;
                for (size_t k = 1; k <= n; ++k)
                    sum += B(k, i) * B(k, j);
                A_good(i, j) = sum;
            }

        for (size_t i = 1; i <= n; ++i)
            A_good(i, i) *= n;  
        std::cout << "\n";

        A_good.print(std::cout);
        std::cout << "\n";

        double abs_good, rel_good;
        run(A_good, x_exact, abs_good, rel_good, false);

        // ПЛОХО ОБУСЛОВЛЕННАЯ 
        Dense_matrix A_bad(n);

        for (size_t i = 1; i <= n; ++i)
            for (size_t j = 1; j <= n; ++j)
            {
                double sum = 0.0;
                for (size_t k = 1; k <= n; ++k)
                    sum += B(k, i) * B(k, j);
                A_bad(i, j) = sum;
            }

        for (size_t i = 1; i <= n; ++i)
            A_bad(i, i) += 1e-6; 
        std::cout << "\n";
        A_bad.print(std::cout);
        std::cout << "\n";

        double abs_bad, rel_bad;
        run(A_bad, x_exact, abs_bad, rel_bad, false);


        std::cout << std::setw(10) << n
            << std::setw(20) << abs_good
            << std::setw(20) << rel_good
            << std::setw(20) << abs_bad
            << std::setw(20) << rel_bad << '\n';
    }
}



std::pair<double, double> Experiment::compute_errors(const Vector& x_approx, const Vector& x_exact)
{
    Vector difference = x_approx - x_exact;
    double abs_err = difference.norm(); 
    double rel_err = 0.0;
    double eps = std::sqrt(std::numeric_limits<double>::epsilon());

    for (size_t i = 1; i <= x_exact.size(); i++)
    {
        double diff = abs(difference[i]);  
        double denom = abs(x_exact[i]);  

        if (denom < eps)
            rel_err = std::max(rel_err, diff);
        else
            rel_err = std::max(rel_err, diff / denom);
    }

    return { abs_err, rel_err };
}
