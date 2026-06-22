#pragma once
#ifndef CONDITIONS_H
#define CONDITIONS_H

#include <iostream>
#include <cstdio>
#include <cmath>
#include <utility>
#include <functional>
#include <string>
#include <algorithm>

#include <Eigen/Sparse>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

class BoundaryConditions
{
    public:
        //virtual BoundaryConditions() = default;
        virtual ~BoundaryConditions() = default;

        virtual void addEquation(double h,const function<double(double)> &k,
        const function<double (double)> &q, const function <double (double)> &f,
        SparseMatrix <double> &A, VectorXd &b);

    private:
};

class DirichletCondition : public BoundaryConditions
{
    private:
        int num;
        double value;
    public:
        DirichletCondition(int _num, double _value) : value(_value), num(_num) {}

        void addEquation(double h,const function<double(double)> &k,
        const function<double (double)> &q, const function <double (double)> &f,
        SparseMatrix <double> &A, VectorXd &b);

};

class NeumannCondition : public BoundaryConditions
{
    private:
        int num;
        double value;
    public:
        NeumannCondition(int _num,  double _value) : value(_value), num(_num) {}

        void addEquation(double h,const function<double(double)> &k,
        const function<double (double)> &q, const function <double (double)> &f,
        SparseMatrix <double> &A, VectorXd &b);
};

class RobinCondition : public BoundaryConditions
{
    private:
        int num;
        double value, sigma;
    public:
        RobinCondition(int _num, double _value, double _sigma) : value(_value), sigma(_sigma), num(_num) {}

        void addEquation(double h,const function<double(double)> &k,
            const function<double (double)> &q, const function <double (double)> &f,
            SparseMatrix <double> &A, VectorXd &b);

};

#endif