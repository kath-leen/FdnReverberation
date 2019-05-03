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
#include "utility"

template <typename T> class Matrix
{
public:
    Matrix(size_t rowsSize, size_t colsSize, T&& initVals) {
        Resize(rowsSize, colsSize, std::forward<T>(initVals));
    };

    Matrix(const std::initializer_list<std::initializer_list<T>>&& initVals) {
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

    void Resize(size_t rowsSize, size_t colsSize, T&& initVals) {
        Clear();
        this->rowsSize = rowsSize;
        this->colsSize = colsSize;
        for (auto i = 0; i < rowsSize; ++i)
            matrixVals.emplace_back(colsSize, std::forward<T>(initVals));
    };

    void Set(std::size_t row, std::size_t col, const T&& val) {
        matrixVals[row][col] = std::forward<const T>(val);
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

    std::pair<std::size_t, std::size_t> GetDimensions() const {
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
    HadamarMatrix(std::size_t dimension, float gain = 1.0f) : Matrix<float>(dimension, dimension, 0.0f) {
        jassert(!(dimension & (dimension - 1))); // is dimension = power of 2
        FillMatrix(dimension, gain);
    }
private:
    void FillMatrix(std::size_t dimension, float gain) {
        if (dimension == 1)
            matrixVals[0][0] =  1.0f * gain;
        else
        {
            std::size_t halfDimension = dimension / 2;
            FillMatrix(halfDimension, gain);

            for (auto i = halfDimension; i < dimension; ++i)
                for (auto j = 0; j < halfDimension; ++j)
                    matrixVals[i][j] = matrixVals[i - halfDimension][j];
            for (auto i = 0; i < halfDimension; ++i)
                for (auto j = halfDimension; j < dimension; ++j)
                    matrixVals[i][j] = matrixVals[i][j - halfDimension];
            for (auto i = halfDimension; i < dimension; ++i)
                for (auto j = halfDimension; j < dimension; ++j)
                    matrixVals[i][j] = -matrixVals[i - halfDimension][j - halfDimension];
        }
    }
};
