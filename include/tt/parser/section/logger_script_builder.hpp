/*
 * Copyright (c) 2020 Danilo Spinella <danyspin97@protonmail.com>.
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

#ifndef TT_LOGGER_SCRIPT_BUILDER_HPP_
#define TT_LOGGER_SCRIPT_BUILDER_HPP_

#include <string>

#include "tt/data/logger_script.hpp"
#include "tt/parser/section/long_lived_script_builder.hpp"

namespace tt {

class LoggerScriptBuilder : public LongLivedScriptBuilder {
public:
    explicit LoggerScriptBuilder(std::string service_name);
    auto logger_script() -> LoggerScript;

    void EndParsing() override;

protected:
    auto GetAttributeForKey(const std::string &key) -> std::string & override;

private:
    void CheckParsedValues();

    void SetDefaultForOptionalValues() noexcept;

    auto GetDefaultExecute() const noexcept -> std::string;

    auto GetDefaultDestination() const noexcept -> std::string;

    std::string destination_;
    std::string maxsize_;
    std::string service_name_;
};

} // namespace tt

#endif // TT_LOGGER_SCRIPT_BUILDER_HPP_
