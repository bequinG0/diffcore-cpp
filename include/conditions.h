#pragma once

#include <functional>

#include <Eigen/Sparse>
#include <Eigen/Dense>

class BoundaryConditions
{
    public:
        virtual ~BoundaryConditions() = default;

        virtual void addEquation(double h,
                                const std::function<double(double)>& k,
                                const std::function<double(double)>& q,
                                const std::function<double(double)>& f,
                                Eigen::SparseMatrix<double>& A,
                                Eigen::VectorXd& b) = 0;
};

class DirichletCondition : public BoundaryConditions
{
    private:
        int num;
        double value;
    public:
        DirichletCondition(int _num, double _value)
            : num(_num), value(_value) {}

        void addEquation(double h,
                        const std::function<double(double)>& k,
                        const std::function<double(double)>& q,
                        const std::function<double(double)>& f,
                        Eigen::SparseMatrix<double>& A,
                        Eigen::VectorXd& b) override;
};

class NeumannCondition : public BoundaryConditions
{
    private:
        int num;
        double value;
    public:
        NeumannCondition(int _num, double _value)
            : num(_num), value(_value) {}

        void addEquation(double h,
                        const std::function<double(double)>& k,
                        const std::function<double(double)>& q,
                        const std::function<double(double)>& f,
                        Eigen::SparseMatrix<double>& A,
                        Eigen::VectorXd& b) override;
};

class RobinCondition : public BoundaryConditions
{
    private:
        int num;
        double value, sigma;
    public:
        RobinCondition(int _num, double _value, double _sigma)
            : num(_num), value(_value), sigma(_sigma) {}

        void addEquation(double h,
                        const std::function<double(double)>& k,
                        const std::function<double(double)>& q,
                        const std::function<double(double)>& f,
                        Eigen::SparseMatrix<double>& A,
                        Eigen::VectorXd& b) override;
};