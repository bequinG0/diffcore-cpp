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
        std::unique_ptr<BoundaryConditions> leftCond;
        std::unique_ptr<BoundaryConditions> rightCond;
        Eigen::SparseMatrix<double> A;
        Eigen::VectorXd b;

    public:
        BVP(double l, double r, double h,
            Function _k, Function _q, Function _f,
            std::unique_ptr<BoundaryConditions> _leftCond,
            std::unique_ptr<BoundaryConditions> _rightCond)
            : left(l), right(r), step(h),
            k(std::move(_k)), q(std::move(_q)), f(std::move(_f)),
            leftCond(std::move(_leftCond)),
            rightCond(std::move(_rightCond)),
            A((int)((r - l) / h) + 1, (int)((r - l) / h) + 1),
            b((int)((r - l) / h) + 1)
        {
            int N = (right - left) / step;
            double h2 = step * step;

            for (int i = 1; i < N; ++i)
            {
                double xi = left + i * step;
                double k_m = k(xi - 0.5 * step);
                double k_p = k(xi + 0.5 * step);

                A.coeffRef(i, i-1) = -k_m / h2;
                A.coeffRef(i, i)   = (k_m + k_p) / h2 - q(xi);
                A.coeffRef(i, i+1) = -k_p / h2;
                b(i) = f(xi);
            }

            leftCond->addEquation(step, k, q, f, A, b);
            rightCond->addEquation(step, k, q, f, A, b);
        }

        ~BVP();

        auto execute() -> std::pair<std::vector<double>, std::vector<double>>
        {
            Eigen::SimplicialLDLT<Eigen::SparseMatrix<double>> solver;
            solver.compute(A);
            Eigen::VectorXd u = solver.solve(b);

            std::vector<double> x(A.rows()), y(A.rows());
            for (int i = 0; i < A.rows(); ++i)
            {
                x[i] = left + i * step;
                y[i] = u(i);
            }
            return {x, y};
        }
};

#endif