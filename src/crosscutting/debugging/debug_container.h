#pragma once

#include "crosscutting/audio/waveform.h"
#include "crosscutting/typedefs.h"
#include "disassembled_line.h"
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace emu::gui {
class Sprite;
}
namespace emu::gui {
class Tile;
}

namespace emu::debugger {

using emu::gui::Sprite;
using emu::gui::Tile;
using emu::wsg3::Waveform;

template<class D>
class RegisterDebugContainer {
public:
    RegisterDebugContainer(
        std::string name,
        std::function<D()> value_retriever_main)
        : m_name(std::move(name))
        , m_value_retriever_main(std::move(value_retriever_main))
        , m_is_alternate_set(false)
    {
    }

    RegisterDebugContainer(
        std::string name,
        std::function<D()> value_retriever_main,
        std::function<D()> value_retriever_alternate)
        : m_name(std::move(name))
        , m_value_retriever_main(std::move(value_retriever_main))
        , m_value_retriever_alternate(std::move(value_retriever_alternate))
        , m_is_alternate_set(true)
    {
    }

    [[nodiscard]] std::string name() const
    {
        return m_name;
    }

    [[nodiscard]] D main() const
    {
        return m_value_retriever_main();
    }

    [[nodiscard]] D alternate() const
    {
        return m_value_retriever_alternate();
    }

    [[nodiscard]] bool is_alternate_set() const
    {
        return m_is_alternate_set;
    }

private:
    std::string m_name;
    std::function<D()> m_value_retriever_main;
    std::function<D()> m_value_retriever_alternate;
    bool m_is_alternate_set;
};

template<class D>
class FlagRegisterDebugContainer {
public:
    FlagRegisterDebugContainer() = default;

    FlagRegisterDebugContainer(
        std::string name,
        std::function<D()> value_retriever,
        std::vector<std::tuple<std::string, unsigned int>> flag_names)
        : m_name(std::move(name))
        , m_value_retriever(std::move(value_retriever))
        , m_flag_names(std::move(flag_names))
    {
    }

    [[nodiscard]] std::string name() const
    {
        return m_name;
    }

    [[nodiscard]] D value() const
    {
        return m_value_retriever();
    }

    [[nodiscard]] std::vector<std::tuple<std::string, unsigned int>> flag_names() const
    {
        return m_flag_names;
    }

private:
    std::string m_name;
    std::function<D()> m_value_retriever;
    std::vector<std::tuple<std::string, unsigned int>> m_flag_names;
};

template<class D>
class IoDebugContainer {
public:
    IoDebugContainer(
        std::string name,
        std::function<bool()> is_active_retriever,
        std::function<D()> value_retriever)
        : m_name(std::move(name))
        , m_is_active_retriever(std::move(is_active_retriever))
        , m_value_retriever(std::move(value_retriever))
        , m_is_divided_into_bits(false)
    {
    }

    IoDebugContainer(
        std::string name,
        std::function<bool()> is_active_retriever,
        std::function<D()> value_retriever,
        std::vector<std::tuple<std::string, unsigned int>> bit_names)
        : m_name(std::move(name))
        , m_is_active_retriever(std::move(is_active_retriever))
        , m_value_retriever(std::move(value_retriever))
        , m_bit_names(std::move(bit_names))
        , m_is_divided_into_bits(true)
    {
    }

    [[nodiscard]] std::string name() const
    {
        return m_name;
    }

    [[nodiscard]] bool is_active() const
    {
        return m_is_active_retriever();
    }

    [[nodiscard]] D value() const
    {
        return m_value_retriever();
    }

    [[nodiscard]] bool is_divided_into_bits() const
    {
        return m_is_divided_into_bits;
    }

    [[nodiscard]] std::vector<std::tuple<std::string, unsigned int>> bit_names() const
    {
        return m_bit_names;
    }

private:
    std::string m_name;
    std::function<bool()> m_is_active_retriever;
    std::function<D()> m_value_retriever;
    std::vector<std::tuple<std::string, unsigned int>> m_bit_names;
    bool m_is_divided_into_bits;
};

template<class D>
class MemoryDebugContainer {
public:
    MemoryDebugContainer() = default;

