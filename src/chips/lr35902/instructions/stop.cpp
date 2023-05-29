#include "crosscutting/memory/next_byte.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "doctest.h"
#include <iostream>

namespace emu::lr35902 {

using emu::memory::NextByte;
using emu::util::byte::is_bit_set;

/**
 * Stop processor and screen until button press
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 4</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * NOTE: Number of cycles spent is unknown.
 *
 * @param cycles is the number of cycles variable, which will be mutated
 */
void stop_0(cyc& cycles)
{
    cycles = 4;
}

void print_stop(std::ostream& ostream)
{
    ostream << "STOP 0";
}
}
