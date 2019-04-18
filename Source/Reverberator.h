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
    enum class FdnDimentions
    {
        matrix2d = 2, matrix4d = 4, matrix8d = 8, matrix16d = 16
    };
    
    Reverberator(FdnDimentions inDim = FdnDimentions::matrix4d, std::vector<int>&& inPowers = {1, 2, 3, 4});
    ~Reverberator() {};
    
    void Reverberate(float* audioData, unsigned blockLength, float drywet = 0.5f);
    void GenerateDelayValues(std::vector<int>& inPowers);
    void SetMatrix(FdnDimentions inDim);
    void SetGain (float inGain);
    void SetBVector(std::vector<float>&& inB);
    void SetCVector(std::vector<float>&& inC);
    
private:
    void UpdateDelayLines(int maxDelayLength);
    
    FdnDimentions dimention;
    Matrix<float> delayLines; //std::vector<std::unique_ptr<float>> delayLines;
    std::vector<int> delayValues;
    float gain = 0.8f;
    std::vector<float> bVector;
    std::vector<float> cVector;
    
    const HadamarMatrix matrix2x2;
    const HadamarMatrix matrix4x4;
    const HadamarMatrix matrix8x8;
    const HadamarMatrix matrix16x16;
    
    const HadamarMatrix* currentMatrix = &matrix4x4;
    
    const std::vector<int> PrimesVector =
    {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101};
    
    const float bValue = 1.f;
    const float cValue = 0.8f;

};
