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

#include "tt/data/bundle_options.hpp"

#include <algorithm> // for copy
#include <iterator>  // for ostream_iterator
#include <ostream>   // for operator<<, ostream, basic_ostream

auto tt::BundleOptions::operator==(const BundleOptions &rhs) const -> bool {
    return contents_ == rhs.contents_;
}

auto tt::BundleOptions::Dump(std::ostream &oss) const -> std::ostream & {
    oss << "contents = ( ";
    copy(contents_.begin(), contents_.end(),
         std::ostream_iterator<std::string>(oss, " "));
    return oss << " )";
}
