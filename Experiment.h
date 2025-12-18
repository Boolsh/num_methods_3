#pragma once
#include "SLAE_solver.h"
#include "Vector.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <functional>

class Experiment
{
public:
    static void run(const Dense_matrix& A, const Vector& x_exact,
        double& abs_err, double& rel_err,
        bool verbose);
    static void run_single_experiment(size_t n,
        double& abs_err,
        double& rel_err);
    static void run_single_experiment_from_files(    const std::string& matrix_file,
    const std::string& x_file,
    double& abs_err,
    double& rel_err,
    bool verbose);
    static void run_condition_experiments();
    static void run_size_experiment();

private:
	static std::pair<double, double> compute_errors(const Vector& x_approx, const Vector& x_exact);
};

