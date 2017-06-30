#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

#include <ransac.hpp>

#include "point.hpp"

struct SomePoints
{
    const P2DS points;

    int size(){ return points.size(); };

    void add(const SomePoints& input)
    {
        // std::merge(points.begin(), points.end()
        //          , input.points.begin(), input.points.end()
        //          , [](auto& a, auto& b){ return a.x() < b.x(); }
        //          );

        // for(auto& p : input.points)
        // {
        //     points.push_back(p);
        // }
    };

    //sample n data from the dataset
    SomePoints sample(const int n) const { return SomePoints{}; };

    SomePoints filter(const SomePoints& to_filter) const { return SomePoints{}; };
};


// template<typename Model, typename DataSet>
// void Ransac::get_model(Model& maybe_model, const DataSet& filtered_data, DataSet& also_inliers)
// {
//     // for (auto d : data)
//     // {
//     //     if (!is_inside(maybe_inliers, d))
//     //     {
//     //         if (is_fitting(maybe_model, d, threshold) )
//     //         {
//     //             also_inliers.add(d);

//     //             if (also_inliers.size() > min_fit)
//     //             {
//     //                 break;
//     //             }
//     //         }
//     //     }
//     // }
// }


struct Line
{
    //generate a model according to dataset
    double generate(const SomePoints& p) { return double(); };

    SomePoints get_inliers(const SomePoints& p) const { return SomePoints{}; }
};

int main()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::normal_distribution<> distrib(0, 1.0);

    P2DS points (100);
    for (auto& p : points)
        p = {50 * distrib(g), 50 * distrib(g)};

    // std::cout << "Dataset:" << std::endl;
    // for (auto& p : points)
    //     std::cout << p << std::endl;

    Line line;
    SomePoints data{points};

    Ransac ransac(int(1e3), 0, 0, 0);

    ransac.fit<Line>(data);

    // data – a set of observed data points
    // model – a model that can be fitted to data points

    // min_n – the minimum number of data values required to fit the model
    // max_n – the maximum number of iterations allowed in the algorithm

    // threshold – a threshold value for determining when a data point fits a model
    // min_fit – the number of close data values required to assert that a model fits well to data

    std::cout << "ransac ended." << std::endl;
    return 0;
}
