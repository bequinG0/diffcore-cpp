#pragma once

#include <iostream>
#include <cstdio>
#include <cmath>
#include <utility>
#include <functional>
#include <string>
#include <algorithm>

#include <Eigen/Sparse>
#include <Eigen/Dense>

template<typename Formula, typename RowFormula>
class DiffScheme
{
    private:
        double h;
        Formula F;
        RowFormula G;
    public:
        DiffScheme(double _h, Formula _f, RowFormula _g) : h(_h), F(std::move(_f)), G(std::move(_g)) {}
        
        template <typename Right>
        std::vector<std::pair<double, double>> execute(double a, double b, double x0, double y0, Right f)
        {
            int step = 0;
            std::vector<std::pair<double, double>> res;
            res.push_back(std::pair<double, double>());
            res[step].first = x0; res[step].second = y0;

            while(res[step].first - a> h/2+1e-10)
            {
                res.push_back(std::pair<double, double>());
                res[step+1].first = res[step].first - h; res[step+1].second = F(res[step].first, res[step].second, -h, f); 
                step++;
            }

            std::reverse(res.begin(), res.end());

            while(b - res[step].first > h/2+1e-10)
            {
                res.push_back(std::pair<double, double>());
                res[step+1].first = res[step].first + h; res[step+1].second = F(res[step].first, res[step].second, h, f); 
                step++;
            }
            
            return res;
        }

        void fillMatrix(Eigen::SparseMatrix<double>& A, Eigen::VectorXd& b, int N, double left)
        {
            for (int i = 1; i < N; ++i)
            {
                auto coeffs = G(i, h, left);  
                A.coeffRef(i, i-1) = coeffs[0];  
                A.coeffRef(i, i) = coeffs[1];  
                A.coeffRef(i, i+1) = coeffs[2];
                b(i) = coeffs[3];
            }
        }
};