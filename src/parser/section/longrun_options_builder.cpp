/*
 * Copyright (c) 2020 Danilo Spinella <oss@danyspin97.org>.
 *
 * This file is part of tt
 * (see https://github.com/danyspin97/tt).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "tt/parser/section/longrun_options_builder.hpp"

#include "tt/data/longrun_options.hpp"     // for LongrunOptions
#include "tt/parser/line/array_parser.hpp" // for ArrayParser
#include "tt/parser/parser_error.hpp"      // for ParserError
#include "tt/parser/section/utils.hpp"     // for AttributeNotFound
#include "tt/utils/parse_boolean.hpp"      // for ParseBoolean

tt::LongrunOptionsBuilder::LongrunOptionsBuilder() noexcept
    : SectionBuilder("options") {}

auto tt::LongrunOptionsBuilder::GetValidAttributes() const noexcept
    -> std::vector<std::string> {
    return {"optional"};
}

auto tt::LongrunOptionsBuilder::GetValidArrayAttributes() const noexcept
    -> std::vector<std::string> {
    return {"dependencies"};
}

auto tt::LongrunOptionsBuilder::EndParsing() noexcept
    -> tl::expected<void, ParserError> {
    if (auto ret = SectionBuilder::EndParsing(); !ret.has_value()) {
        return ret;
    }

    if (auto optional = GetAttribute("optional"); optional.has_value()) {
        auto ret = utils::ParseBoolean(optional.value());
        if (!ret.has_value()) {
            return make_parser_error<void>(ParserError::Type::InvalidBoolean,
                                           ret.error());
        }
        options_.optional(ret.value());
    }

    if (auto dependencies = GetArrayAttribute("dependencies");
        dependencies.has_value()) {
        options_.dependencies(dependencies.value());
    }
    return {};
}

auto tt::LongrunOptionsBuilder::options() -> LongrunOptions && {
    return std::move(options_);
}
