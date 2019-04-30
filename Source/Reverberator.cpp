/*
  ==============================================================================

    Reverberator.cpp
    Created: 15 Apr 2019 11:49:33am
    Author:  Ekaterina Poklonskaya

  ==============================================================================
*/

#include "Reverberator.h"
#include "math.h"

Reverberator::Reverberator(FdnDimension dim, std::vector<int>& powers) :
        dimension(dim),
        delayLines((std::size_t)dim, (std::size_t)dim, 0.f)
{
    CalculateMaxPowerValues();
    GenerateDelayValues(powers);
    
    matrixes.emplace(std::piecewise_construct,
                     std::forward_as_tuple(FdnDimension::matrix2d),
                     std::forward_as_tuple(2, commonMatrixGain * 1.f / std::sqrt(2.f)));
    matrixes.emplace(std::piecewise_construct,
                     std::forward_as_tuple(FdnDimension::matrix4d),
                     std::forward_as_tuple(4, commonMatrixGain * 0.5f));
    matrixes.emplace(std::piecewise_construct,
                     std::forward_as_tuple(FdnDimension::matrix8d),
                     std::forward_as_tuple(8, commonMatrixGain * 0.5f / std::sqrt(2.f)));
    matrixes.emplace(std::piecewise_construct,
                     std::forward_as_tuple(FdnDimension::matrix16d),
                     std::forward_as_tuple(16, commonMatrixGain * 0.25f));
}

void Reverberator::GenerateDelayValues(std::vector<int>& powers)
{
    delayValues.clear();
    auto idxPrimes = 0;
    auto maxDelay = 0;
    for (auto &it : powers)
    {
        auto power = (it % maxPowValues[idxPrimes]) ? (it % maxPowValues[idxPrimes]) : maxPowValues[idxPrimes];
        auto newDelayValue = std::pow(PrimesVector[idxPrimes++], power);
        delayValues.push_back(newDelayValue);
        if (newDelayValue > maxDelay)
            maxDelay = newDelayValue;
    }
    std::sort(delayValues.begin(), delayValues.end());
    UpdateDelayLines(maxDelay);
    SetBVector(std::vector<float>((int)dimension, bValue));
    SetCVector(std::vector<float>((int)dimension, cValue));
}

void Reverberator::UpdateDelayLines(int maxDelayLength)
{
    delayLines.Resize((size_t)dimension, maxDelayLength, 0.f);
}

void Reverberator::CalculateMaxPowerValues()
{
    for (auto &it : PrimesVector)
        maxPowValues.push_back(floor(std::log(MaxDelay)) / std::log(it));
}

void Reverberator::SetDimension(FdnDimension dim)
{
    dimension = dim;
}

void Reverberator::SetGain (float gain)
{
    this->gain = gain;
}

void Reverberator::SetBVector(std::vector<float>&& b)
{
    this->bVector = b;
}

void Reverberator::SetCVector(std::vector<float>&& c)
{
    this->cVector = c;
}

void Reverberator::Reverberate(float* audioData, unsigned blockLength, float drywet)
{
    int N = (int)dimension;
    jassert(N == delayValues.size());
    
    int delayDepth = delayLines.GetDimensions().second; // signed type is better whith delayedIdx calculation
    
    int idx = 0;
//    float maxAbsIn = 0.0f;
//    float maxAbsOut = 0.0f;
    
    for (auto n = 0; n < blockLength; ++n)
    {
        float input = audioData[n];
        float output = audioData[n];
        
//        if (std::abs(input) > maxAbsIn)
//            maxAbsIn = std::abs(input);
        
        std::vector<float> tmp(N, 0.f);
        
        for (auto i = 0; i < N; ++i)
        {
            auto delayed_idx = (idx - delayValues[i] + delayDepth) % delayDepth;
            tmp[i] = delayLines.Get(i, delayed_idx);
            output += cVector[i] * tmp[i];
        }
        
        for (auto i = 0; i < N; ++i)
        {
            float dotMultiplication = 0.f;
            for (auto j = 0; j < N; ++j)
                dotMultiplication += tmp[j] * matrixes.at(dimension).Get(i,j);
            delayLines.Set(i, idx, input * bVector[i] + dotMultiplication);
        }
        
        audioData[n] = drywet * output + (1.f - drywet) * input;
        
//        if (std::abs(audioData[n]) > maxAbsOut)
//            maxAbsOut = std::abs(audioData[n]);
        
        idx = (idx + 1) % delayDepth;
    }
    
    
//    auto coefficient = maxAbsIn / maxAbsOut;
//    for (auto n = 0; n < blockLength; ++n)
//        audioData[n] = audioData[n] * coefficient;
}
