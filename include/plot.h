#pragma once

#ifndef CONDITIONS_H
#define CONDITIONS_H

#include <iostream>
#include <vector> 
#include <algorithm>
#include <string>
#include <cstdio>

class Plot
{
    public:
        static void draw(const std::vector<double>& x,
                     const std::vector<std::vector<double>>& ys,
                     const std::string& title = "",
                     const std::vector<std::string>& labels = {},
                     const std::string& style = "lines")
    {
        FILE* gp = popen("gnuplot -persist", "w");
        if (!gp) return;

        fprintf(gp, "set title '%s'\n", title.c_str());
        fprintf(gp, "set xlabel 'x'\n");
        fprintf(gp, "set ylabel 'y'\n");
        fprintf(gp, "set grid\n");

        fprintf(gp, "plot ");
        for (size_t k = 0; k < ys.size(); ++k)
        {
            if (k > 0) fprintf(gp, ", ");
            fprintf(gp, "'-' w %s lw 2", style.c_str());
            if (k < labels.size())
                fprintf(gp, " title '%s'", labels[k].c_str());
            else
                fprintf(gp, " title ''");
        }
        fprintf(gp, "\n");

        for (size_t k = 0; k < ys.size(); ++k)
        {
            for (size_t i = 0; i < x.size(); ++i)
                fprintf(gp, "%f %f\n", x[i], ys[k][i]);
            fprintf(gp, "e\n");
        }

        fflush(gp);
        pclose(gp);
    }

    static void save(const std::vector<double>& x,
                     const std::vector<std::vector<double>>& ys,
                     const std::string& filename,
                     const std::string& title = "",
                     const std::vector<std::string>& labels = {},
                     const std::string& style = "lines")
    {
        FILE* gp = popen("gnuplot", "w");
        if (!gp) return;

        size_t dot = filename.find_last_of('.');
        std::string ext = (dot != std::string::npos) ? filename.substr(dot + 1) : "png";

        if (ext == "svg")
            fprintf(gp, "set terminal svg size 800,600\n");
        else if (ext == "pdf")
            fprintf(gp, "set terminal pdf size 8cm,6cm\n");
        else
            fprintf(gp, "set terminal png size 800,600\n");

        fprintf(gp, "set output '%s'\n", filename.c_str());
        fprintf(gp, "set title '%s'\n", title.c_str());
        fprintf(gp, "set xlabel 'x'\n");
        fprintf(gp, "set ylabel 'y'\n");
        fprintf(gp, "set grid\n");

        fprintf(gp, "plot ");
        for (size_t k = 0; k < ys.size(); ++k)
        {
            if (k > 0) fprintf(gp, ", ");
            fprintf(gp, "'-' w %s lw 2", style.c_str());
            if (k < labels.size())
                fprintf(gp, " title '%s'", labels[k].c_str());
            else
                fprintf(gp, " title ''");
        }
        fprintf(gp, "\n");

        for (size_t k = 0; k < ys.size(); ++k)
        {
            for (size_t i = 0; i < x.size(); ++i)
                fprintf(gp, "%f %f\n", x[i], ys[k][i]);
            fprintf(gp, "e\n");
        }

        fflush(gp);
        pclose(gp);
    }
};


#endif