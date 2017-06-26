#pragma once

#include <vector>

template<typename T, int S> struct Point
{
    T data[S];

    template<typename ... X> Point(X ... t)
    : data{ T(t)... }
    {}

    Point(){}

    template<int I> T& at()
    {
        static_assert(S > I,"Error: Point::at() out of range.");
        return data[I];
    }

    template<int I> T const& at() const
    {
        static_assert(S > I,"Error: Point::at() out of range."); return data[I];
    }

    T & x() { return at<0>(); }
    T & y() { return at<1>(); }
    T & z() { return at<2>(); }

    T const & x() const { return at<0>(); }
    T const & y() const { return at<1>(); }
    T const & z() const { return at<2>(); }

    T norm() const
    {
        return spec_norm(*this);
    }
};

using P2D = Point<double, 2>;
using P3D = Point<double, 3>;

using P2DS = std::vector<P2D>;
using P3DS = std::vector<P3D>;

double distance(const P2D& p1, const P2D& p2)
{
    return std::hypot(p1.x() - p2.x(), p1.y() - p2.y());
}

std::ostream& operator<<(std::ostream& o, const P2D& p)
{
    o << "{" << p.x() << ", " << p.y() << "}";

    return o;
}

std::ostream& operator<<(std::ostream& o, const P3D& p)
{
    o << "{" << p.x() << ", " << p.y() << ", " << p.z() << "}";

    return o;
}
