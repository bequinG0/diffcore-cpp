#include "../include/interpolate.h"

Interpolator::Interpolator(vector <pair <double, double>> a)
{
    for(auto e : a) data.push_back(e);
}

double Interpolator::LagrangeInterpolate(double x) 
{
    double L = 0;
    vector <double> l;
    for(int i=0; i<data.size(); i++)
    {
        l.push_back(1);
        for(int j=0; j<data.size(); j++)
        {
            if(j != i) l[i] *= (x-data[j].first)/(data[i].first - data[j].first);
        }
        L = L + data[i].second*l[i];
    }

    return L;
}

double Interpolator::NewtonInterpolate(double x)
{
    double N = 0;
    int n = static_cast<int>(data.size());
    vector<double> f(n);
    for (int i = 0; i < n; ++i)
        f[i] = data[i].second;

    for (int k = 1; k < n; ++k)          
    {
        for (int i = n - 1; i >= k; --i) 
        {
            f[i] = (f[i] - f[i-1]) / (data[i].first - data[i - k].first);
        }
    }

    double temp = 1;
    for(int i=0; i<data.size(); i++) 
    {   
        N = N + f[i] * temp;
        temp *= x - data[i].first;
    }

    return N;
}

double Interpolator::BSplainInterpolate(double x)
{
    return 0;
}