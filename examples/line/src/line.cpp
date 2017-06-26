#include <iostream>
#include <random>
#include <vector>

#include <ransac.hpp>

#include "point.hpp"

struct Line{};

int main()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::normal_distribution<> distrib(0, 10.0);

    std::vector<P2D> points;
    for (double p = 0; p < 100.0; ++p)
    {
        points.emplace_back( p * distrib(g), p * distrib(g));
    }

    for (auto& p : points)
        std::cout << p << std::endl;

    return 0;
}
