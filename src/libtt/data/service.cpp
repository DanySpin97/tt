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

#include "libtt/data/service.hpp"

#include <filesystem>

using std::ostream;

using tt::Service;
using tt::ServiceOptions;

Service::Service(const std::string name, const std::string polish_name,
                 const std::string description, const std::string path,
                 ServiceOptions &options)
    : name_(name), polish_name_(polish_name), description_(description),
      path_(path), options_(options) {
    ValidatePath();
}

void Service::ValidatePath() const {
    std::filesystem::path file(path());
    if (file.stem() != name()) {
        // const auto msg = "The attribute \"name\" must match the name of
        // the file";
        throw std::exception();
    }
}

ostream &Service::Dump(ostream &o) const {
    o << "name = " << name();
    if (polish_name().size() != 0) {
        o << "\npolish_name = " << polish_name();
    }
    if (description().size() != 0) {
        o << "\ndescription = " << description();
    }
    return o;
}

ostream &operator<<(ostream &oss, const Service &service) {
    return service.Dump(oss);
}