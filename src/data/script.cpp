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

#include <optional>    // for optional
#include <ostream>     // for operator<<, ostream, basic_ostream
#include <string>      // for operator<<, string, basic_string, char...
#include <type_traits> // for remove_reference<>::type
#include <utility>     // for move

#include "tt/data/script.hpp" // for Script, Script::Type, operator<<
#include "tt/signal.hpp"      // for GetSignalName

tt::Script::Script(Type type, std::string &&execute)
    : execute_(std::move(execute)), type_(type) {}

tt::Script::Script(Type type, std::string &&execute, std::string &&user,
                   std::string &&group)
    : execute_(std::move(execute)), type_(type), user_(std::move(user)),
      group_(std::move(group)) {}

auto tt::Script::operator==(const Script &rhs) const -> bool {
    return execute_ == rhs.execute_ && user_ == rhs.user_ &&
           group_ == rhs.group_ && timeout_ == rhs.timeout_ &&
           timeout_kill_ == rhs.timeout_kill_ &&
           down_signal_ == rhs.down_signal_ && max_death_ == rhs.max_death_;
}

auto tt::Script::Dump(std::ostream &oss) const -> std::ostream & {
    // TODO: Convert from Type to string
    // o << "type = " << type() << "\n";
    oss << "execute = (\n" << execute() << "\n)";
    if (user() && !user().value().empty()) {
        oss << "\nuser = " << user().value();
    }
    if (group() && !group().value().empty()) {
        oss << "\ngroup = " << group().value();
    }
    oss << "\ntimeout = " << timeout();
    oss << "\ntimeout_kill = " << timeout_kill();
    oss << "\ndown_signal = " << GetSignalName(down_signal());
    oss << "\nmax_death = " << max_death();
    return oss;
}

auto operator<<(std::ostream &oss, const tt::Script &script) -> std::ostream & {
    return script.Dump(oss);
}
