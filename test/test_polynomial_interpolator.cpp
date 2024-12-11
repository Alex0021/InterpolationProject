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
class PolynomialInterpolatorTest: public ::testing::Test {
    protected:
        Eigen::Matrix<double, -1, 2> X;
        PolynomialInterpolator<double>* interpolator;

        void SetUp() override {
            Eigen::VectorXd x = Eigen::VectorXd::LinSpaced(11,-10,10);
            Eigen::VectorXd y = x.cwisePow(2);
            X = Eigen::MatrixXd(x.rows(), 2);
            X << x, y;
            ASSERT_NO_THROW(interpolator->fit(X, 1)) << "Fitting interpolator failed, cannot test interpolation characteristics!";
        }

        void TearDown() override {
            delete interpolator;
        }

        void FitDataUsingDim() { 
            // Check function fit w/ (X,dim)
            interpolator->fit(X, 1);
            ASSERT_TRUE(X.col(0).isApprox(interpolator->get_X_data()));
            ASSERT_TRUE(X.col(1).isApprox(interpolator->get_y_data()));

            // Checking function fit w/ wrong dim
            ASSERT_ANY_THROW(interpolator->fit(X,2)) << "Fitting 2D data with dim_idx > 1 should throw an exception!";
        }

        void FitDataUsingY() 
        {
            // Check function fit w/ (X,y)
            interpolator->fit(X.col(0), X.col(1));
            ASSERT_TRUE(X.col(0).isApprox(interpolator->get_X_data()));
            ASSERT_TRUE(X.col(1).isApprox(interpolator->get_y_data()));

            // Check function fit w/ wrong y dim
            Eigen::Vector4d idx = Eigen::Vector4d::LinSpaced(0,3);
            ASSERT_ANY_THROW(interpolator->fit(X, X.col(1)(idx)));
        }

        void FitRange() 
        {
            interpolator->fit(X, 1);
            auto range = interpolator->get_range();
            EXPECT_DOUBLE_EQ(std::get<0>(range)(0), -10.0) << "Lower bound is " << std::get<0>(range)(0) << " instead of -10.0!";
            EXPECT_DOUBLE_EQ(std::get<1>(range)(0), 10.0)<< "Upper bound is " << std::get<1>(range)(0) << " instead of 10.0!";
        }

        // void VectorDatapoints() 
        // {
        //     Eigen::VectorXd y = (*interpolator)(querry_points);
        //     for (unsigned int i=0; i<y.rows(); i++)
        //     {
        //         EXPECT_NEAR(y(i), labels(i), TEST_TOLERANCE);
        //     }
        // }

        // void ExactPoints() 
        // {
        //     for (unsigned int i=0; i<X.rows(); i++)
        //     {
        //         EXPECT_DOUBLE_EQ(interpolator(X(i,0)), X(i,1));
        //     }
        // }

        // void Extrapolation() 
        // {
        //     // New out of range y vector
        //     Eigen::VectorXd querry_points_extra = 15.0*Eigen::VectorXd::Random(50);
        //     ASSERT_ANY_THROW(interpolator(querry_points_extra)) << "Extrapolation should not be permitted!";

        //     querry_points_extra = -10.1*Eigen::VectorXd::Random(50);
        //     ASSERT_ANY_THROW(interpolator(querry_points_extra)) << "Extrapolation should not be permitted!";
        // }

        // void Multidimensional() 
        // {
        //     // Check for an excpetion if passing multidimensional datapoints
        //     ASSERT_ANY_THROW(interpolator(X)) << "Multidimensional interpolation should not throw an exception!";
        // }

};

