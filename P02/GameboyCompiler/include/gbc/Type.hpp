/**
 * @file Type.hpp
 * @author ottojo
 * @date 1/21/22
 * Description here TODO
 */

#ifndef GAMEBOYCOMPILER_TYPE_HPP
#define GAMEBOYCOMPILER_TYPE_HPP

#include <string>
#include <optional>
#include <gbc/SourceLocation.hpp>
#include <memory>

struct Type {
        std::string name;
        std::size_t size = 0;
        std::optional<SourceLocation> loc = std::nullopt; ///< Location of type definition, may be omitted for non-user-defined types
};

// TODO: Types for optional
using TypePtr = std::shared_ptr<const Type>;
using OptionalTypePtr = std::shared_ptr<const Type>;

#endif //GAMEBOYCOMPILER_TYPE_HPP
