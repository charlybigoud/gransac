#include <iostream>
#include <random>
#include <algorithm>

#include <ransac.hpp>

#include "point.hpp"

struct SomePoints
{
    P2DS points;

    size_t size() const { return points.size(); };

    //sample n data from the dataset
    SomePoints sample(const int n) const
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

        return SomePoints{out};
    };

    SomePoints operator+(const SomePoints& input) const
    {
        P2DS output = points;
        for (auto& p : input.points)
            output.push_back(p);

        return SomePoints{output};
    };

    SomePoints operator-(const SomePoints& input) const
    {
        P2DS output;
        for (auto& p : input.points)
        {
            bool to_filter = false;
            for (auto& rp : points)
            {
                if ((rp.x() == p.x())
                and (rp.y() == p.y())
                )
                    to_filter = true;

            }
            if (!to_filter)
                output.push_back(p);
        }
        return SomePoints{output};
    };
};





struct Line
{
    P2D a, b;

    double slope() const { return (b.y() - a.y()) / (b.x() - a.x()); }
    double get_y(const double x) const { return slope() * x + (a.y() - a.x() * slope()); }

    //generates (a,b) according to dataset
    //BRUTE FORCE takes two points in dataset
    void generate(const SomePoints& p)
    {
        auto g = std::mt19937{(std::random_device{}())};
        std::uniform_int_distribution<> d(0, p.size() - 1);

        a = p.points[d(g)];
        b = p.points[d(g)];
    };

    //DEGUEULASSE A CHANGER
    // prend un set de donnnées
    // pour chaque point du set calcul erreur
    // si le calcul est inf a threshold alors on dit que c'est inliers 
    template<typename Error>
    SomePoints get_inliers(const SomePoints& points, const Error& error, const double threshold_on_error) const
    {
        P2DS out;
        for (auto& p : points.points)
        {
            if ( error(*this, SomePoints{P2DS{p}}) < threshold_on_error )
            {
                out.push_back(p);
            }
        }

        return SomePoints{out};
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

double compute_mean_error(const Line& line, const SomePoints& points)
{
    double error = 0.0;
    for (auto& p : points.points)
    {
        error += distance(line, p);
    }

    return error / double(points.size()); 
}

struct Error
{
    double operator()(const Line& line, const SomePoints& points) const
    {
        return compute_mean_error(line, points);
    }
};






int main()
{
    Line ground_truth{ P2D{100.0,-200.0}, P2D{0.0,0.0} };
    std::cout << "ground_truth slope: " << ground_truth.slope() << std::endl;

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

    SomePoints dataset{points};
    std::cout << "Dataset:" << std::endl;
    std::cout << "\tsize: " << dataset.size() << std::endl;
    // for (auto& p : points)
    //     std::cout << p << std::endl;

    Error error;

    Ransac ransac(int(1e3), 500, 50, 500);
    Line line = ransac.fit<Line>(dataset, error);

    std::cout << "ransac ended." << std::endl;
    std::cout << "Line parameters:\n\ta: " << line.a << "\n\tb: " << line.b << std::endl;
    std::cout << "Line equation: " << line.slope() << " * x + " << line.get_y(0) << std::endl;

    return 0;
}
