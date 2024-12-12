#include <gtest/gtest.h>
#include "polynomial_interpolator.hpp"
#include "Eigen/Core"
#include "datagen.hpp"

#define TEST_TOLERANCE 1e-2

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
        Eigen::VectorXd query_points;
        Eigen::VectorXd labels;    
        Eigen::Matrix<double, -1, 2> X;
        PolynomialInterpolator<double>* interpolator;

        void SetUp() override {
            Eigen::VectorXd x = Eigen::VectorXd::LinSpaced(10, -M_PI_2, M_PI_2);

            Eigen::VectorXd y = x.array().cos();
            X = Eigen::MatrixXd(x.rows(), 2);
            X << x, y;


            ASSERT_NO_THROW(interpolator->fit(X, 1)) << "Fitting interpolator failed, cannot test interpolation characteristics!";

            query_points = M_PI_2*Eigen::VectorXd::Random(50);
            labels = query_points.array().cos();
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
            EXPECT_DOUBLE_EQ(std::get<0>(range)(0),  - M_PI_2) << "Lower bound is " << std::get<0>(range)(0) << " instead of -10.0!";
            EXPECT_DOUBLE_EQ(std::get<1>(range)(0), M_PI_2)<< "Upper bound is " << std::get<1>(range)(0) << " instead of 10.0!";
        }

        void SingleDataPoint()
        {
            for (unsigned int i=0; i<query_points.rows(); i++)
            {
                double y = (*interpolator)(query_points(i));
                EXPECT_NEAR(y, labels(i), TEST_TOLERANCE);
            }
        }


        void VectorDatapoints() 
        {
            Eigen::VectorXd y = (*interpolator)(query_points);
            for (unsigned int i=0; i<y.rows(); i++)
            {
                EXPECT_NEAR(y(i), labels(i), TEST_TOLERANCE);
            }
        }

        void Extrapolation() 
        {
            // New out of range y vector
            Eigen::VectorXd query_points_extra = 15.0*Eigen::VectorXd::Random(50);
            ASSERT_ANY_THROW((*interpolator)(query_points_extra)) << "Extrapolation should not be permitted!";

            query_points_extra = -10.1*Eigen::VectorXd::Random(50);
            ASSERT_ANY_THROW((*interpolator)(query_points_extra)) << "Extrapolation should not be permitted!";
        }

        void Multidimensional() 
        {
            // Check for an excpetion if passing multidimensional datapoints
            ASSERT_ANY_THROW((*interpolator)(X)) << "Multidimensional interpolation should not throw an exception!";
        }

        void ExactPoints() 
        {
        for (unsigned int i=0; i<X.rows(); i++)
            {
                EXPECT_DOUBLE_EQ((*interpolator)(X(i,0)), X(i,1));
            }
        }

};

