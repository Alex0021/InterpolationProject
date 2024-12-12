#include <gtest/gtest.h>
#include <Eigen/Core>
#include "datagen.hpp"
#include "cubic_spline_interpolator.hpp"
#include "test_polynomial_interpolator.cpp"

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
TEST_F(CubicSplineInterpolatorTest, VectorDatapoints) {this->VectorDatapoints();}
TEST_F(CubicSplineInterpolatorTest, Extrapolation) {this->Extrapolation();}
TEST_F(CubicSplineInterpolatorTest, SingleDataPoint) {this->SingleDataPoint();}
TEST_F(CubicSplineInterpolatorTest, Multidimensional) {this->Multidimensional();}

class ClampedCubicSplineInterpolatorTest: public PolynomialInterpolatorTest {
    protected:
        void SetUp() override {
            interpolator = (PolynomialInterpolator<double>*) new CubicSplineInterpolator<double>(CubicSplineInterpolator<double>::BoundaryConstraint::CLAMPED, Eigen::Vector2d(1,-1));
            PolynomialInterpolatorTest::SetUp();
        };
};

TEST_F(ClampedCubicSplineInterpolatorTest, FitDataUsingDim){this->FitDataUsingDim();}
TEST_F(ClampedCubicSplineInterpolatorTest, FitDataUsingY)  {this->FitDataUsingY();}
TEST_F(ClampedCubicSplineInterpolatorTest, FitRange)       {this->FitRange();}
TEST_F(ClampedCubicSplineInterpolatorTest, VectorDatapoints) {this->VectorDatapoints();}
TEST_F(ClampedCubicSplineInterpolatorTest, Extrapolation) {this->Extrapolation();}
TEST_F(ClampedCubicSplineInterpolatorTest, SingleDataPoint) {this->SingleDataPoint();}
TEST_F(ClampedCubicSplineInterpolatorTest, Multidimensional) {this->Multidimensional();}