/*
  ==============================================================================

    Matrix.h
    Created: 15 Apr 2019 11:49:47am
    Author:  Ekaterina Poklonskaya

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "vector"

template <typename T> class Matrix
{
public:
    Matrix(size_t inRowsSize, size_t inColsSize, T initVals) {
        Resize(inRowsSize, inColsSize, initVals);
    };

    Matrix(const std::initializer_list<std::initializer_list<T>> initVals) {
        rowsSize = initVals.begin()->size();
        colsSize = 0;
        for (auto &it : initVals)
        {
            jassert(it.size() == rowsSize);
            matrixVals.push_back(it);
            ++colsSize;
        }
    };

    void Clear() {
        matrixVals.clear();
        rowsSize = 0;
        colsSize = 0;
    };

    void Resize(size_t inRowsSize, size_t inColsSize, T initVals) {
        Clear();
        rowsSize = inRowsSize;
        colsSize = inColsSize;
        for (auto i = 0; i < inRowsSize; ++i)
            matrixVals.push_back(std::vector<T>(inColsSize, initVals));
    };

    void Set(std::size_t row, std::size_t col, T val) {
        matrixVals[row][col] = val;
    };

    void SetRow(std::size_t row, std::vector<T>&& rowVal) {
        jassert(rowVal.size() == colsSize);
        matrixVals[row] = rowVal;
    };

    T Get(std::size_t row, std::size_t col) const {
        return matrixVals[row][col];
    };

    std::vector<T> GetRow(std::size_t row) const {
        return matrixVals[row];
    };

    std::pair<std::size_t, std::size_t> GetDimentions() const {
        return std::make_pair(rowsSize, colsSize);
    };

protected:
    std::vector<std::vector<T>> matrixVals;
    std::size_t rowsSize = 0;
    std::size_t colsSize = 0;
};



class HadamarMatrix : public Matrix<float>
{
public:
    HadamarMatrix(std::size_t dimention, float gain = 1.0f) : Matrix<float>(dimention, dimention, 0.0f) {
        jassert(!(dimention & (dimention - 1))); // is dimention = power of 2
        FillMatrix(dimention, gain);
    }
private:
    void FillMatrix(std::size_t dimention, float gain) {
        if (dimention == 1)
            matrixVals[0][0] =  1.0f * gain;
        else
        {
            decltype(dimention) halfDimention = dimention / 2;
            HadamarMatrix(halfDimention, gain);

            for (auto i = halfDimention; i < dimention; ++i)
                for (auto j = 0; j < halfDimention; ++j)
                    matrixVals[i][j] = matrixVals[i - halfDimention][j];
            for (auto i = 0; i < halfDimention; ++i)
                for (auto j = halfDimention; j < dimention; ++j)
                    matrixVals[i][j] = matrixVals[i][j - halfDimention];
            for (auto i = halfDimention; i < dimention; ++i)
                for (auto j = halfDimention; j < dimention; ++j)
                    matrixVals[i][j] = -matrixVals[i - halfDimention][j - halfDimention];
        }
    }
};
