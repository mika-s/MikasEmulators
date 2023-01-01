#pragma once

#include "crosscutting/misc/session.h"
#include <memory>

namespace emu::applications::zxspectrum_48k {
class PrintableFormat;
}

namespace emu::applications::zxspectrum_48k {

using emu::misc::Session;

class ZxSpectrum48kPrintHeaderSession
    : public Session {
public:
    explicit ZxSpectrum48kPrintHeaderSession(std::shared_ptr<PrintableFormat> format);

    void run() override;

    void pause() override;

    void stop() override;

private:
    std::shared_ptr<PrintableFormat> m_format;
};
}
