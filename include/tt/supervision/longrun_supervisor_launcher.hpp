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

#include <map>    // for map
#include <string> // for string

#include <tiny-process-library/process.hpp> // for Process

#include "tt/data/service.hpp" // for Service

namespace tt {

class Dirs;

class LongrunSupervisorLauncher {
public:
    explicit LongrunSupervisorLauncher(std::shared_ptr<Dirs> dirs);

    auto Launch(const Longrun &longrun)
        -> std::unique_ptr<TinyProcessLib::Process>;
    static void Close(TinyProcessLib::Process &process);

private:
    std::shared_ptr<Dirs> dirs_;
};

} // namespace tt
