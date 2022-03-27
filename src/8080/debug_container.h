#ifndef MIKA_EMULATORS_8080_DEBUG_CONTAINER_H
#define MIKA_EMULATORS_8080_DEBUG_CONTAINER_H

#include <functional>
#include <string>
#include <vector>

namespace emu::cpu8080 {

    class DebugContainer {
    public:
        DebugContainer();

        void add_register(const std::string& name, const std::function<std::uint8_t()>& value_retriever);

        std::vector<std::tuple<std::string, std::function<std::uint8_t()>>> get_registers();

        void add_pc(const std::function<std::uint16_t()>& value_retriever);

        [[nodiscard]] std::uint16_t get_pc() const;

        [[nodiscard]] bool is_pc_set() const;

        void add_sp(const std::function<std::uint16_t()>& value_retriever);

        [[nodiscard]] std::uint16_t get_sp() const;

        [[nodiscard]] bool is_sp_set() const;

        void add_is_interrupted(const std::function<bool()>& value_retriever);

        [[nodiscard]] bool get_is_interrupted() const;

        [[nodiscard]] bool is_interrupted_set() const;

    private:
        std::vector<std::tuple<std::string, std::function<std::uint8_t()>>> registers;

        bool is_pc_set_;

        std::function<std::uint16_t()> pc_retriever;

        bool is_sp_set_;

        std::function<std::uint16_t()> sp_retriever;

        bool is_interrupted_set_;

        std::function<bool()> is_interrupted_retriever;
    };
}

#endif //MIKA_EMULATORS_8080_DEBUG_CONTAINER_H
