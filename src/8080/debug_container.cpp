#include "debug_container.h"

namespace emu::cpu8080 {

    DebugContainer::DebugContainer()
            : is_pc_set_(false),
              is_sp_set_(false),
              is_interrupted_set_(false) {
    }

    void DebugContainer::add_register(const std::string &name, const std::function<std::uint8_t()> &value_retriever) {
        registers.emplace_back(name, value_retriever);
    }

    std::vector<std::tuple<std::string, std::function<std::uint8_t()>>> DebugContainer::get_registers() {
        return registers;
    }

    void DebugContainer::add_pc(const std::function<std::uint16_t()> &value_retriever) {
        pc_retriever = value_retriever;
        is_pc_set_ = true;
    }

    std::uint16_t DebugContainer::get_pc() const {
        return pc_retriever();
    }

    bool DebugContainer::is_pc_set() const {
        return is_pc_set_;
    }

    void DebugContainer::add_sp(const std::function<std::uint16_t()> &value_retriever) {
        sp_retriever = value_retriever;
        is_sp_set_ = true;
    }

    std::uint16_t DebugContainer::get_sp() const {
        return sp_retriever();
    }

    bool DebugContainer::is_sp_set() const {
        return is_sp_set_;
    }

    void DebugContainer::add_is_interrupted(const std::function<bool()> &value_retriever) {
        is_interrupted_retriever = value_retriever;
        is_interrupted_set_ = true;
    }

    bool DebugContainer::get_is_interrupted() const {
        return is_interrupted_retriever();
    }

    bool DebugContainer::is_interrupted_set() const {
        return is_interrupted_set_;
    }
}
