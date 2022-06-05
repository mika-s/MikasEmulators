#ifndef MIKA_EMULATORS_8080_DEBUG_CONTAINER_H
#define MIKA_EMULATORS_8080_DEBUG_CONTAINER_H

#include <functional>
#include <string>
#include <vector>

namespace emu::cpu8080 {

    class RegisterDebugContainer {
    public:
        RegisterDebugContainer(
                std::string name,
                std::function<std::uint8_t()> value_retriever
        );

        [[nodiscard]] std::string name() const;

        [[nodiscard]] std::uint8_t value() const;

    private:
        std::string m_name;
        std::function<std::uint8_t()> m_value_retriever;
    };

    class FlagRegisterDebugContainer {
    public:
        FlagRegisterDebugContainer();

        FlagRegisterDebugContainer(
                std::string name,
                std::function<std::uint8_t()> value_retriever,
                std::vector<std::tuple<std::string, int>> flag_names
        );

        [[nodiscard]] std::string name() const;

        [[nodiscard]] std::uint8_t value() const;

        [[nodiscard]] std::vector<std::tuple<std::string, int>> flag_names() const;

    private:
        std::string m_name;
        std::function<std::uint8_t()> m_value_retriever;
        std::vector<std::tuple<std::string, int>> m_flag_names;
    };

    class IoDebugContainer {
    public:
        IoDebugContainer(
                std::string name,
                std::function<bool()> is_active_retriever,
                std::function<std::uint8_t()> value_retriever
        );

        IoDebugContainer(
                std::string name,
                std::function<bool()> is_active_retriever,
                std::function<std::uint8_t()> value_retriever,
                std::vector<std::tuple<std::string, int>> bit_name
        );

        [[nodiscard]] std::string name() const;

        [[nodiscard]] bool is_active() const;

        [[nodiscard]] std::uint8_t value() const;

    private:
        std::string m_name;
        std::function<bool()> m_is_active_retriever;
        std::function<std::uint8_t()> m_value_retriever;
        std::vector<std::tuple<std::string, int>> m_bit_name;
        bool m_is_divided_into_bits;
    };

    class DebugContainer {
    public:
        DebugContainer();

        void add_register(const RegisterDebugContainer &reg);

        std::vector<RegisterDebugContainer> registers();

        void add_flag_register(const FlagRegisterDebugContainer &flag_reg);

        [[nodiscard]] FlagRegisterDebugContainer flag_register() const;

        [[nodiscard]] bool is_flag_register_set() const;

        void add_io(const IoDebugContainer& io);

        [[nodiscard]] std::vector<IoDebugContainer> io() const;

        [[nodiscard]] bool is_io_set() const;

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
        std::vector<RegisterDebugContainer> m_register_retrievers;

        FlagRegisterDebugContainer m_flag_register_retriever;
        bool m_is_flag_register_set;

        std::vector<IoDebugContainer> m_io_retrievers;
        bool m_is_io_set;

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
