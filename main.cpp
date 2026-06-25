#include <iostream>
#include <cmath>
#include <memory>
#include <functional>

#include "include/conditions.h"
#include "include/bvp.h"
#include "include/plot.h"

using namespace std;
using namespace Eigen;

int main()
{
    double a = 0, b = 2;
    double h = 1e-4;
    int N = (b - a) / h;

    auto k = [](double x) -> double { return 4 - x; };
    auto q = [](double x) -> double { return 2; };
    auto f = [](double x) -> double { return (10*(x*x - 5*x + 9))/exp(x); };

    auto left  = std::make_unique<DirichletCondition>(0, 0.0);         
    auto right = std::make_unique<DirichletCondition>(N, 20*exp(-2));          

    BVP<std::function<double(double)>> task(a, b, h, k, q, f,
        std::move(left), std::move(right));

    auto ans = task.solve();

    /*for (size_t i = 0; i < ans.first.size(); ++i)
        std::cout << ans.first[i] << "\t" << ans.second[i] << "\n"; */

    Plot::draw(ans.first, {ans.second}, "test k=x", {"u(x)"});

    return 0;
}