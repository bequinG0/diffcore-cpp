#pragma once
#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <iostream>
#include <cstdio>
#include <cmath>
#include <utility>
#include <functional>
#include <string>
#include <algorithm>

using namespace std;

class Interpolator
{
    public:
        vector <pair <double, double>> data;

        Interpolator(vector <pair <double, double>> a);
    private:

        double LagrangeInterpolate(double x);
        double NewtonInterpolate(double x);
        double BSplainInterpolate(double x);
};

#endif