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

#include "tt/net/client.hpp"

#include <cstring> // for memcpy
#include <utility> // for move

#include "tl/expected.hpp" // for expected

#include "nngpp/protocol/req0.h" // for open
#include "nngpp/socket_view.h"   // for socket_view

#include "tt/exception.hpp" // for Exception

tt::net::Client::Client(std::filesystem::path socket_path)
    : Socket(Socket::Mode::Client, std::move(socket_path)) {}

auto tt::net::Client::Connect() -> tl::expected<void, std::string> {
    return Socket::Connect();
}

auto tt::net::Client::IsConnected() const noexcept -> bool {
    return Socket::IsConnected();
}
