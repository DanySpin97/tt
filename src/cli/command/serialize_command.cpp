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

#include "tt/cli/command/serialize_command.hpp"

#include <filesystem>
#include <fstream>
#include <utility>

#include <sys/stat.h>

#include "args.hxx"

#include "spdlog/spdlog.h"

#include "tt/data/service.hpp"
#include "tt/log/cli_logger.hpp"      // for CliLogger
#include "tt/parser/parser_error.hpp" // for ParserError
#include "tt/parser/service/service_parser.hpp"
#include "tt/path/dirs.hpp"
#include "tt/utils/serialize.hpp"

#include "tt/cli/global_options.hpp"

tt::cli::SerializeCommand::SerializeCommand(args::Subparser &parser)
    : Command(parser),
      service_(parser, "service", "path of the service to serialize"),
      filename_(parser, "filename", "filename to serialize the service into") {}

auto tt::cli::SerializeCommand::Execute() -> int {
    int ret = ParseLongrunFromFile();
    if (ret != 0) {
        return ret;
    }

    try {
        utils::Serialize(*longrun_, filename_.Get());
    } catch (const Exception & /*e*/) {
        return 255;
    }
    return 0;
}

auto tt::cli::SerializeCommand::ParseLongrunFromFile() -> int {
    auto service_name = args::get(service_);
    if (!std::filesystem::exists(service_name)) {
        spdlog::error("Service {} does not exist", service_name);
    }

    auto parser = ServiceParser();
    auto parsed_service = parser.ParseService(service_name);
    if (!parsed_service.has_value()) {
        logger()->LogError("{}", parsed_service.error().msg());
        return 255;
    }
    auto service = std::move(parsed_service.value());
    if (service.index() != 2) {
        logger()->LogError("Service {} is not of type longrun", service_name);
        return 255;
    }

    longrun_ = std::make_unique<Longrun>(std::get<Longrun>(service));
    return 0;
}
