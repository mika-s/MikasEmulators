#pragma once

#include "crosscutting/typedefs.h"
#include <sstream>
#include <string>
#include <vector>

namespace emu::util::file {

std::vector<u8> read_file_into_vector(std::string const& path);

std::stringstream read_file(std::string const& path);
}
