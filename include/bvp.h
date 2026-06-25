#pragma once
#ifndef BVP_H
#define BVP_H

#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include <Eigen/Sparse>
#include <Eigen/Dense>

#include "conditions.h"
#include "diffscheme.h"

template <typename Function, typename RowFormula>
class BVP
{
private:
    double left, right, step;
    Function k, q, f;
    std::unique_ptr<BoundaryConditions> leftCond;
    std::unique_ptr<BoundaryConditions> rightCond;
    Eigen::SparseMatrix<double> A;
    Eigen::VectorXd b;
    DiffScheme<Function, RowFormula> scheme;

public:
    BVP(double l, double r, double h,
        Function _k, Function _q, Function _f,
        std::unique_ptr<BoundaryConditions> _leftCond,
        std::unique_ptr<BoundaryConditions> _rightCond,
        DiffScheme<Function, RowFormula> _scheme)
        : left(l), right(r), step(h),
          k(std::move(_k)), q(std::move(_q)), f(std::move(_f)),
          leftCond(std::move(_leftCond)),
          rightCond(std::move(_rightCond)),
          scheme(std::move(_scheme)),
          A(static_cast<int>((r - l) / h) + 1,
            static_cast<int>((r - l) / h) + 1),
          b(static_cast<int>((r - l) / h) + 1)
    {
        int N = static_cast<int>((right - left) / step);
        double h2 = step * step;

        for (int i = 1; i < N; ++i) scheme.fillMatrix(A, b, N, left);

        leftCond->addEquation(step, k, q, f, A, b);
        rightCond->addEquation(step, k, q, f, A, b);
    }

    ~BVP() = default;

    auto solve() -> std::pair<std::vector<double>, std::vector<double>>
    {
        Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
        solver.compute(A);
        Eigen::VectorXd u = solver.solve(b);

        std::vector<double> x(static_cast<size_t>(A.rows()));
        std::vector<double> y(static_cast<size_t>(A.rows()));
        for (int i = 0; i < A.rows(); ++i)
        {
            x[static_cast<size_t>(i)] = left + i * step;
            y[static_cast<size_t>(i)] = u(i);
        }
        return {x, y};
    }

    std::vector <double> eigenvalues()
    {
        Eigen::MatrixXd A_dense(A);
        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigensolver(A_dense);

        if (eigensolver.info() != Eigen::Success) throw std::runtime_error("EigenSolver failed");

        std::vector<double> ev(eigensolver.eigenvalues().size());
        for (int i = 0; i < eigensolver.eigenvalues().size(); ++i) ev[i] = eigensolver.eigenvalues()(i);

        return ev;
    }
};

#endif