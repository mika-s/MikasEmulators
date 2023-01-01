#include "zxspectrum_48k_print_header_session.h"
#include "applications/zxspectrum_48k/interfaces/printable_format.h"
#include <memory>
#include <utility>

namespace emu::applications::zxspectrum_48k {

ZxSpectrum48kPrintHeaderSession::ZxSpectrum48kPrintHeaderSession(std::shared_ptr<PrintableFormat> format)
    : m_format(std::move(format))
{
}

void ZxSpectrum48kPrintHeaderSession::run()
{
    m_format->print_header();
}

void ZxSpectrum48kPrintHeaderSession::pause()
{
}

void ZxSpectrum48kPrintHeaderSession::stop()
{
}

}
