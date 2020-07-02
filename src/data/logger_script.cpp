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

#include <utility>

#include "tt/data/logger_script.hpp"

using std::ostream;

using tt::LoggerScript;

LoggerScript::LoggerScript(Type type, std::string &&execute,
                           const std::string &service_to_log,
                           std::string &&user, std::string &&group)
    : Script(type, std::move(execute)), service_to_log_(service_to_log) {
    this->user(user);
    this->group(group);
}

auto LoggerScript::Dump(ostream &oss) const -> ostream & {
    Script::Dump(oss);
    return oss << "\nservice_to_log = " << service_to_log();
}
