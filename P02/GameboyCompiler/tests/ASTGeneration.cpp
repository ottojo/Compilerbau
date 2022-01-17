/**
 * Some code snippets that should compile without error
 */

#include <gtest/gtest.h>
#include "GameboyLanguageLexer.h"
#include "GameboyLanguageParser.h"
#include <gbc/ASTGenerationVisitor.hpp>
#include <gbc/CompilerError.hpp>

struct CTp {
    std::string name;
    std::string sourceCode;
    bool shouldCompile;
};

class CompileTest : public ::testing::TestWithParam<CTp> {
    protected:
        void SetUp() override {
            sourceStream.load(GetParam().sourceCode);
        }

        antlr4::ANTLRInputStream sourceStream;
};

std::string compileTestName(const testing::TestParamInfo<CTp> &info) {
    return info.param.name;
}

TEST_P(CompileTest, ASTGeneration) {
    gbparser::GameboyLanguageLexer lexer(&sourceStream);
    antlr4::CommonTokenStream tokenStream(&lexer);
    gbparser::GameboyLanguageParser parser(&tokenStream);
    auto programParseTree = parser.program();
    if (GetParam().shouldCompile) {
        EXPECT_EQ(lexer.getNumberOfSyntaxErrors(), 0);
        EXPECT_EQ(parser.getNumberOfSyntaxErrors(), 0);
    } else {
        EXPECT_GT(lexer.getNumberOfSyntaxErrors() + parser.getNumberOfSyntaxErrors(), 0);
    }

    ASTGenerationVisitor visitor;
    if (GetParam().shouldCompile) {
        EXPECT_NO_THROW(AST ast = visitor.generateAST(programParseTree));
    } else {
        EXPECT_THROW(AST ast = visitor.generateAST(programParseTree), CompilerError);
    }
}

TEST_P(CompileTest, CodeGen) {

}

INSTANTIATE_TEST_SUITE_P(FirstSet,
                         CompileTest,
                         testing::Values(
                                 CTp{"variable_initialization", "int a = 3;", true},
                                 CTp{"parser_fail", "int a = ;", false}
                         ),
                         compileTestName);
