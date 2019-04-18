/*
  ==============================================================================

    Reverberator.cpp
    Created: 15 Apr 2019 11:49:33am
    Author:  Ekaterina Poklonskaya

  ==============================================================================
*/

#include "Reverberator.h"

Reverberator::Reverberator(FdnDimentions inDim, std::vector<int>&& inPowers) :
        dimention(inDim),
        delayLines((std::size_t)inDim, (std::size_t)inDim, 0.f),
        matrix2x2(2, 1.f / std::sqrt(2.f)),
        matrix4x4(4, 0.5f),
        matrix8x8(8, 0.5f / std::sqrt(2.f)),
        matrix16x16(16, 0.25f)
{
    GenerateDelayValues(inPowers);
    SetMatrix(inDim);
}

void Reverberator::GenerateDelayValues(std::vector<int>& inPowers)
{
    delayValues.clear();
    auto idxPrimes = 0;
    auto maxDelay = 0;
    for (auto &it : inPowers)
    {
        auto newDelayValue = std::pow(PrimesVector[idxPrimes++], it);
        delayValues.push_back(newDelayValue);
        if (newDelayValue > maxDelay)
            maxDelay = newDelayValue;
    }
    UpdateDelayLines(maxDelay);
    SetBVector(std::vector<float>((int)dimention, bValue));
    SetCVector(std::vector<float>((int)dimention, cValue));
}

void Reverberator::UpdateDelayLines(int maxDelayLength)
{
    delayLines.Resize((size_t)dimention, maxDelayLength, 0.f);
    
//    for (auto i = 0; i < (int)dimention; ++i)
//        delayLines.push_back(std::unique_ptr<float>(new float[maxDelayLength] {0.f})); //TODO check zeros!
}

void Reverberator::SetMatrix(FdnDimentions inDim)
{
    dimention = inDim;
    switch(inDim) {
        case FdnDimentions::matrix2d:
            currentMatrix = &matrix2x2;
            break;
        case FdnDimentions::matrix4d:
            currentMatrix = &matrix4x4;
            break;
        case FdnDimentions::matrix8d:
            currentMatrix = &matrix8x8;
            break;
        case FdnDimentions::matrix16d:
            currentMatrix = &matrix16x16;
            break;
    }
}

void Reverberator::SetGain (float inGain)
{
    gain = inGain;
}

void Reverberator::SetBVector(std::vector<float>&& inB)
{
    bVector = inB;
}

void Reverberator::SetCVector(std::vector<float>&& inC)
{
    cVector = inC;
}

void Reverberator::Reverberate(float* audioData, unsigned blockLength, float drywet)
{
    int N = (int)dimention;
    jassert(N == delayValues.size());
    
    int idx = 0;
    std::vector<float> tmp(N, 0.f);
    for (auto n = 0; n < blockLength; ++n)
    {
        float input = audioData[n];
        float output = audioData[n];
        for (auto i = 0; i < N; ++i)
        {
            auto delayed_idx = (idx - delayValues[i] + N) % N;
            tmp[i] = delayLines.Get(i, delayed_idx);
            output += cVector[i] * tmp[i];
        }
        for (auto i = 0; i < N; ++i)
        {
            float dotMultiplication = 0.f;
            for (auto j = 0; j < N; ++j)
                dotMultiplication += tmp[j] * currentMatrix->Get(i, j);
            delayLines.Set(i, idx, input * bVector[i] + dotMultiplication);
        }
        audioData[n] = drywet * output + (1.f - output) * input;
        idx = (idx + 1) % N;
    }
}
