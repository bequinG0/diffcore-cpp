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

using namespace std;

class BoundaryConditions
{
    public:
        //virtual BoundaryConditions() = default;
        virtual ~BoundaryConditions() = default;

        virtual void addEquation();

    private:
};

class DirichletConditions : public BoundaryConditions
{
    private:
        double x, value;
    
    public:

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