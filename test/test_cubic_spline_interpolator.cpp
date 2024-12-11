#include <gtest/gtest.h>
#include <Eigen/Core>
#include "datagen.hpp"
#include "cubic_spline_interpolator.hpp"
#include "test_polynomial_interpolator.cpp"

#define TEST_TOLERANCE 1e-12

class CubicSplineInterpolatorTest: public PolynomialInterpolatorTest {
    protected:
        void SetUp() override {
            interpolator = (PolynomialInterpolator<double>*) new CubicSplineInterpolator<double>();
            PolynomialInterpolatorTest::SetUp();
        };
};

TEST_F(CubicSplineInterpolatorTest, FitDataUsingDim){this->FitDataUsingDim();}
TEST_F(CubicSplineInterpolatorTest, FitDataUsingY)  {this->FitDataUsingY();}
TEST_F(CubicSplineInterpolatorTest, FitRange)       {this->FitRange();}
