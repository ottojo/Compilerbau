/**
 * @file VarAccess.hpp
 * @author ottojo
 * @date 1/21/22
 * Description here TODO
 */

#ifndef GAMEBOYCOMPILER_VARACCESS_HPP
#define GAMEBOYCOMPILER_VARACCESS_HPP

#include <string>
#include <variant>

namespace VarAccess {
    struct FPRelative {
        int offset;
    };
    struct ByGlobalName {
        std::string name;
    };
    using Location = std::variant<FPRelative, ByGlobalName>;
}

#endif //GAMEBOYCOMPILER_VARACCESS_HPP
