#pragma once

#ifndef PLOT_H
#define PLOT_H
#pragma once

#include <vector>
#include <string>
#include <cstdio>

class Plot
{
public:
    // N графиков в окне
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
};

#endif