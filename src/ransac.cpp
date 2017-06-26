#include "ransac.hpp"

Ransac::Ransac()
{

}

Ransac::~Ransac()
{

}

bool Ransac::stop(const int it) const
{
    return (it >= max_iterations);
}
