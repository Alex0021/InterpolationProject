#include <gtest/gtest.h>
#include <Eigen/Core>
#include "lagrange_interpolator.hpp"
#include "test_polynomial_interpolator.cpp"

/**
 * @brief Performs a simple test for the Lagrange Interpolator 
 * using f(x) = x^2 (1D function, basic case)
 * P.S. should be exact inteprolation if n points > 2
 * 
 */
class LagrangeInterpolatorTest: public PolynomialInterpolatorTest {
    protected:

        void SetUp() override {
            interpolator = (PolynomialInterpolator<double>*) new LagrangeInterpolator<double>();
            PolynomialInterpolatorTest::SetUp();
        };

};

TEST_F(LagrangeInterpolatorTest, ExactPoints) {this->ExactPoints();}
TEST_F(LagrangeInterpolatorTest, FitDataUsingDim){this->FitDataUsingDim();}
TEST_F(LagrangeInterpolatorTest, FitDataUsingY)  {this->FitDataUsingY();}
TEST_F(LagrangeInterpolatorTest, FitRange)       {this->FitRange();}
TEST_F(LagrangeInterpolatorTest, VectorDatapoints) {this->VectorDatapoints();}
TEST_F(LagrangeInterpolatorTest, Extrapolation) {this->Extrapolation();}
TEST_F(LagrangeInterpolatorTest, SingleDataPoint) {this->SingleDataPoint();}
TEST_F(LagrangeInterpolatorTest, Multidimensional) {this->Multidimensional();}

