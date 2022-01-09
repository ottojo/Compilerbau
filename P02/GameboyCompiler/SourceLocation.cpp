/**
 * @file SourceLocation.cpp
 * @author ottojo
 * @date 1/9/22
 * Description here TODO
 */

#include "SourceLocation.hpp"

SourceLocation::SourceLocation(antlr4::TokenSource *source) :
        file(source->getSourceName()),
        line(source->getLine()),
        column(source->getCharPositionInLine()) {}
