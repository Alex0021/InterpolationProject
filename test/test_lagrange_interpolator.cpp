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
            ASSERT_NO_THROW(interpolator.fit(X, 1)) << "Fitting interpolator failed, cannot test interpolation characteristics!";

        };
};


TEST_F(LagrangeInterpolatorTest, SingleDataPoint)
{
    for (unsigned int i=0; i<querry_points.rows(); i++)
    {
        EXPECT_NEAR(interpolator(querry_points(i)), labels(i), TEST_TOLERANCE);
    }
}

TEST_F(LagrangeInterpolatorTest, VectorDatapoints)
{
    Eigen::VectorXd y = interpolator(querry_points);
    for (unsigned int i=0; i<y.rows(); i++)
    {
        EXPECT_NEAR(y(i), labels(i), TEST_TOLERANCE);
    }
}

TEST_F(LagrangeInterpolatorTest, ExactPoints)
{
    for (unsigned int i=0; i<X.rows(); i++)
    {
        EXPECT_DOUBLE_EQ(interpolator(X(i,0)), X(i,1));
    }
}

TEST_F(LagrangeInterpolatorTest, Extrapolation)
{
    // New out of range y vector
    Eigen::VectorXd querry_points_extra = 15.0*Eigen::VectorXd::Ones(50);
    ASSERT_ANY_THROW(interpolator(querry_points_extra)) << "Extrapolation should not be permitted!";

    querry_points_extra = -10.1*Eigen::VectorXd::Ones(50);
    ASSERT_ANY_THROW(interpolator(querry_points_extra)) << "Extrapolation should not be permitted!";
}

TEST_F(LagrangeInterpolatorTest, Multidimensional)
{
    // Check for an excpetion if passing multidimensional datapoints
    ASSERT_ANY_THROW(interpolator(X)) << "Multidimensional interpolation should not throw an exception!";
}
