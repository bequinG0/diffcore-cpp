#pragma once
#ifndef BVP_H
#define BVP_H

#include <iostream>
#include <cstdio>
#include <cmath>
#include <utility>
#include <functional>
#include <string>
#include <algorithm>

#include <Eigen/Sparse>
#include <Eigen/Dense>

#include "conditions.h"

using namespace std;
using namespace Eigen;

template <typename Function>
class BVP
{
    private:
        double left, right, step;
        Function k, q, f;
        BoundaryConditons conditons;

        SparseMatrix <double> A;
        VectorXd b;
    public:
        //BVP(double l, double r, double h, Function _k, Function _q, Function _rho, BoundaryConditions _cond) : left(l), right(r), step(h), k(_k), q(_q), rho(_rho), conditions(_cond) {}
        BVP(double l, double r, double h, Function _k, Function _q, Function _f, BoundaryConditions _cond)
        {
            left = l; right = r; step = h; k = _k; q = _q; f = _f; conditions = _cond;

            int N = (right - left) / step;
            A.resize(N + 1, N + 1);
            b.resize(N + 1);
            double h2 = step * step;  // h^2
            for (int i = 1; i < N; ++i)
            {
                double xi = left + i * step;
                double k_m = k(xi - 0.5 * step);  // k_{i-1/2}
                double k_p = k(xi + 0.5 * step);  // k_{i+1/2}

                A.coeffRef(i, i-1) = -k_m / h2;
                A.coeffRef(i, i)   = (k_m + k_p) / h2 - q(xi);
                A.coeffRef(i, i+1) = -k_p / h2;
                b(i) = f(xi);
            }

            A.coeffRef(0, 0) = 1.0;
            b(0) = 0.0;  // заглушка

            A.coeffRef(N, N) = 1.0;
            b(N) = 0.0;  // заглушка
        }

        ~BVP();

        vector <pair <double, double>> execute()
        {
            return vector <pair <double, double>> ();
        }
};

#endif