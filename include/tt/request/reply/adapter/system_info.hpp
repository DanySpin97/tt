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

#include <nlohmann/json.hpp> // for json, adl_serializer

#include "tt/request/reply/system_info.hpp" // for SystemInfo

#include "spdlog/spdlog.h"

namespace nlohmann {
template <> struct adl_serializer<tt::request::reply::SystemInfo> {
    static auto from_json(const json &j) -> tt::request::reply::SystemInfo {
        return tt::request::reply::SystemInfo{
            j.at("encoded_services").get<std::vector<std::string>>()};
    }

    // Here's the catch! You must provide a to_json method! Otherwise you
    // will not be able to convert move_only_type to json, since you fully
    // specialized adl_serializer on that type
    static void to_json(json &j,
                        const tt::request::reply::SystemInfo &system_info) {
        j["encoded_services"] = system_info.data();
    }
};
} // namespace nlohmann
