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

#include "tt/request/notify_service_status.hpp" // for NotifyServiceStatus

namespace nlohmann {
template <> struct adl_serializer<tt::request::NotifyServiceStatus> {
    static auto from_json(const json &j) -> tt::request::NotifyServiceStatus {
        auto status = tt::ParseServiceStatusFromString(j.at("status"));
        return {j.at("service"), status};
    }

    // Here's the catch! You must provide a to_json method! Otherwise you
    // will not be able to convert move_only_type to json, since you fully
    // specialized adl_serializer on that type
    static void
    to_json(json &j, const tt::request::NotifyServiceStatus &notify_service) {
        j["service"] = notify_service.service();
        j["status"] = tt::GetServiceStatusName(notify_service.status());
    }
};
} // namespace nlohmann
