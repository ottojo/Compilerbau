/**
 * @file ScopeIndent.cpp
 * @author ottojo
 * @date 1/9/22
 * Description here TODO
 */

#include <gbc/ScopeIndent.hpp>

ScopeIndent::ScopeIndent(AssemblyOutput &out) : out(out){
   out.indent();
}

ScopeIndent::~ScopeIndent() {
    out.unindent();
}