    explicit MemoryDebugContainer(std::function<std::vector<D>()> value_retriever)
    {
        m_value_retriever = std::move(value_retriever);
    }

    [[nodiscard]] std::vector<D> value() const
    {
        return m_value_retriever();
    }

private:
    std::function<std::vector<D>()> m_value_retriever;
};

/**
 * Contains the data that is used in the debugging panes.
 *
 * @tparam A is the address type
 * @tparam D is the data type
 * @tparam B is the base (hex or decimal)
 */
template<class A, class D, std::size_t B>
class DebugContainer {
public:
    DebugContainer() = default;

    bool is_decimal()
    {
        return B == 10;
    }

    void add_register(RegisterDebugContainer<D> const& reg)
    {
        m_register_retrievers.emplace_back(reg);
        if (reg.is_alternate_set()) {
            m_has_alternate_registers = true;
        }
    }

    std::vector<RegisterDebugContainer<D>> registers()
    {
        return m_register_retrievers;
    }

    [[nodiscard]] bool has_alternate_registers() const
    {
        return m_has_alternate_registers;
    }

    void add_flag_register(FlagRegisterDebugContainer<D> const& flag_reg)
    {
        m_flag_register_retriever = flag_reg;
        m_is_flag_register_set = true;
    }

    [[nodiscard]] FlagRegisterDebugContainer<D> flag_register() const
    {
        return m_flag_register_retriever;
    }

    [[nodiscard]] bool is_flag_register_set() const
    {
        return m_is_flag_register_set;
    }

    void add_io(IoDebugContainer<D> const& io)
    {
        m_io_retrievers.emplace_back(io);
        m_is_io_set = true;
    }

    [[nodiscard]] std::vector<IoDebugContainer<D>> io() const
    {
        return m_io_retrievers;
    }

    [[nodiscard]] bool is_io_set() const
    {
        return m_is_io_set;
    }

    void add_memory(MemoryDebugContainer<D> const& memory)
    {
        m_memory_retriever = memory;
        m_is_memory_set = true;
    }

    [[nodiscard]] MemoryDebugContainer<D> memory() const
    {
        return m_memory_retriever;
    }

    [[nodiscard]] bool is_memory_set() const
    {
        return m_is_memory_set;
    }

    void add_pc(std::function<A()> const& value_retriever)
    {
        m_pc_retriever = value_retriever;
        m_is_pc_set = true;
    }

    [[nodiscard]] A pc() const
    {
        return m_pc_retriever();
    }

    [[nodiscard]] bool is_pc_set() const
    {
        return m_is_pc_set;
    }

    void add_sp(std::function<A()> const& value_retriever)
    {
        m_sp_retriever = value_retriever;
        m_is_sp_set = true;
    }

    [[nodiscard]] A sp() const
    {
        return m_sp_retriever();
    }

    [[nodiscard]] bool is_sp_set() const
    {
        return m_is_sp_set;
    }

    void add_is_interrupted(std::function<bool()> const& value_retriever)
    {
        m_is_interrupted_retriever = value_retriever;
        m_is_interrupted_set = true;
    }

    [[nodiscard]] bool is_interrupted() const
    {
        return m_is_interrupted_retriever();
    }

    [[nodiscard]] bool is_interrupted_set() const
    {
        return m_is_interrupted_set;
    }

    void add_interrupt_mode(std::function<std::string()> const& value_retriever)
    {
        m_interrupt_mode_retriever = value_retriever;
        m_is_interrupt_mode_set = true;
    }

    [[nodiscard]] std::string interrupt_mode() const
    {
        return m_interrupt_mode_retriever();
    }

    [[nodiscard]] bool is_interrupt_mode_set() const
    {
        return m_is_interrupt_mode_set;
    }

    void add_disassembled_program(std::vector<DisassembledLine<A, B>> disassembled_program)
    {
        m_disassembled_program = std::move(disassembled_program);
        m_is_disassembled_program_set = true;
    }

