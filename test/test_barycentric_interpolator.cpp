#include <gtest/gtest.h>
#include <Eigen/Core>
#include "barycentric_interpolator.hpp"
#include "test_polynomial_interpolator.cpp"

/**
 * @brief Performs some simple tests for the Barycentric Interpolator 
 * using f(x) = x^2 (1D function, basic case)
 * 
 * P.S. should be exact inteprolation if n points > 2
 * 
 * Tests are also provided for adding datapoints
 */
class BarycentricInterpolatorTest: public PolynomialInterpolatorTest {
    protected:
        BarycentricInterpolator<double>* barycentric_interpolator;

        void SetUp() override {
            barycentric_interpolator = new BarycentricInterpolator<double>();
            interpolator = (PolynomialInterpolator<double>*) barycentric_interpolator; // pointer for base class tests
            PolynomialInterpolatorTest::SetUp();
        };

    void AddSingleDatapoint()
    {
        double new_x = 6.25;
        double new_y = 6.25*6.25;

        ASSERT_NO_THROW(barycentric_interpolator->add_data(new_x, new_y));

        // Verify interpolation
        Eigen::VectorXd y = (*barycentric_interpolator)(query_points);
        for (unsigned int i=0; i<y.rows(); i++)
        {
            EXPECT_NEAR(y(i), labels(i), TEST_TOLERANCE);
        }
    }

    void AddMultipleDatapoints()
    {
        // New data
        Eigen::VectorXd new_X(5);
        new_X << -12, -11, 11, 12, 12.5;
        Eigen::VectorXd new_y = new_X.cwisePow(2);

        ASSERT_NO_THROW(barycentric_interpolator->add_data(new_X, new_y));

        // Verify interpolation
        Eigen::VectorXd y = (*barycentric_interpolator)(query_points);
        for (unsigned int i=0; i<y.rows(); i++)
        {
            EXPECT_NEAR(y(i), labels(i), TEST_TOLERANCE);
        }
    }
        
};

TEST_F(BarycentricInterpolatorTest, FitDataUsingDim){this->FitDataUsingDim();}
TEST_F(BarycentricInterpolatorTest, FitDataUsingY)  {this->FitDataUsingY();}
TEST_F(BarycentricInterpolatorTest, FitRange)       {this->FitRange();}
TEST_F(BarycentricInterpolatorTest, VectorDatapoints) {this->VectorDatapoints();}
TEST_F(BarycentricInterpolatorTest, Extrapolation) {this->Extrapolation();}
TEST_F(BarycentricInterpolatorTest, SingleDataPoint) {this->SingleDataPoint();}
TEST_F(BarycentricInterpolatorTest, Multidimensional) {this->Multidimensional();}
TEST_F(BarycentricInterpolatorTest, ExactPoints) {this->ExactPoints();}
TEST_F(BarycentricInterpolatorTest, AddSingleDatapoint) {this->AddSingleDatapoint();}
TEST_F(BarycentricInterpolatorTest, AddMultipleDatapoints) {this->AddMultipleDatapoints();}

