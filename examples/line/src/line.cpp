#include <iostream>
#include <random>
#include <vector>

#include <ransac.hpp>

#include "point.hpp"

struct Line{};

P2DS get_sample(const P2DS& p2ds, int min)
{

}

Line generate(const P2DS& p2ds){}

void empty(const P2DS& p2ds){}

int main()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::normal_distribution<> distrib(0, 1.0);

    P2DS points (100);
    for (auto& p : points)
        p = {50 * distrib(g), 50 * distrib(g)};

    std::cout << "Dataset:" << std::endl;
    for (auto& p : points)
        std::cout << p << std::endl;

    Line line;

    Ransac ransac;

    // ransac<double>(0,0,0,0);

    ransac.fit<Line>(points);

    // data – a set of observed data points
    // model – a model that can be fitted to data points

    // min_n – the minimum number of data values required to fit the model
    // max_n – the maximum number of iterations allowed in the algorithm

    // threshold – a threshold value for determining when a data point fits a model
    // min_fit – the number of close data values required to assert that a model fits well to data

    return 0;
}
