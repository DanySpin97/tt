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

#include "tt/request/request_factory.hpp"

#include <string> // for string, operator==

#include <nlohmann/json.hpp> // for json, detail::exception

#include "fmt/format.h"    // for format
#include "tl/expected.hpp" // for expected

#include "tt/exception.hpp"                             // for Exception
#include "tt/request/adapter/notify_service_statup.hpp" // IWYU pragma: keep
#include "tt/request/adapter/reload_graph.hpp"          // IWYU pragma: keep
#include "tt/request/adapter/service_info.hpp"          // IWYU pragma: keep
#include "tt/request/adapter/start_services.hpp"        // IWYU pragma: keep
#include "tt/request/adapter/stop_services.hpp"         // IWYU pragma: keep
#include "tt/request/adapter/system_info.hpp"           // IWYU pragma: keep
#include "tt/request/notify_service_status.hpp" // for NotifyServiceStatus
#include "tt/request/reload_graph.hpp"          // for ReloadGraph
#include "tt/request/service_info.hpp"          // for ServiceStatus
#include "tt/request/start_services.hpp"        // for StartServices
#include "tt/request/stop_services.hpp"         // for StopServices
#include "tt/request/system_info.hpp"           // for SystemInfo

using json = nlohmann::json;

auto tt::request::RequestFactory::GetRequestFromBuffer(
    const std::string &buffer)
    -> tl::expected<std::shared_ptr<Request>, std::string> {
    try {
        json j = json::parse(buffer);
        // ComplexRequest:
        //     request_name: "complex_action"
        //     request: { request_object }
        std::string request_name = std::move(j.at("request_name"));

        auto j_req(std::move(j.at("request")));

        if (request_name == NotifyServiceStatus::request_name) {
            return std::make_shared<NotifyServiceStatus>(std::move(j_req));
        }
        if (request_name == ServiceInfo::request_name) {
            return std::make_shared<ServiceInfo>(std::move(j_req));
        }
        if (request_name == StartServices::request_name) {
            return std::make_shared<StartServices>(std::move(j_req));
        }
        if (request_name == StopServices::request_name) {
            return std::make_shared<StopServices>(std::move(j_req));
        }
        if (request_name == SystemInfo::request_name) {
            return std::make_shared<SystemInfo>(std::move(j_req));
        }
        if (request_name == ReloadGraph::request_name) {
            return std::make_shared<ReloadGraph>(std::move(j_req));
        }
        return tl::make_unexpected(request_name +
                                   " is not a valid request name");
    } catch (const nlohmann::detail::exception &e) {
        return tl::make_unexpected(
            fmt::format("'{}', while parsing:\n{}", e.what(), buffer));
    }
}
