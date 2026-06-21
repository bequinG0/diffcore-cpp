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

        virtual int addEquation(double h,const function<double(double)> &k,
        const function<double (double)> &q, const function <double (double)> &f,
        SparseMatrix <double> A, VectorXd b);

    private:
};

class DirichletCondition : public BoundaryConditions
{
    private:
        double x, value;
    public:
        DirichletCondition(double _x, double _value) : value(_value), x(_x) {}

        int addEquation(double h,const function<double(double)> &k,
        const function<double (double)> &q, const function <double (double)> &f,
        SparseMatrix <double> A, VectorXd b);

};

class NeumannCondition : public BoundaryConditions
{
    private:
        double x, value;
    public:
        
};

class RobinCondition : public BoundaryConditions
{
    private:
        double x, value, sigma;
    public:

};

#endif