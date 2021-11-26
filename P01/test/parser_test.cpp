#include <gtest/gtest.h>
#include "CalculatorParser.hpp"

struct TestCase {
    std::string name;
    std::string input;
    std::optional<double> result;
};

std::ostream &operator<<(std::ostream &os, const TestCase &dt) {
    if (dt.result.has_value()) {
        os << dt.input << " = " << dt.result.value();
    } else {
        os << dt.input << " = ERROR";
    }
    return os;
}

class ParseTest : public testing::TestWithParam<TestCase> {

};

std::string name(const testing::TestParamInfo<TestCase> &tc) {
    return tc.param.name;
}


TEST_P(ParseTest, Success_Fail) {
    CalculatorParser p(GetParam().input);
    EXPECT_EQ(p.parse().has_value(), GetParam().result.has_value());
}

TEST_P(ParseTest, CorrectResult) {
    CalculatorParser p(GetParam().input);
    auto res = p.parse();
    EXPECT_EQ(res.has_value(), GetParam().result.has_value());
    if (res.has_value() and GetParam().result.has_value()) {
        EXPECT_DOUBLE_EQ(res.value(), GetParam().result.value());
    }
}

INSTANTIATE_TEST_SUITE_P(AssignmentTests,
                         ParseTest,
                         testing::Values(
                                 TestCase{.name="a", .input = "2 + 5 / 3 * 2", .result=2.0 + 5.0 / 3 * 2},
                                 TestCase{.name="b", .input = "(2 + 5) / 3", .result=(2.0 + 5) / 3},
                                 TestCase{.name="c", .input = "((10 - 213) * 25) + 27 ", .result=-5048},
                                 TestCase{.name="d", .input = "(7 - 3) * 4^3", .result=256},
                                 TestCase{.name="e", .input = "((1 + 1) * 1 - 2", .result=std::nullopt},
                                 TestCase{.name="f", .input = "a + 1 * 3", .result=std::nullopt}
                         ),
                         name);

INSTANTIATE_TEST_SUITE_P(ExponentialTests,
                         ParseTest,
                         testing::Values(
                                 TestCase{.name="multiple_failure", .input="1^2^3^4", .result=std::nullopt},
                                 TestCase{.name="parentheses", .input="2^(1+2)", .result=8},
                                 TestCase{.name="multiple_parens", .input="2^(2^2)", .result=16}
                         ),
                         name);

INSTANTIATE_TEST_SUITE_P(NegativeLiterals,
                         ParseTest,
                         testing::Values(
                                 TestCase{.name="single_literal", .input="-7", .result=-7},
                                 TestCase{.name="add_negative", .input="3+(-2)", .result=1},
                                 TestCase{.name="negative_exponent", .input="2^-4", .result=0.0625}
                         ),
                         name);
