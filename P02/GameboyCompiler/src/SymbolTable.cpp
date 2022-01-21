/**
 * @file SymbolTable.cpp
 * @author ottojo
 * @date 1/9/22
 */

#include <gbc/SymbolTable.hpp>

bool SymbolTable::enterVariableDeclaration(const std::string &id, const std::shared_ptr<VariableDeclaration> &decl) {
    if (stacks[id].size() > currNl) {
        // Name already existent at current nesting level
        return false;
    }
    stacks[id]; // Ensure stack for ID exists
    // Store iterator to list of current NL in new element
    auto verticalEntry = verticalConnections.find(currNl);
    auto nextStack = stacks.end();
    if (verticalEntry != verticalConnections.end()) {
        nextStack = verticalEntry->second;
        assert(not nextStack->second.empty());
    }


    DE de{.name=id, .decl=decl, .nextStackAtSameNL = nextStack};
    stacks[id].push(de);
    // Append this element to list of current NL
    verticalConnections[currNl] = stacks.find(id);
    return true;
}

std::shared_ptr<VariableDeclaration> SymbolTable::lookupVariable(const std::string &id) const {
    auto r = stacks.find(id);
    if (r == stacks.end()) {
        return nullptr;
    }
    return r->second.top().decl;
}

void SymbolTable::enterScope() {
    currNl++;
}

std::vector<DE> SymbolTable::leaveScope() {
    if (currNl < 1) {
        throw std::runtime_error{"can not leave global scope!"};
    }
    std::vector<DE> ret;
    auto firstStackWithScopeIt = verticalConnections.find(currNl);
    if (firstStackWithScopeIt != verticalConnections.end()) {
        auto nextStack = firstStackWithScopeIt->second;
        while (nextStack != stacks.end()) {
            auto &stack = nextStack->second;
            assert(not stack.empty());
            auto de = stack.top();
            stack.pop();
            if (stack.empty()) {
                stacks.erase(nextStack);
            }
            nextStack = de.nextStackAtSameNL;
            ret.emplace_back(de);
        }
        verticalConnections.erase(firstStackWithScopeIt);
    }
    currNl--;
    return ret;
}

std::size_t SymbolTable::currentNestingLevel() const {
    return currNl;
}
