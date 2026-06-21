#include "../include/conditions.h"

int DirichletCondition::addEquation(double h,const function<double(double)> &k,
        const function<double (double)> &q, const function <double (double)> &f,
        SparseMatrix <double> A, VectorXd b)
{
    return 0;
}