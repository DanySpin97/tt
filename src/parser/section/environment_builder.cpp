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

#include "tt/parser/section/environment_builder.hpp"

#include <cctype> // for isalnum, isdigit

#include "tt/data/environment.hpp"             // for Environment
#include "tt/parser/line/exception.hpp"        // for KeyValueParserLineInv...
#include "tt/parser/line/key_value_parser.hpp" // for KeyValueParser
#include "tt/parser/section/exception.hpp"     // for EnvironmentKeyNotVali...
#include "tt/parser/section/utils.hpp"         // for IsEmptyLine

void tt::EnvironmentBuilder::ParseLine(const std::string &line) {
    try {
        TryParseLine(line);
    } catch (const KeyValueParserLineInvalidException &e) {
        const std::string msg = e.msg() + " in [config] section";
        throw SectionBuilderException(msg);
    } catch (const EnvironmentKeyNotValidException &e) {
        const std::string msg = e.msg() + " in [config] section";
        throw SectionBuilderException(msg);
    } catch (const EnvironmentKeyAlreadySetException &e) {
        const std::string msg = e.msg() + " in [config] section";
        throw SectionBuilderException(msg);
    }
}

void tt::EnvironmentBuilder::TryParseLine(const std::string &line) {
    if (IsEmptyLine(line)) {
        return;
    }

    auto key_value_parser = KeyValueParser(line, true);
    const std::string key = key_value_parser.key();
    CheckKeyIsValid(key);
    const std::string value = key_value_parser.value();

    if (!environment_.SetUnique(key, value)) {
        const auto msg = "Key " + key + " has been already set";
        throw tt::EnvironmentKeyAlreadySetException(msg);
    }
}

void tt::EnvironmentBuilder::CheckKeyIsValid(const std::string &key) {
    if (isdigit(key[0]) != 0) {
        const auto msg = "Key " + key + " cannot start with a digit";
        throw EnvironmentKeyNotValidException(msg);
    }

    for (char c : key) {
        if ((isalnum(c) == 0) && c != '_') {
            const auto msg =
                "Character " + std::string{c} + " not valid in key " + key;
            throw EnvironmentKeyNotValidException(msg);
        }
    }
}

auto tt::EnvironmentBuilder::environment() -> Environment && {
    return std::move(environment_);
}
