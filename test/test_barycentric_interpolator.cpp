#include <gtest/gtest.h>
#include <Eigen/Core>
#include "barycentric_interpolator.hpp"

#define TEST_TOLERANCE 1e-12

/**
 * @brief Performs some simple tests for the Barycentric Interpolator 
 * using f(x) = x^2 (1D function, basic case)
 * 
 * P.S. should be exact inteprolation if n points > 2
 * 
 * Tests are also provided for adding datapoints
 */
class BarycentricInterpolatorTest: public ::testing::Test {
    protected:
        Eigen::VectorXd querry_points;
        Eigen::VectorXd labels;
        Eigen::MatrixXd X;
        BarycentricInterpolator<double> interpolator;

        void SetUp() override {
            Eigen::VectorXd x = Eigen::VectorXd::LinSpaced(11, -10, 10);
            Eigen::VectorXd y = x.cwisePow(2);
            X = Eigen::MatrixXd(x.rows(), 2);
            X << x, y;
            querry_points = 10.0*Eigen::VectorXd::Random(50);
            labels = querry_points.cwisePow(2);
            interpolator = BarycentricInterpolator<double>();
            ASSERT_NO_THROW(interpolator.fit(X, 1)) << "Fitting interpolator failed, cannot test interpolation characteristics!";
        };
};


TEST_F(BarycentricInterpolatorTest, SingleDataPoint)
{
    for (unsigned int i=0; i<querry_points.rows(); i++)
    {
        EXPECT_NEAR(interpolator(querry_points(i)), labels(i), TEST_TOLERANCE);
    }
}

TEST_F(BarycentricInterpolatorTest, VectorDatapoints)
{
    Eigen::VectorXd y = interpolator(querry_points);
    for (unsigned int i=0; i<y.rows(); i++)
    {
        EXPECT_NEAR(y(i), labels(i), TEST_TOLERANCE);
    }
}

TEST_F(BarycentricInterpolatorTest, ExactPoints)
{
    for (unsigned int i=0; i<X.rows(); i++)
    {
        EXPECT_DOUBLE_EQ(interpolator(X(i,0)), X(i,1));
    }
}

TEST_F(BarycentricInterpolatorTest, AddSingleDatapoint)
{
    double new_x = 6.25;
    double new_y = 6.25*6.25;

    ASSERT_NO_THROW(interpolator.add_data(new_x, new_y));

    // Verify interpolation
    Eigen::VectorXd y = interpolator(querry_points);
    for (unsigned int i=0; i<y.rows(); i++)
    {
        EXPECT_NEAR(y(i), labels(i), TEST_TOLERANCE);
    }
}

TEST_F(BarycentricInterpolatorTest, AddMultipleDatapoints)
{
    // New data
    Eigen::VectorXd new_X(5);
    new_X << -12, -11, 11, 12, 12.5;
    Eigen::VectorXd new_y = new_X.cwisePow(2);

    ASSERT_NO_THROW(interpolator.add_data(new_X, new_y));

    // Verify interpolation
    Eigen::VectorXd y = interpolator(querry_points);
    for (unsigned int i=0; i<y.rows(); i++)
    {
        EXPECT_NEAR(y(i), labels(i), TEST_TOLERANCE);
    }
}

TEST_F(BarycentricInterpolatorTest, Extrapolation)
{
    // New out of range y vector
    Eigen::VectorXd querry_points_extra = 15.0*Eigen::VectorXd::Ones(50);
    ASSERT_ANY_THROW(interpolator(querry_points_extra)) << "Extrapolation should not be permitted!";

    querry_points_extra = -10.1*Eigen::VectorXd::Ones(50);
    ASSERT_ANY_THROW(interpolator(querry_points_extra)) << "Extrapolation should not be permitted!";
}

TEST_F(BarycentricInterpolatorTest, Multidimensional)
{
    // Check for an excpetion if passing multidimensional datapoints
    ASSERT_ANY_THROW(interpolator(X)) << "Multidimensional interpolation should not throw an exception!";
}
