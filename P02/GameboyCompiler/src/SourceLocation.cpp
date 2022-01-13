/**
 * @file SourceLocation.cpp
 * @author ottojo
 * @date 1/9/22
 * Description here TODO
 */

#include <gbc/SourceLocation.hpp>

#include <TokenSource.h>

SourceLocation::SourceLocation(antlr4::Token *token) :
        file(token->getTokenSource()->getSourceName()),
        line(token->getLine()),
        column(token->getCharPositionInLine()) {}
