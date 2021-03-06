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

#pragma once

// Thanks StackOverflow!
// https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring

#include <algorithm>
#include <cctype>
#include <locale>

namespace tt::utils {

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
                return std::isspace(ch) == 0;
            }));
}
// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](int ch) { return std::isspace(ch) == 0; })
                .base(),
            s.end());
}
// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
static inline auto ltrim_copy(std::string s) -> std::string {
    ltrim(s);
    return s;
}
// trim from end (copying)
static inline auto rtrim_copy(std::string s) -> std::string {
    rtrim(s);
    return s;
}
// trim from both ends (copying)
static inline auto trim_copy(std::string s) -> std::string {
    trim(s);
    return s;
}

} // namespace tt::utils
