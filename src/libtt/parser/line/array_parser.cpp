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

#include "libtt/parser/line/array_parser.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>

#include "libtt/parser/utils.hpp"

using std::string;
using tt::ArrayParser;

bool ArrayParser::StartParsing(const string line) {
    assert(!IsParsing());

    auto equal_token_pos = line.find('=');
#include <iostream>
    if (equal_token_pos == string::npos) {
        return false;
    }
    auto parenthesis_pos = line.find('(', equal_token_pos + 1);
    if (parenthesis_pos == string::npos) {
        return false;
    }

    auto tmp =
        line.substr(equal_token_pos + 1, parenthesis_pos - equal_token_pos - 1);
    trim(tmp);
    if (tmp.size() != 0) {
        return false;
    }

    key_ = line.substr(0, equal_token_pos);
    trim(key_);
    if (key_.size() == 0) {
        // Empty keys are not allowed
        throw std::exception();
    }

    is_parsing_ = true;

    UpdateStatus(line.substr(parenthesis_pos + 1, string::npos));

    return true;
}

void ArrayParser::ParseLine(const string line) {
    assert(IsParsing());
    UpdateStatus(line);
}

void ArrayParser::UpdateStatus(const string line) {
    auto trimmed_line = trim_copy(line);
    if (line.size() == 0) {
        return;
    }
    auto ending_token_pos = trimmed_line.find(')');
    if (ending_token_pos != string::npos) {
        is_parsing_ = false;
        // There shall be on character after the ending token ')'
        if (ending_token_pos + 1 != trimmed_line.size()) {
            throw std::exception();
        }
    }
    AddValuesFromLine(trimmed_line.substr(0, ending_token_pos));

    if (!IsParsing() && values_.size() == 0) {
        // Empty array
        throw std::exception();
    }
}

void ArrayParser::AddValuesFromLine(const string line) {
    std::istringstream iss(line);
    auto old_end = std::end(values_);
    values_.insert(old_end, std::istream_iterator<string>{iss},
                   std::istream_iterator<string>());
}