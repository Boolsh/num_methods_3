#pragma once
#include "Dense_matrix.h"
#include "Vector.h"

class SLAE_solver
{
public:
    static Vector cholesky_method(const Dense_matrix& A, const Vector& d, bool verbose);
};