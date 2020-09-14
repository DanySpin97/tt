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

#include "tt/parser/service/services_parser.hpp"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iterator>
#include <sstream>
#include <utility>

#include "tt/data/bundle.hpp"
#include "tt/data/longrun.hpp"
#include "tt/data/oneshot.hpp"
#include "tt/dependency_graph/utils.hpp"
#include "tt/parser/service/exception.hpp"
#include "tt/parser/service/instance_service_parser.hpp"
#include "tt/parser/service/service_parser.hpp"

using std::advance;
using std::string;
using std::stringstream;
using std::vector;

using tt::ServiceNotFoundParserException;
using tt::ServiceParser;
using tt::ServicesParser;

ServicesParser::ServicesParser(std::string suffix,
                               std::vector<std::string> paths)
    : suffix_(std::move(suffix)), paths_(std::move(paths)) {}

void ServicesParser::ParseServices(
    const std::vector<std::string> &service_names) {
    for (const auto &name : service_names) {
        ParseService(name);
    }
}

void ServicesParser::ParseService(const std::string &service_name) {
    if (service_map_.count(service_name) != 0U) {
        return;
    }

    // Parse instance service
    if (size_t token_index = GetInstanceTokenIndex(service_name);
        token_index != string::npos) {
        string new_service_name = service_name;
        string instance_name =
            SplitServiceNameFromInstance(new_service_name, token_index);
        auto path = GetPathForServiceName(new_service_name);
        auto service = InstanceServiceParser(path, instance_name).service();
        auto service_name = new_service_name + instance_name;
        service_map_.emplace(service_name, service);
        return;
    }

    auto path = GetPathForServiceName(service_name);
    service_map_.emplace(service_name, ServiceParser(path).service());
    ParseDependenciesOfService(service_map_.at(service_name));
}

auto ServicesParser::GetInstanceTokenIndex(const string &service_name) -> size_t {
    return service_name.find('@');
}

auto ServicesParser::SplitServiceNameFromInstance(string &service_name,
                                                    size_t token_index) -> string {
    auto string_iter = service_name.begin();
    advance(string_iter, token_index);
    string instance{string_iter, service_name.end()};
    service_name = service_name.substr(0, token_index);
    return instance;
}

auto ServicesParser::GetPathForServiceName(const string &name) -> string {
    for (auto i = paths_.rbegin(); i != paths_.rend(); ++i) {
        string service_path = *i + "/" + name + suffix_;
        struct stat buffer {};
        if (stat(service_path.c_str(), &buffer) == 0) {
            return service_path;
        }
    }

    auto msg =
        "Service '" + name + suffix_ + "' could not be found in directories ";
    stringstream joined_paths;
    for (size_t i = 0; i < paths_.size(); ++i) {
        if (i != 0) {
            joined_paths << ",";
        }
        joined_paths << paths_[i];
    }
    msg += joined_paths.str();
    throw ServiceNotFoundParserException(msg);
}

void ServicesParser::ParseDependenciesOfService(const Service &service) {
    tt::ForEachDependencyOfService(service,
                                   [&](auto name) { ParseService(name); });
}

auto tt::ServicesParser::services() -> std::vector<Service> {
    auto services = std::vector<Service>(service_map_.size());
    for (auto &elem : service_map_) {
        services.push_back(elem.second);
    }
    return services;
}
