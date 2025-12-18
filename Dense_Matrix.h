#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <random>

class Vector;

class Dense_matrix
{
private:
    size_t n;
    std::vector<std::vector<double>> data;

public:
    Dense_matrix(size_t n = 0);

    size_t size() const;

    double& operator()(size_t i, size_t j);
    const double& operator()(size_t i, size_t j) const;

    Dense_matrix operator+(const Dense_matrix& other) const;
    Dense_matrix operator-(const Dense_matrix& other) const;

    Vector operator*(const Vector& x) const;

    void fill_random(double min, double max);
    void read_from_file(std::ifstream& in);
    void read_from_console();
    void print(std::ostream& out) const;
};