    std::vector<DisassembledLine<A, B>> const& disassembled_program() const
    {
        return m_disassembled_program;
    }

    [[nodiscard]] bool is_disassembled_program_set() const
    {
        return m_is_disassembled_program_set;
    }

    void add_tilemap(std::vector<std::vector<std::shared_ptr<Tile>>> tiles)
    {
        m_tiles = std::move(tiles);
        m_is_tilemap_set = true;
    }

    std::vector<std::vector<std::shared_ptr<Tile>>> tiles()
    {
        return m_tiles;
    }

    [[nodiscard]] bool is_tilemap_set() const
    {
        return m_is_tilemap_set;
    }

    void add_spritemap(std::tuple<
        std::vector<std::vector<std::shared_ptr<Sprite>>>,
        std::vector<std::vector<std::shared_ptr<Sprite>>>,
        std::vector<std::vector<std::shared_ptr<Sprite>>>,
        std::vector<std::vector<std::shared_ptr<Sprite>>>> const& sprites)
    {
        m_sprites = sprites;
        m_is_spritemap_set = true;
    }

    std::tuple<
        std::vector<std::vector<std::shared_ptr<Sprite>>>,
        std::vector<std::vector<std::shared_ptr<Sprite>>>,
        std::vector<std::vector<std::shared_ptr<Sprite>>>,
        std::vector<std::vector<std::shared_ptr<Sprite>>>>
    sprites()
    {
        return m_sprites;
    }

    [[nodiscard]] bool is_spritemap_set() const
    {
        return m_is_spritemap_set;
    }

    void add_waveforms(std::vector<Waveform> const& waveforms)
    {
        m_waveforms = waveforms;
        m_is_waveforms_set = true;
    }

    std::vector<Waveform> waveforms()
    {
        return m_waveforms;
    }

    [[nodiscard]] bool is_waveforms_set() const
    {
        return m_is_waveforms_set;
    }

    void add_file_content(std::function<std::string()> const& value_retriever)
    {
        m_file_content_retriever = value_retriever;
        m_is_file_content_set = true;
    }

    [[nodiscard]] std::string file_content() const
    {
        return m_file_content_retriever();
    }

    [[nodiscard]] bool is_file_content_set() const
    {
        return m_is_file_content_set;
    }

private:
    std::vector<RegisterDebugContainer<D>> m_register_retrievers;
    bool m_has_alternate_registers { false };

    FlagRegisterDebugContainer<D> m_flag_register_retriever;
    bool m_is_flag_register_set { false };

    std::vector<IoDebugContainer<D>> m_io_retrievers;
    bool m_is_io_set { false };

    MemoryDebugContainer<D> m_memory_retriever;
    bool m_is_memory_set { false };

    std::function<A()> m_pc_retriever;
    bool m_is_pc_set { false };

    std::function<A()> m_sp_retriever;
    bool m_is_sp_set { false };

    std::function<bool()> m_is_interrupted_retriever;
    bool m_is_interrupted_set { false };

    std::function<std::string()> m_interrupt_mode_retriever;
    bool m_is_interrupt_mode_set { false };

    std::vector<DisassembledLine<A, B>> m_disassembled_program;
    bool m_is_disassembled_program_set { false };

    std::vector<std::vector<std::shared_ptr<Tile>>> m_tiles;
    bool m_is_tilemap_set { false };

    std::tuple<
        std::vector<std::vector<std::shared_ptr<Sprite>>>,
        std::vector<std::vector<std::shared_ptr<Sprite>>>,
        std::vector<std::vector<std::shared_ptr<Sprite>>>,
        std::vector<std::vector<std::shared_ptr<Sprite>>>>
        m_sprites;
    bool m_is_spritemap_set { false };

    std::vector<Waveform> m_waveforms;
    bool m_is_waveforms_set { false };

    std::function<std::string()> m_file_content_retriever;
    bool m_is_file_content_set { false };
};
}
