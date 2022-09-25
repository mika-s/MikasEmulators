#ifndef MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_DEBUG_CONTAINER_H
#define MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_DEBUG_CONTAINER_H

#include <functional>
#include <string>
#include <vector>
#include "crosscutting/typedefs.h"

namespace emu::debugger {

    class RegisterDebugContainer {
    public:
        RegisterDebugContainer(
                std::string name,
                std::function<u8()> value_retriever
        );

        [[nodiscard]] std::string name() const;

        [[nodiscard]] u8 value() const;

    private:
        std::string m_name;
        std::function<u8()> m_value_retriever;
    };

    class FlagRegisterDebugContainer {
    public:
        FlagRegisterDebugContainer();

        FlagRegisterDebugContainer(
                std::string name,
                std::function<u8()> value_retriever,
                std::vector<std::tuple<std::string, int>> flag_names
        );

        [[nodiscard]] std::string name() const;

        [[nodiscard]] u8 value() const;

        [[nodiscard]] std::vector<std::tuple<std::string, int>> flag_names() const;

    private:
        std::string m_name;
        std::function<u8()> m_value_retriever;
        std::vector<std::tuple<std::string, int>> m_flag_names;
    };

    class IoDebugContainer {
    public:
        IoDebugContainer(
                std::string name,
                std::function<bool()> is_active_retriever,
                std::function<u8()> value_retriever
        );

        IoDebugContainer(
                std::string name,
                std::function<bool()> is_active_retriever,
                std::function<u8()> value_retriever,
                std::vector<std::tuple<std::string, int>> bit_name
        );

        [[nodiscard]] std::string name() const;

        [[nodiscard]] bool is_active() const;

        [[nodiscard]] u8 value() const;

        [[nodiscard]] bool is_divided_into_bits() const;

        [[nodiscard]] std::vector<std::tuple<std::string, int>> bit_names() const;

    private:
        std::string m_name;
        std::function<bool()> m_is_active_retriever;
        std::function<u8()> m_value_retriever;
        std::vector<std::tuple<std::string, int>> m_bit_names;
        bool m_is_divided_into_bits;
    };

    class MemoryDebugContainer {
    public:
        MemoryDebugContainer();

        explicit MemoryDebugContainer(
                std::function<std::vector<u8>()> value_retriever
        );

        [[nodiscard]] std::vector<u8> value() const;

    private:
        std::function<std::vector<u8>()> m_value_retriever;
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

        void add_memory(const MemoryDebugContainer& memory);

        [[nodiscard]] MemoryDebugContainer memory() const;

        [[nodiscard]] bool is_memory_set() const;

        void add_pc(const std::function<u16()> &value_retriever);

        [[nodiscard]] u16 pc() const;

        [[nodiscard]] bool is_pc_set() const;

        void add_sp(const std::function<u16()> &value_retriever);

        [[nodiscard]] u16 sp() const;

        [[nodiscard]] bool is_sp_set() const;

        void add_is_interrupted(const std::function<bool()> &value_retriever);

        [[nodiscard]] bool is_interrupted() const;

        [[nodiscard]] bool is_interrupted_set() const;

        void add_interrupt_mode(const std::function<std::string()> &value_retriever);

        [[nodiscard]] std::string interrupt_mode() const;

        [[nodiscard]] bool is_interrupt_mode_set() const;

        void add_disassembled_program(std::vector<std::string> disassembled_program);

        std::vector<std::string> disassembled_program();

        [[nodiscard]] bool is_disassembled_program_set() const;

    private:
        std::vector<RegisterDebugContainer> m_register_retrievers;

        FlagRegisterDebugContainer m_flag_register_retriever;
        bool m_is_flag_register_set;

        std::vector<IoDebugContainer> m_io_retrievers;
        bool m_is_io_set;

        MemoryDebugContainer m_memory_retriever;
        bool m_is_memory_set;

        std::function<u16()> m_pc_retriever;
        bool m_is_pc_set;

        std::function<u16()> m_sp_retriever;
        bool m_is_sp_set;

        std::function<bool()> m_is_interrupted_retriever;
        bool m_is_interrupted_set;

        std::function<std::string()> m_interrupt_mode_retriever;
        bool m_is_interrupt_mode_set;

        std::vector<std::string> m_disassembled_program;
        bool m_is_disassembled_program_set;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_DEBUG_CONTAINER_H
