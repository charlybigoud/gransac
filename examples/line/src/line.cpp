#include <iostream>
#include <random>
#include <algorithm>

#include <ransac.hpp>

#include "point.hpp"

struct OperationsOnContainers
{
    P2DS merge(const P2DS& a, const P2DS& b) const
    {
        P2DS output = a;
        for (auto& p : b)
            output.push_back(p);

        return output;
    }

    P2DS filter(const P2DS& reference, const P2DS& data_to_filter) const
    {
        P2DS output;
        for (auto& r : reference)
        {
            bool do_filter = false;
            for (auto& d : data_to_filter)
            {
                if ((r.x() == d.x())
                and (r.y() == d.y())
                )
                    do_filter = true;
            }

            if (!do_filter)
                output.push_back(r);
        }

        return output;
    }
};

struct Sampling
{
    //sample n data from the dataset
    P2DS operator()(const P2DS& points, const int n) const
    {
        //C++11
        P2DS v1 = points;
        P2DS out;
        std::shuffle(begin(v1)
                   , end(v1)
                   , std::mt19937{std::random_device{}()}
                   );
        std::copy(begin(v1)
                , begin(v1) + n
                , std::back_inserter(out)
                );

        //C++17
        // P2DS out = points;
        // std::sample(begin(points)
        //           , end(points)
        //           , std::back_inserter(out)
        //           , n
        //           , std::mt19937{std::random_device{}()}
        //           );

        return out;
    };
};

// struct Generator
// {
//     //generates (a,b) according to dataset
//     //BRUTE FORCE takes two points in dataset
//     void operator()(const P2DS& p)
//     {
//         auto g = std::mt19937{(std::random_device{}())};
//         std::uniform_int_distribution<> d(0, p.size() - 1);

//         a = p[d(g)];
//         b = p[d(g)];
//     };
// };

struct Line
{
    P2D a, b;

    double slope() const { return (b.y() - a.y()) / (b.x() - a.x()); }
    double get_y(const double x) const { return slope() * x + (a.y() - a.x() * slope()); }

    //generates (a,b) according to dataset
    //BRUTE FORCE takes two points in dataset
    void generate(const P2DS& p)
    {
        auto g = std::mt19937{(std::random_device{}())};
        std::uniform_int_distribution<> d(0, p.size() - 1);

        a = p[d(g)];
        b = p[d(g)];
    };

    //DEGUEULASSE A CHANGER (inclure dans ransac ?)
    // prend un set de donnnées
    // pour chaque point du set calcul erreur
    // si le calcul est inf a threshold alors on dit que c'est inliers 
    template<typename Error>
    P2DS get_inliers(const P2DS& points, const Error& error, const double threshold_on_error) const
    {
        P2DS out;

        std::for_each(begin(points)
                    , end(points)
                    , [&](auto& a){
                        if ( distance(*this, a) < threshold_on_error )
                            out.push_back(a);
                        }
                    );

        return out;
    }
};





//the line is defined by two points (a,b)
double distance(const Line& l, const P2D& p)
{
    const double cx = l.b.x() - l.a.x();
    const double cy = l.b.y() - l.a.y();

    return std::abs(cy * p.x() - cx * p.y() + l.b.x() * l.a.y() + l.b.y() * l.a.x())
         / std::hypot(cy,cx);
}

double compute_mean_error(const Line& line, const P2DS& points)
{
    double error = 0.0;
    for (auto& p : points)
    {
        error += distance(line, p);
    }

    return error / double(points.size()); 
}

struct Error
{
    double operator()(const Line& line, const P2DS& points) const
    {
        return compute_mean_error(line, points);
    }
};






int main()
{
//     OperationsOnContainers ooc;
//     P2DS a{ {0,0},{1,1},{2,2} }
//     // , b{ {0,0},{2,2} }
//     , b{ {2,2} }
//     ;
//     for (auto& p : a) std::cout << p << ", ";
//         std::cout << std::endl;
//         for (auto& p : b) std::cout << p << ", ";
//         std::cout << std::endl;
//     P2DS c = ooc.filter(a,b);
// for (auto& p : c) std::cout << p << ", ";
//         std::cout << std::endl;
//     // return 0;

    Line ground_truth{ P2D{100.0,-200.0}, P2D{0.0,0.0} };
    std::cout << "ground_truth equation: " << ground_truth.slope() << " * x + " << ground_truth.get_y(0) << std::endl;

    //generating points around the ground truth
    auto g = std::mt19937{(std::random_device{}())};
    std::normal_distribution<double> inliers_distrib(0, 10.0);
    std::normal_distribution<double> outliers_distrib(0, 100.0);

    P2DS points;
    for (double x = -100.0; x < 100.0; x+=.1)
    {
        points.emplace_back(inliers_distrib(g) + x, inliers_distrib(g) + ground_truth.get_y(x));
        points.emplace_back(outliers_distrib(g) + x, outliers_distrib(g) + ground_truth.get_y(x));
    }

    // std::cout << "Dataset:" << std::endl;
    // std::cout << "\tsize: " << points.size() << std::endl;
    // for (auto& p : points)
    //     std::cout << p << std::endl;

    OperationsOnContainers operations;
    Sampling sampling;
    Error error;

    Ransac ransac(int(1e3), 500, 50, 200);
    Line line = ransac.fit<Line>(points, operations, sampling, error);

    std::cout << "ransac ended." << std::endl;
    std::cout << "Line parameters:\n\ta: " << line.a << "\n\tb: " << line.b << std::endl;
    std::cout << "Line equation: " << line.slope() << " * x + " << line.get_y(0) << std::endl;

    return 0;
}
