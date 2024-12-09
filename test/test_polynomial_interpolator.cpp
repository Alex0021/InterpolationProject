#include <gtest/gtest.h>
#include "polynomial_interpolator.hpp"
#include "Eigen/Core"

/**
 * @brief Performs a simple test for the Polynomial Interpolator 
 * using f(x) = x^2 (1D function, basic case) within the
 * range [-10,10] using 11 datapoints.
 * 
 * Test for fitting functions w/ dim or y
 * 
 * Test for range calculation [low , high]
 */
class PolynomialInterpolatorTest: public ::testing::Test 
{
    protected:
        Eigen::Matrix<double, -1, 2> X;
        PolynomialInterpolator<double> interpolator;
    
        void SetUp() override {
            Eigen::VectorXd x = Eigen::VectorXd::LinSpaced(11,-10,10);
            Eigen::VectorXd y = x.cwisePow(2);
            X = Eigen::MatrixXd(x.rows(), 2);
            X << x, y;
            interpolator = PolynomialInterpolator<double>();
        }
};

TEST_F(PolynomialInterpolatorTest, FitDataUsingDim)
{
    // Check function fit w/ (X,dim)
    interpolator.fit(X, 1);
    ASSERT_TRUE(X.col(0).isApprox(interpolator.get_X_data()));
    ASSERT_TRUE(X.col(1).isApprox(interpolator.get_y_data()));

    // Checking function fit w/ wrong dim
    ASSERT_ANY_THROW(interpolator.fit(X,2)) << "Fitting 2D data with dim_idx > 1 should throw an exception!";
}

TEST_F(PolynomialInterpolatorTest, FitDataUsingY)
{
    // Check function fit w/ (X,y)
    interpolator.fit(X.col(0), X.col(1));
    ASSERT_TRUE(X.col(0).isApprox(interpolator.get_X_data()));
    ASSERT_TRUE(X.col(1).isApprox(interpolator.get_y_data()));

    // Check function fit w/ wrong y dim
    Eigen::Vector4d idx = Eigen::Vector4d::LinSpaced(0,3);
    ASSERT_ANY_THROW(interpolator.fit(X, X.col(1)(idx)));
}

TEST_F(PolynomialInterpolatorTest, FitRange)
{
    interpolator.fit(X, 1);
    auto range = interpolator.get_range();
    EXPECT_DOUBLE_EQ(std::get<0>(range)(0), -10.0) << "Lower bound is " << std::get<0>(range)(0) << " instead of -10.0!";
    EXPECT_DOUBLE_EQ(std::get<1>(range)(0), 10.0)<< "Upper bound is " << std::get<1>(range)(0) << " instead of 10.0!";
}