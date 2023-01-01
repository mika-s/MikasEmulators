#pragma once

namespace emu::applications::zxspectrum_48k {

class PrintableFormat {
public:
    virtual ~PrintableFormat() = default;

    virtual void print_header() = 0;
};
}
