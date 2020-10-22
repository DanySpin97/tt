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

#include "tt/supervision/script_supervisor.hpp"

#include "catch2/catch.hpp"

#include <unistd.h>

#include <chrono>
#include <filesystem>
#include <thread>

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace tt::test {

void TestOutput(bool is_stdout) {
    std::string type = is_stdout ? "stdout" : "stderr";
    std::string line{"Write line to " + type};
    std::string command = std::string{is_stdout ? "" : ">&2 "} + "echo " + line;
    tt::Script script{tt::Script::Type::SH, std::move(command)};
    std::string test_name = "test-write-" + type;
    const auto logfile = std::string{test_name} + ".log";
    if (std::filesystem::exists(logfile)) {
        std::filesystem::remove(logfile);
    }
    auto logger =
        spdlog::basic_logger_mt<spdlog::async_factory>(test_name, logfile);
    logger->set_pattern("%v");
    tt::Environment env;
    tt::ScriptSupervisor spawn_script(test_name, script, env,
                                 tt::ScriptLogger(logger));
    tt::ScriptStatus status = spawn_script.Execute();
    REQUIRE(status == tt::ScriptStatus::Success);
    REQUIRE(std::filesystem::exists(logfile));

    logger->flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::ifstream ifs(logfile);
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    CHECK(content == "[" + type + "] " + line + "\n");
}

} // namespace tt::test

auto console = spdlog::stdout_color_mt("spawn_script_test");
tt::ScriptLogger logger{console};

TEST_CASE("ScriptSupervisor") {
    tt::Environment env;

    SECTION("Execute sucessfull script") {
        tt::Script script{tt::Script::Type::SH, "true"};
        tt::ScriptSupervisor spawn_script("test-path-sleep", script, env, logger);
        tt::ScriptStatus status = spawn_script.Execute();
        CHECK(status == tt::ScriptStatus::Success);
    }

    SECTION("Execute failing script") {
        tt::Script script{tt::Script::Type::SH, "false"};
        tt::ScriptSupervisor spawn_script("test-path-sleep", script, env, logger);
        tt::ScriptStatus status = spawn_script.Execute();
        CHECK(status == tt::ScriptStatus::Failure);
    }

    SECTION("Execute script within timeout") {
        tt::Script script{tt::Script::Type::SH, "sleep 1"};
        script.timeout(2000);
        tt::ScriptSupervisor spawn_script("test-path-sleep", script, env, logger);
        tt::ScriptStatus status = spawn_script.Execute();
        CHECK(status == tt::ScriptStatus::Success);
    }

    SECTION("Execute script that exceed timeout") {
        tt::Script script{tt::Script::Type::SH, "sleep 10"};
        script.timeout(50);
        tt::ScriptSupervisor spawn_script("test-path-sleep", script, env, logger);
        tt::ScriptStatus status = spawn_script.Execute();
        CHECK(status == tt::ScriptStatus::Failure);
    }

    SECTION("Execute script within timeout and fail") {
        tt::Script script{tt::Script::Type::SH, "sleep 1 && false"};
        script.timeout(2000);

        tt::ScriptSupervisor spawn_script("test-path-sleep", script, env, logger);
        tt::ScriptStatus status = spawn_script.Execute();
        CHECK(status == tt::ScriptStatus::Failure);
    }

    SECTION("Check time tried") {
        // Cleanup previous test
        std::filesystem::path testfile{"TIME_TRIED"};
        if (std::filesystem::exists(testfile)) {
            std::filesystem::remove(testfile);
        }

        constexpr auto time_to_try = 3;
        const auto *command = "echo 1 >> TIME_TRIED ; exit 1";
        tt::Script script{tt::Script::Type::SH, command};
        script.max_death(time_to_try);

        tt::ScriptSupervisor spawn_script("test-path-sleep", script, env, logger);

        tt::ScriptStatus status = spawn_script.Execute();
        CHECK(status == tt::ScriptStatus::Failure);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        REQUIRE(std::filesystem::exists(testfile));
        const auto size = std::filesystem::file_size(testfile);
        CHECK(size == time_to_try * 2);
    }

    SECTION("Check process is killed") {
        // Cleanup previous test
        std::filesystem::path testfile{"PROCESS_EXISTS"};
        if (std::filesystem::exists(testfile)) {
            std::filesystem::remove(testfile);
        }

        const auto *command = "sleep 1 && touch PROCESS_EXISTS";
        tt::Script script{tt::Script::Type::SH, command};
        script.timeout(50);
        script.max_death(1);

        tt::ScriptSupervisor spawn_script("test-path-sleep", script, env, logger);
        tt::ScriptStatus status = spawn_script.Execute();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        CHECK(status == tt::ScriptStatus::Failure);
        CHECK_FALSE(std::filesystem::exists(testfile));
    }

    SECTION("Execute sucessfull path") {
        tt::Script script{tt::Script::Type::Path, "sleep 0"};
        tt::ScriptSupervisor spawn_script("test-path-sleep", script, env, logger);
        tt::ScriptStatus status = spawn_script.Execute();
        CHECK(status == tt::ScriptStatus::Success);
    }

    SECTION("Execute sucessfull sleep as path") {
        tt::Script script{tt::Script::Type::Path, "sleep 2"};
        script.timeout(5000);
        tt::ScriptSupervisor spawn_script("test-path-sleep", script, env, logger);
        tt::ScriptStatus status = spawn_script.Execute();
        CHECK(status == tt::ScriptStatus::Success);
    }

    SECTION("Write to stdout") { tt::test::TestOutput(true); }

    SECTION("Write to stderr") { tt::test::TestOutput(false); }
}
