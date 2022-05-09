#include <utility>
#include "debug_container.h"

namespace emu::cpu8080 {

    DebugContainer::DebugContainer()
            : m_is_flag_register_set(false),
              m_is_io_set(false),
              m_is_pc_set(false),
              m_is_sp_set(false),
              m_is_interrupted_set(false),
              m_is_disassembled_program_set(false) {
    }

    void DebugContainer::add_register(const std::string &name, const std::function<std::uint8_t()> &value_retriever) {
        m_register_retrievers.emplace_back(name, value_retriever);
    }

    std::vector<std::tuple<std::string, std::function<std::uint8_t()>>> DebugContainer::registers() {
        return m_register_retrievers;
    }

    void DebugContainer::add_flag_register(
            const std::string &name,
            const std::function<std::uint8_t()> &value_retriever,
            const std::vector<std::tuple<std::string, int>> &flag_names
    ) {
        m_flag_register_retriever = std::make_tuple(name, value_retriever);
        m_flag_names = flag_names;
        m_is_flag_register_set = true;
    }

    std::tuple<std::string, std::function<std::uint8_t()>> DebugContainer::flag_register() const {
        return m_flag_register_retriever;
    }

    std::vector<std::tuple<std::string, int>> DebugContainer::flag_names() const {
        return m_flag_names;
    }

    bool DebugContainer::is_flag_register_set() const {
        return m_is_flag_register_set;
    }

    void DebugContainer::add_io(
            const std::string &name,
            const std::function<bool()> &is_active_retriever,
            const std::function<std::uint8_t()> &value_retriever
    ) {
        m_io_retrievers.emplace_back(std::make_tuple(name, is_active_retriever, value_retriever));
        m_is_io_set = true;
    }

    std::vector<std::tuple<std::string, std::function<bool()>, std::function<std::uint8_t()>>> DebugContainer::io() const {
        return m_io_retrievers;
    }

    bool DebugContainer::is_io_set() const {
        return m_is_io_set;
    }

    void DebugContainer::add_pc(const std::function<std::uint16_t()> &value_retriever) {
        m_pc_retriever = value_retriever;
        m_is_pc_set = true;
    }

    std::uint16_t DebugContainer::pc() const {
        return m_pc_retriever();
    }

    bool DebugContainer::is_pc_set() const {
        return m_is_pc_set;
    }

    void DebugContainer::add_sp(const std::function<std::uint16_t()> &value_retriever) {
        m_sp_retriever = value_retriever;
        m_is_sp_set = true;
    }

    std::uint16_t DebugContainer::sp() const {
        return m_sp_retriever();
    }

    bool DebugContainer::is_sp_set() const {
        return m_is_sp_set;
    }

    void DebugContainer::add_is_interrupted(const std::function<bool()> &value_retriever) {
        m_is_interrupted_retriever = value_retriever;
        m_is_interrupted_set = true;
    }

    bool DebugContainer::is_interrupted() const {
        return m_is_interrupted_retriever();
    }

    bool DebugContainer::is_interrupted_set() const {
        return m_is_interrupted_set;
    }

    void DebugContainer::add_disassembled_program(std::vector<std::string> disassembled_program) {
        m_disassembled_program = std::move(disassembled_program);
        m_is_disassembled_program_set = true;
    }

    std::vector<std::string> DebugContainer::disassembled_program() {
        return m_disassembled_program;
    }

    bool DebugContainer::is_disassembled_program_set() const {
        return m_is_disassembled_program_set;
    }
}
