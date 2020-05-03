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

#pragma once

#include <string>
#include <vector>

#include "tt/service_visitor.hpp"

namespace tt {

class Bundle;
class Longrun;
class Oneshot;

class DependencyReader : public ServiceVisitor {
public:
    std::vector<std::string> dependencies() { return dependencies_; }

    void Visit(Bundle *bundle);
    void Visit(Longrun *longrun);
    void Visit(Oneshot *oneshot);

private:
    std::vector<std::string> dependencies_;
};

} // namespace tt