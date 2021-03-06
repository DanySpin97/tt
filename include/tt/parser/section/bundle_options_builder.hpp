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

#ifndef TT_BUNDLE_OPTION_BUILDER_HPP_
#define TT_BUNDLE_OPTION_BUILDER_HPP_

#include <string> // for string

#include "tt/data/bundle_options.hpp"            // for BundleOptions
#include "tt/parser/section/section_builder.hpp" // for SectionBuilder

namespace tt {

class BundleOptionsBuilder : public SectionBuilder {
public:
    BundleOptionsBuilder() noexcept;

    [[nodiscard]] auto options() noexcept -> BundleOptions &&;

    [[nodiscard]] auto EndParsing() noexcept
        -> tl::expected<void, ParserError> override;

protected:
    [[nodiscard]] auto GetValidAttributes() const noexcept
        -> std::vector<std::string> override {
        return {};
    }
    [[nodiscard]] auto GetValidArrayAttributes() const noexcept
        -> std::vector<std::string> override;

private:
    BundleOptions options_;
};

} // namespace tt

#endif // TT_BUNDLE_OPTION_BUILDER_HPP_
