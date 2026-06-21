#include "../include/interpolate.h"

Interpolator::Interpolator(vector <pair <double, double>> a)
{
    for(auto e : a) data.push_back(e);
}

double Interpolator::LagrangeInterpolate(double x) 
{
    return 0;
}

double Interpolator::NewtonInterpolate(double x)
{
    return 0;
}

double Interpolator::BSplainInterpolate(double x)
{
    return 0;
}