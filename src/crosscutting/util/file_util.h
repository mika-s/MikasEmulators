#pragma once

#include "crosscutting/typedefs.h"
#include <sstream>
#include <string>
#include <vector>

namespace emu::util::file {

auto read_file_into_vector(std::string const& path) -> std::vector<u8>;

auto read_file(std::string const& path) -> std::stringstream;
}
