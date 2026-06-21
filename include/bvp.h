#pragma once
#ifndef BVP_H
#define BVP_H

#include <iostream>
#include <cstdio>
#include <cmath>
#include <utility>
#include <functional>
#include <string>
#include <algorithm>

using namespace std;

template <typename Function>
class BVP
{
    private:
        double left, right, step;
        Function k, q, rho;
    public:
        BVP(double l, double r, double h, Function _k, Function _q, Function _rho) : left(l), right(r), step(h), k(_k), q(_q), rho(_rho) {}

        ~BVP();

        vector <pair <double, double>> execute()
        {
            return vector <pair <double, double>> ();
        }
};

#endif