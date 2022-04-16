#ifndef MIKA_EMULATORS_8080_DEBUG_CONTAINER_H
#define MIKA_EMULATORS_8080_DEBUG_CONTAINER_H

#include <functional>
#include <string>
#include <vector>

namespace emu::cpu8080 {

    class DebugContainer {
    public:
        DebugContainer();

        void add_register(const std::string &name, const std::function<std::uint8_t()> &value_retriever);

        std::vector<std::tuple<std::string, std::function<std::uint8_t()>>> registers();

        void add_flag_register(
                const std::string &name,
                const std::function<std::uint8_t()> &value_retriever,
                const std::vector<std::tuple<std::string, int>> &flag_names
        );

        [[nodiscard]] std::tuple<std::string, std::function<std::uint8_t()>> flag_register() const;

        [[nodiscard]] std::vector<std::tuple<std::string, int>> flag_names() const;

        [[nodiscard]] bool is_flag_register_set() const;

        void add_pc(const std::function<std::uint16_t()> &value_retriever);

        [[nodiscard]] std::uint16_t pc() const;

        [[nodiscard]] bool is_pc_set() const;

        void add_sp(const std::function<std::uint16_t()> &value_retriever);

        [[nodiscard]] std::uint16_t sp() const;

        [[nodiscard]] bool is_sp_set() const;

        void add_is_interrupted(const std::function<bool()> &value_retriever);

        [[nodiscard]] bool is_interrupted() const;

        [[nodiscard]] bool is_interrupted_set() const;

        void add_disassembled_program(std::vector<std::string> disassembled_program);

        std::vector<std::string> disassembled_program();

        [[nodiscard]] bool is_disassembled_program_set() const;

    private:
        std::vector<std::tuple<std::string, std::function<std::uint8_t()>>> m_register_retrievers;

        std::tuple<std::string, std::function<std::uint8_t()>> m_flag_register_retriever;
        std::vector<std::tuple<std::string, int>> m_flag_names;
        bool m_is_flag_register_set;

        std::function<std::uint16_t()> m_pc_retriever;
        bool m_is_pc_set;

        std::function<std::uint16_t()> m_sp_retriever;
        bool m_is_sp_set;

        std::function<bool()> m_is_interrupted_retriever;
        bool m_is_interrupted_set;

        std::vector<std::string> m_disassembled_program;
        bool m_is_disassembled_program_set;
    };
}

#endif //MIKA_EMULATORS_8080_DEBUG_CONTAINER_H
