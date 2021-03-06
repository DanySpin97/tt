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

#include "tt/log/cli_logger.hpp"

#include <filesystem> // for operator/, path
#include <iterator>   // for begin, end
#include <unistd.h>   // for isatty, STDOUT_FILENO
#include <vector>     // for vector

#include "spdlog/async.h"                    // for async_factory
#include "spdlog/common.h"                   // for sink_ptr, from_str, off
#include "spdlog/logger.h"                   // for logger
#include "spdlog/sinks/ansicolor_sink.h"     // for ansicolor_stdout_sink
#include "spdlog/sinks/basic_file_sink.h"    // for basic_file_sink_mt, bas...
#include "spdlog/sinks/stdout_color_sinks.h" // for stdout_color_sink_mt
#include "spdlog/spdlog.h"                   // for set_default_logger

#include "tt/path/dirs.hpp" // for Dirs

tt::CliLogger::CliLogger(const std::string &command_name,
                         const std::shared_ptr<Dirs> &dirs,
                         const std::string &verbosity, bool silence_stderr) {
    auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console->set_level(spdlog::level::trace);

    if (silence_stderr) {
        console->set_level(spdlog::level::off);
    }

    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(console);

    auto level = spdlog::level::from_str(verbosity);
    printer_ =
        std::make_shared<spdlog::logger>("cli", begin(sinks), end(sinks));
    printer_->set_level(level);
    printer_->set_pattern("%v");

    if (isatty(STDOUT_FILENO) == 0) {
        std::filesystem::path logdir = dirs->logdir();

        auto cli_file_sink =
            std::make_shared<spdlog::sinks::basic_file_sink_mt>(logdir /
                                                                "cli.log");
        // {DATE TIME.milliseconds} (command_name) [short log level (e.g. I)]
        cli_file_sink->set_pattern("{%D %T.%e} (" + command_name +
                                   ") [%L]\n%v");
        // Set cli_file_sink to the mininum logging level
        cli_file_sink->set_level(spdlog::level::trace);
        sinks.push_back(cli_file_sink);
    }

    logger_ = std::make_shared<spdlog::logger>("cli_logger", begin(sinks),
                                               end(sinks));
    logger_->set_level(level);
    spdlog::set_default_logger(logger_);
}
