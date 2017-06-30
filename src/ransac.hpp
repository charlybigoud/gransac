#pragma once

#include <limits>

/*
Given:
    data – a set of observed data points
    model – a model that can be fitted to data points
    min_n – the minimum number of data values required to fit the model
    max_iterations – the maximum number of iterations allowed in the algorithm
    threshold – a threshold value for determining when a data point fits a model
    min_fit – the number of close data values required to assert that a model fits well to data

Return:
    best_fit – model parameters which best fit the data (or nul if no good model is found)

iterations = 0
best_fit = nul
best_error = something really large
while iterations < max_n
{
    maybe_inliers = min_n randomly selected values from data
    maybe_model = model parameters fitted to maybe_inliers
    also_inliers = empty set

    for every point in data not in maybe_inliers
    {
        if point fits maybe_model with an error smaller than threshold
             add point to also_inliers
    }

    if the number of elements in also_inliers is > min_fit
    {
        % this implies that we may have found a good model
        % now test how good it is
        bettermodel = model parameters fitted to all points in maybe_inliers and also_inliers
        thiserr = a measure of how well model fits these points
        if thiserr < best_error
        {
            best_fit = bettermodel
            best_error = thiserr
        }
    }

    increment iterations
}
return best_fit
*/

struct Ransac
{
    //constants
    int max_iterations; // the maximum number of iterations allowed in the algorithm
    int sample_number; // the minimum number of data values required to fit the model
    double threshold; // a threshold value for determining when a data point fits a model
    int min_fit; // the number of close data values required to assert that a model fits well to data

    //variables
    int current_iteration;
    double best_error;

    Ransac(const int max_it = 1, const int smp_nbr = 1, const double t = 0.0, const int m_f = 1);
    ~Ransac();

    bool stop() const;

    template<typename Model, typename DataSet>
    Model fit(const DataSet& data);
};

template<typename Model, typename DataSet>
Model Ransac::fit(const DataSet& data)
{
    //initiate system
    current_iteration = 0;
    best_error = std::numeric_limits<double>::max();
    Model better_model, best_model;

    while ( stop() )
    {
        DataSet maybe_inliers = data.sample(sample_number);

        //recursif ?
        //generer un modèle qui colle avec le set donné
        Model maybe_model;
        maybe_model.generate(maybe_inliers);

        // wanted to use a sub() function instead of operator- but it has a diffrent meanig to me
        DataSet also_inliers = maybe_model.get_inliers(data - maybe_inliers);

        //on a trouvé assez de données on peut tester maybe_model
        if( also_inliers.size() < min_fit ) //then we may have a good model
        {
            double current_error = better_model.generate(maybe_inliers + also_inliers);

            if (current_error < best_error)
            {
                best_model = better_model;
                best_error = current_error;
            }
        }

        ++current_iteration;
    }
}
