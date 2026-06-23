#include "../include/conditions.h"

void DirichletCondition::addEquation(
    double h,
    const std::function<double(double)>& k,
    const std::function<double(double)>& q,
    const std::function<double(double)>& f,
    Eigen::SparseMatrix<double>& A,
    Eigen::VectorXd& b)
{
    A.coeffRef(num, num) = 1.0;
    b(num) = value;
}

void NeumannCondition::addEquation(
    double h,
    const std::function<double(double)>& k,
    const std::function<double(double)>& q,
    const std::function<double(double)>& f,
    Eigen::SparseMatrix<double>& A,
    Eigen::VectorXd& b)
{
    double x_mid, k_mid;
    int neighbor;

    if (num == 0)  // левая граница
    {
        x_mid = h / 2.0;
        k_mid = k(x_mid);
        neighbor = 1;

        A.coeffRef(0, 0) = k_mid / h;
        A.coeffRef(0, 1) = -k_mid / h;
        b(0) = value + h / 2.0 * f(0);
    }
    else  // правая граница
    {
        x_mid = num * h - h / 2.0;
        k_mid = k(x_mid);
        neighbor = num - 1;

        A.coeffRef(num, neighbor) = -k_mid / h;
        A.coeffRef(num, num) = k_mid / h;
        b(num) = value + h / 2.0 * f(num * h);
    }
}

void RobinCondition::addEquation(
    double h,
    const std::function<double(double)>& k,
    const std::function<double(double)>& q,
    const std::function<double(double)>& f,
    Eigen::SparseMatrix<double>& A,
    Eigen::VectorXd& b)
{
    double x_mid, k_mid;
    int neighbor;

    if (num == 0)  // левая граница
    {
        x_mid = h / 2.0;
        k_mid = k(x_mid);
        neighbor = 1;

        A.coeffRef(0, 0) = k_mid / h + sigma;
        A.coeffRef(0, 1) = -k_mid / h;
        b(0) = sigma * value + h / 2.0 * f(0);
    }
    else  // правая граница
    {
        x_mid = num * h - h / 2.0;
        k_mid = k(x_mid);
        neighbor = num - 1;

        A.coeffRef(num, neighbor) = -k_mid / h;
        A.coeffRef(num, num) = k_mid / h + sigma;
        b(num) = sigma * value + h / 2.0 * f(num * h);
    }
}