#include <gtest/gtest.h>
#include "data_reader.hpp"

class DataReaderTest: public ::testing::Test {
    protected:
        DataReader<double>* reader;
        std::filesystem::path data_folder;

        void SetUp() override {
            reader = new DataReader<double>();
            data_folder = std::filesystem::path("../test/data/");
        }

        void TearDown() override {
            delete reader;
        }

        void NoHashTagDelimiter()
        {
            EXPECT_ANY_THROW(reader->read(data_folder.concat("missing_hashtag.txt")));
        }

        void NonNumericDatapointNumber()
        {
            EXPECT_ANY_THROW(reader->read(data_folder.concat("non_numeric_N.txt")));
        }

        void NonNumericDatapointDimension()
        {
            EXPECT_ANY_THROW(reader->read(data_folder.concat("non_numeric_M.txt")));
        }

        void NonNumericDatapoint()
        {
            EXPECT_ANY_THROW(reader->read(data_folder.concat("non_numeric_datapoint.txt")));
        }

        void NonCoherentDatapointCount()
        {
            EXPECT_ANY_THROW(reader->read(data_folder.concat("invalid_number_datapoints.txt")));
        }

        void ExpectedFormat()
        {
            EXPECT_NO_THROW(reader->read(data_folder.concat("valid_datafile.txt")));
        }
};

TEST_F(DataReaderTest, NonNumericDatapointNumber) { this->NonNumericDatapointNumber(); }
TEST_F(DataReaderTest, NonNumericDatapointDimension) { this->NonNumericDatapointDimension(); }
TEST_F(DataReaderTest, NonNumericDatapoint) { this->NonNumericDatapoint(); }
TEST_F(DataReaderTest, NonCoherentDatapointCount) { this->NonCoherentDatapointCount(); }
TEST_F(DataReaderTest, ExpectedFormat) { this->ExpectedFormat(); }
TEST_F(DataReaderTest, NoHashTagDelimiter) { this->NoHashTagDelimiter(); }

