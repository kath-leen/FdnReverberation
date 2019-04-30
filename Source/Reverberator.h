/*
  ==============================================================================

    Reverberator.h
    Created: 15 Apr 2019 11:49:33am
    Author:  Ekaterina Poklonskaya

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "vector"

#include "Matrix.h"


class Reverberator
{
public:
    enum class FdnDimension
    {
        matrix2d = 2, matrix4d = 4, matrix8d = 8, matrix16d = 16
    };
    
    Reverberator(FdnDimension dim, std::vector<int>& powers);
    ~Reverberator() {};
    
    void Reverberate(float* audioData, unsigned blockLength, float drywet = 0.5f);
    void GenerateDelayValues(std::vector<int>& powers);
    void SetDimension(FdnDimension dim);
    void SetGain (float gain);
    void SetBVector(std::vector<float>&& b);
    void SetCVector(std::vector<float>&& c);
    
private:
    void UpdateDelayLines(int maxDelayLength);
    void CalculateMaxPowerValues();
    
    FdnDimension dimension;
    Matrix<float> delayLines;
    std::vector<int> delayValues;
    float gain = 0.8f;
    std::vector<float> bVector;
    std::vector<float> cVector;
    std::vector<int> maxPowValues; // the restriction to prevent the creation of very long delays (calculates based on the MaxDelay value)
    
    std::map<FdnDimension, const HadamarMatrix> matrixes;
    
    const HadamarMatrix* currentMatrix = nullptr;
    
    const float bValue = 1.f;
    const float cValue = 0.8f;
    const float commonMatrixGain = 0.97f;
    
    const std::vector<int> PrimesVector =
    {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101};
    
    const int MaxDelay = 50000;

};
