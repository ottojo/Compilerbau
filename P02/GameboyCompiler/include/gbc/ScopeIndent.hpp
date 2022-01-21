/**
 * @file ScopeIndent.hpp
 * @author ottojo
 * @date 1/9/22
 */

#ifndef GAMEBOYCOMPILER_SCOPEINDENT_HPP
#define GAMEBOYCOMPILER_SCOPEINDENT_HPP

#include <gbc/AssemblyOutput.hpp>

class ScopeIndent {
    public:
        explicit ScopeIndent(AssemblyOutput &out);

        ScopeIndent(const ScopeIndent &rhs) = delete;

        ScopeIndent &operator=(const ScopeIndent &rhs) = delete;

        ScopeIndent(ScopeIndent &&rhs) = delete;

        ScopeIndent &operator=(ScopeIndent &&rhs) = delete;

        ~ScopeIndent();

    private:
        AssemblyOutput &out;
};


#endif //GAMEBOYCOMPILER_SCOPEINDENT_HPP
