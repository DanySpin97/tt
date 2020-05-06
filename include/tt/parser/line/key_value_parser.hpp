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

#ifndef TT_KEY_VALUE_PARSER_HPP_
#define TT_KEY_VALUE_PARSER_HPP_

#include <string>

namespace tt {

class KeyValueParser {
public:
    std::string key() const noexcept { return key_; }

    std::string value() const noexcept { return value_; }

    bool IsLineValid() const noexcept { return valid_; }

    KeyValueParser(std::string line, bool throw_on_error = false);

private:
    void ParseLine();
    bool TryParseLine();

    std::string line_;
    std::string key_;
    std::string value_;
    bool valid_;
    bool throw_on_error_;
};

} // namespace tt

#endif // TT_KEY_VALUE_PARSER_HPP_
