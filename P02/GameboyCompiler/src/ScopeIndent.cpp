/**
 * @file ScopeIndent.cpp
 * @author ottojo
 * @date 1/9/22
dynamic_cast */

#include <gbc/ScopeIndent.hpp>

ScopeIndent::ScopeIndent(AssemblyOutput &out) : out(out){
   out.indent();
}

ScopeIndent::~ScopeIndent() {
    out.unindent();
}
