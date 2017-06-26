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
    bestfit – model parameters which best fit the data (or nul if no good model is found)

iterations = 0
bestfit = nul
besterr = something really large
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
        if thiserr < besterr
        {
            bestfit = bettermodel
            besterr = thiserr
        }
    }

    increment iterations
}
return bestfit
*/

struct Ransac
{
    int max_iterations;
    int min_n;
    int threshold;
    int min_fit;

    int iterations;
    int best_error;

    Ransac();
    ~Ransac();

    bool stop(const int it) const;

    template<typename Model, typename DataType>
    void get_model(Model& maybe_model, DataType& also_inliers);

    template<typename Model, typename DataType>
    Model fit(const DataType& data);
};

template<typename Model, typename DataType>
void Ransac::get_model(Model& maybe_model, DataType& also_inliers)
{
    // for (auto d : data)
    // {
    //     if (!is_inside(maybe_inliers, d))
    //     {
    //         if (is_fitting(maybe_model, d, threshold) )
    //         {
    //             add(also_inliers, d);

    //             if (size(also_inliers) > min_fit)
    //             {
    //                 break;
    //             }
    //         }
    //     }
    // }
}

template<typename Model, typename DataType>
Model Ransac::fit(const DataType& data)
{
    //initiate system
    iterations = 0;
    best_error = std::numeric_limits<double>::max();
    Model best_model;

    while (stop(iterations))
    {
        DataType maybe_inliers = get_sample(data, min_n);

        //recursif ?
        //generer un modèle qui colle avec le set donné
        Model maybe_model = generate(maybe_inliers);

        //constructeur ? dois etre vide !!!!!
        DataType also_inliers;
        empty(also_inliers);

        get_model(maybe_model, also_inliers);

        //on a trouvé assez de données on peut tester maybe_model

        ++iterations;
    }
}

// template<typename DataType>
// template<typename Model>
// Model Ransac<DataType>::fit(DataType data)
    // Model m;
