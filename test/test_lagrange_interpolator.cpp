#include <gtest/gtest.h>
#include <Eigen/Core>
#include "lagrange_interpolator.hpp"

#define TEST_TOLERANCE 1e-12

/**
 * @brief Performs a simple test for the Lagrange Interpolator 
 * using f(x) = x^2 (1D function, basic case)
 * P.S. should be exact inteprolation if n points > 2
 * 
 */
class LagrangeInterpolatorTest: public ::testing::Test {
    protected:
        Eigen::VectorXd querry_points;
        Eigen::VectorXd labels;
        Eigen::MatrixXd X;
        LagrangeInterpolator<double> interpolator;

        void SetUp() override {
            Eigen::VectorXd x = Eigen::VectorXd::LinSpaced(11, -10, 10);
            Eigen::VectorXd y = x.cwisePow(2);
            X = Eigen::MatrixXd(x.rows(), 2);
            X << x, y;
            querry_points = 10.0*Eigen::VectorXd::Random(50);
            labels = querry_points.cwisePow(2);
            interpolator = LagrangeInterpolator<double>();
        };
};

TEST_F(LagrangeInterpolatorTest, FitDataUsingDim)
{
    // Check function fit w/ (X,dim)
    interpolator.fit(X, 1);
    ASSERT_TRUE(X.col(0).isApprox(interpolator.get_X_data()));
    ASSERT_TRUE(X.col(1).isApprox(interpolator.get_y_data()));

    // Checking function fit w/ wrong dim
    ASSERT_ANY_THROW(interpolator.fit(X,2)) << "Fitting 2D data with dim_idx > 1 should throw an exception!";
}

TEST_F(LagrangeInterpolatorTest, FitDataUsingY)
{
    // Check function fit w/ (X,y)
    interpolator.fit(X.col(0), X.col(1));
    ASSERT_TRUE(X.col(0).isApprox(interpolator.get_X_data()));
    ASSERT_TRUE(X.col(1).isApprox(interpolator.get_y_data()));

    // Check function fit w/ wrong y dim
    Eigen::Vector4d idx = Eigen::Vector4d::LinSpaced(0,3);
    ASSERT_ANY_THROW(interpolator.fit(X, X.col(1)(idx)));
}

TEST_F(LagrangeInterpolatorTest, FitRange)
{
    interpolator.fit(X, 1);
    auto range = interpolator.get_range();
    EXPECT_DOUBLE_EQ(std::get<0>(range)(0), -10.0) << "Lower bound is " << std::get<0>(range)(0) << " instead of -10.0!";
    EXPECT_DOUBLE_EQ(std::get<1>(range)(0), 10.0)<< "Upper bound is " << std::get<1>(range)(0) << " instead of 10.0!";
}


TEST_F(LagrangeInterpolatorTest, SingleDataPoint)
{
    ASSERT_NO_THROW(interpolator.fit(X, 1)) << "Fitting interpolator failed, cannot test interpolation characteristics!";

    for (unsigned int i=0; i<querry_points.rows(); i++)
    {
        EXPECT_NEAR(interpolator(querry_points(i)), labels(i), TEST_TOLERANCE);
    }
}

TEST_F(LagrangeInterpolatorTest, VectorDatapoints)
{
    ASSERT_NO_THROW(interpolator.fit(X, 1)) << "Fitting interpolator failed, cannot test interpolation characteristics!";

    Eigen::VectorXd y = interpolator(querry_points);
    for (unsigned int i=0; i<y.rows(); i++)
    {
        EXPECT_NEAR(y(i), labels(i), TEST_TOLERANCE);
    }
}

TEST_F(LagrangeInterpolatorTest, ExactPoints)
{
    ASSERT_NO_THROW(interpolator.fit(X, 1)) << "Fitting interpolator failed, cannot test interpolation characteristics!";

    for (unsigned int i=0; i<X.rows(); i++)
    {
        EXPECT_DOUBLE_EQ(interpolator(X(i,0)), X(i,1));
    }
}

TEST_F(LagrangeInterpolatorTest, Extrapolation)
{
    ASSERT_NO_THROW(interpolator.fit(X, 1)) << "Fitting interpolator failed, cannot test interpolation characteristics!";
    // New out of range y vector
    Eigen::VectorXd querry_points_extra = 15.0*Eigen::VectorXd::Random(50);
    ASSERT_ANY_THROW(interpolator(querry_points_extra)) << "Extrapolation should not be permitted!";

    querry_points_extra = -10.1*Eigen::VectorXd::Random(50);
    ASSERT_ANY_THROW(interpolator(querry_points_extra)) << "Extrapolation should not be permitted!";
}

TEST_F(LagrangeInterpolatorTest, Multidimensional)
{
    ASSERT_NO_THROW(interpolator.fit(X, 1)) << "Fitting interpolator failed, cannot test interpolation characteristics!";
    // Check for an excpetion if passing multidimensional datapoints
    ASSERT_ANY_THROW(interpolator(X)) << "Multidimensional interpolation should not throw an exception!";
}
