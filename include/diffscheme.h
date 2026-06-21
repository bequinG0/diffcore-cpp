#pragma once
#ifndef DIFFSCHEME_H
#define DIFFSCHEME_H

#include <iostream>
#include <cstdio>
#include <cmath>
#include <utility>
#include <functional>
#include <string>
#include <algorithm>

using namespace std;

template<typename Formula>
class DiffScheme
{
    private:
        double h;
        Formula F;
    public:
        DiffScheme(double _h, Formula _f) : h(_h), F(move(_f)) {}
        
        template <typename Right>
        vector <pair<double, double>> execute(double a, double b, double x0, double y0, Right f)
        {
            int step = 0;
            vector <pair<double, double>> res;
            res.push_back(pair <double, double> ());
            res[step].first = x0; res[step].second = y0;

            while(res[step].first - a> h/2+1e-10) //левая часть отрезка
            {
                res.push_back(pair <double, double> ());
                res[step+1].first = res[step].first - h; res[step+1].second = F(res[step].first, res[step].second, -h, f); 
                step++;
            }

            reverse(res.begin(), res.end());

            while(b - res[step].first > h/2+1e-10) //правая часть отрезка
            {
                res.push_back(pair <double, double> ());
                res[step+1].first = res[step].first + h; res[step+1].second = F(res[step].first, res[step].second, h, f); 
                step++;
            }
            
            return res;
        }
};

#endif //DIFFSCHEME_H

