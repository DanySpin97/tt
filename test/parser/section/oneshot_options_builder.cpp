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

#include "tt/parser/section/oneshot_options_builder.hpp"

#include "catch2/catch.hpp"

#include "tt/data/oneshot_options.hpp"
#include "tt/parser/section/exception.hpp"
#include "tt/parser/section/utils.hpp"

using std::string;
using std::vector;

using tt::OneshotOptions;
using tt::OneshotOptionsBuilder;
using tt::SectionBuilderException;
using tt::TestBuilderWithFile;

TEST_CASE("OneshotOptionsBuilder") {
    auto options = OneshotOptions();
    auto builder = OneshotOptionsBuilder(options);

    SECTION("Parse valid section") {
        TestBuilderWithFile(builder, "../test/data/oneshot_options_section");

        auto expected_deps = vector<string>{"foo", "bar"};
        CHECK(options.optional() == true);
        CHECK(options.write_start_stop_messages() == false);
        CHECK(options.dependencies() == expected_deps);
    }

    SECTION("Parse invalid sections") {
        const auto testFiles =
            vector<string>{"empty_multiline_value",    "invalid",
                           "invalid_multiline_value",  "invalid_quotes",
                           "invalid_boolean",          "unclosed_quotes",
                           "unclosed_multiline_value", "unknown_key",
                           "unknown_multiline_value"};
        for (auto &test : testFiles) {
            CHECK_THROWS_AS(
                TestBuilderWithFile(builder, "../test/data/" + test),
                SectionBuilderException);
        }
    }
}