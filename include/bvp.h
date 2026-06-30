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

template <typename Function, typename Formula, typename RowFormula>
class BVP
{
private:
    double left, right, step;
    Function k, q, f;
    std::unique_ptr<BoundaryConditions> leftCond;
    std::unique_ptr<BoundaryConditions> rightCond;
    Eigen::SparseMatrix<double> A;
    Eigen::VectorXd b;
    DiffScheme<Formula, RowFormula> scheme;

public:
    BVP(double l, double r, double h,
        Function _k, Function _q, Function _f,
        std::unique_ptr<BoundaryConditions> _leftCond,
        std::unique_ptr<BoundaryConditions> _rightCond,
        DiffScheme<Formula, RowFormula> _scheme)
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
        int n = A.rows();
        std::vector<double> a(n), b_diag(n), c(n), d(n);
        for (int i = 0; i < n; ++i) {
            b_diag[i] = A.coeff(i, i);
            d[i] = b(i);
            if (i > 0)   a[i] = A.coeff(i, i-1);
            if (i < n-1) c[i] = A.coeff(i, i+1);
        }

        std::vector<double> P(n), Q(n);
        P[0] = -c[0] / b_diag[0];
        Q[0] = d[0] / b_diag[0];

        for (int i = 1; i < n; ++i) {
            double denom = b_diag[i] + a[i] * P[i-1];
            P[i] = (i < n-1) ? -c[i] / denom : 0.0;
            Q[i] = (d[i] - a[i] * Q[i-1]) / denom;
        }

        std::vector<double> u(n);
        u[n-1] = Q[n-1];
        for (int i = n-2; i >= 0; --i)
            u[i] = P[i] * u[i+1] + Q[i];

        std::vector<double> x(n);
        for (int i = 0; i < n; ++i)
            x[i] = left + i * step;

        return {x, u};
    }

    auto eigenSolve() -> std::pair<std::pair<double, double>, std::pair<std::vector<double>, std::vector<double>>>
    {
        int N = A.rows() - 1;
        Eigen::SparseMatrix<double> A_inner = A.block(1, 1, N-1, N-1);
        Eigen::MatrixXd A_dense(A_inner);
        
        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(A_dense);
        if (solver.info() != Eigen::Success)
            throw std::runtime_error("EigenSolver failed");

        double lambda1 = solver.eigenvalues()(0);
        double lambda2 = solver.eigenvalues()(1);

        std::vector<double> u1(A.rows(), 0.0), u2(A.rows(), 0.0);
        for (int i = 0; i < N-1; ++i) {
            u1[i+1] = solver.eigenvectors()(i, 0);
            u2[i+1] = solver.eigenvectors()(i, 1);
        }

        return {{lambda1, lambda2}, {u1, u2}};
    }

    auto conditionNumber() -> double
    {
        int N = A.rows() - 1;
        Eigen::SparseMatrix<double> A_inner = A.block(1, 1, N-1, N-1);
        Eigen::MatrixXd A_dense(A_inner);
        
        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(A_dense);
        if (solver.info() != Eigen::Success)
            throw std::runtime_error("EigenSolver failed");

        auto lambdas = solver.eigenvalues();
        double lambda_min = lambdas(0);
        double lambda_max = lambdas(lambdas.size() - 1);
        return lambda_max / lambda_min;
    }
};

#endif