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

tt::LoggerScript::LoggerScript(Type type, std::string &&execute,
                               std::string service_to_log, std::string &&user,
                               std::string &&group)
    : LongLivedScript(type, std::move(execute), std::move(user),
                      std::move(group)),
      service_to_log_(std::move(service_to_log)) {}

auto tt::LoggerScript::Dump(std::ostream &oss) const -> std::ostream & {
    LongLivedScript::Dump(oss);
    return oss << "\nservice_to_log = " << service_to_log();
}
