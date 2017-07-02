#include "ransac.hpp"

Ransac::Ransac(const int max_it, const int smp_nbr, const double t, const int m_f)
: max_iterations(max_it), sample_number(smp_nbr), threshold(t), min_fit(m_f)
{
}

Ransac::~Ransac()
{

}

bool Ransac::stop() const
{
    return current_iteration < max_iterations;
}
