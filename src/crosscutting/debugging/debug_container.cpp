#include "debug_container.h"

namespace emu::debugger {

    RegisterDebugContainer::RegisterDebugContainer(
            std::string name,
            std::function<u8()> value_retriever_main
    ) : m_name(std::move(name)),
        m_value_retriever_main(std::move(value_retriever_main)),
        m_value_retriever_alternate([&]() { return 0; }),
        m_is_alternate_set(false) {
    }

    RegisterDebugContainer::RegisterDebugContainer(
            std::string name,
            std::function<u8()> value_retriever_main,
            std::function<u8()> value_retriever_alternate
    ) : m_name(std::move(name)),
        m_value_retriever_main(std::move(value_retriever_main)),
        m_value_retriever_alternate(std::move(value_retriever_alternate)),
        m_is_alternate_set(true) {
    }

    std::string RegisterDebugContainer::name() const {
        return m_name;
    }

    u8 RegisterDebugContainer::main() const {
        return m_value_retriever_main();
    }

    u8 RegisterDebugContainer::alternate() const {
        return m_value_retriever_alternate();
    }

    bool RegisterDebugContainer::is_alternate_set() const {
        return m_is_alternate_set;
    }

    FlagRegisterDebugContainer::FlagRegisterDebugContainer() = default;

    FlagRegisterDebugContainer::FlagRegisterDebugContainer(
            std::string name,
            std::function<u8()> value_retriever,
            std::vector<std::tuple<std::string, unsigned int>> flag_names
    ) : m_name(std::move(name)),
        m_value_retriever(std::move(value_retriever)),
        m_flag_names(std::move(flag_names)) {
    }

    std::string FlagRegisterDebugContainer::name() const {
        return m_name;
    }

    u8 FlagRegisterDebugContainer::value() const {
        return m_value_retriever();
    }

    std::vector<std::tuple<std::string, unsigned int>> FlagRegisterDebugContainer::flag_names() const {
        return m_flag_names;
    }

    IoDebugContainer::IoDebugContainer(
            std::string name,
            std::function<bool()> is_active_retriever,
            std::function<u8()> value_retriever
    ) : m_name(std::move(name)),
        m_is_active_retriever(std::move(is_active_retriever)),
        m_value_retriever(std::move(value_retriever)),
        m_is_divided_into_bits(false) {
    }

    IoDebugContainer::IoDebugContainer(
            std::string name,
            std::function<bool()> is_active_retriever,
            std::function<u8()> value_retriever,
            std::vector<std::tuple<std::string, unsigned int>> m_bit_names
    ) : m_name(std::move(name)),
        m_is_active_retriever(std::move(is_active_retriever)),
        m_value_retriever(std::move(value_retriever)),
        m_bit_names(std::move(m_bit_names)),
        m_is_divided_into_bits(true) {
    }

    std::string IoDebugContainer::name() const {
        return m_name;
    }

    bool IoDebugContainer::is_active() const {
        return m_is_active_retriever();
    }

    u8 IoDebugContainer::value() const {
        return m_value_retriever();
    }

    bool IoDebugContainer::is_divided_into_bits() const {
        return m_is_divided_into_bits;
    }

    std::vector<std::tuple<std::string, unsigned int>> IoDebugContainer::bit_names() const {
        return m_bit_names;
    }

    MemoryDebugContainer::MemoryDebugContainer() = default;

    MemoryDebugContainer::MemoryDebugContainer(std::function<std::vector<u8>()> value_retriever) {
        m_value_retriever = std::move(value_retriever);
    }

    std::vector<u8> MemoryDebugContainer::value() const {
        return m_value_retriever();
    }

    DebugContainer::DebugContainer()
            : m_has_alternate_registers(false),
              m_flag_register_retriever(FlagRegisterDebugContainer()),
              m_is_flag_register_set(false),
              m_is_io_set(false),
              m_is_memory_set(false),
              m_is_pc_set(false),
              m_is_sp_set(false),
              m_is_interrupted_set(false),
              m_is_interrupt_mode_set(false),
              m_is_disassembled_program_set(false),
              m_is_tilemap_set(false) {
    }

    void DebugContainer::add_register(const RegisterDebugContainer &reg) {
        m_register_retrievers.emplace_back(reg);
        if (reg.is_alternate_set()) {
            m_has_alternate_registers = true;
        }
    }

    std::vector<RegisterDebugContainer> DebugContainer::registers() {
        return m_register_retrievers;
    }

    bool DebugContainer::has_alternate_registers() const {
        return m_has_alternate_registers;
    }

    void DebugContainer::add_flag_register(const FlagRegisterDebugContainer &flag_reg) {
        m_flag_register_retriever = flag_reg;
        m_is_flag_register_set = true;
    }

    FlagRegisterDebugContainer DebugContainer::flag_register() const {
        return m_flag_register_retriever;
    }

    bool DebugContainer::is_flag_register_set() const {
        return m_is_flag_register_set;
    }

    void DebugContainer::add_io(const IoDebugContainer &io) {
        m_io_retrievers.emplace_back(io);
        m_is_io_set = true;
    }

    std::vector<IoDebugContainer> DebugContainer::io() const {
        return m_io_retrievers;
    }

    bool DebugContainer::is_io_set() const {
        return m_is_io_set;
    }

    void DebugContainer::add_memory(const MemoryDebugContainer &memory) {
        m_memory_retriever = memory;
        m_is_memory_set = true;
    }

    MemoryDebugContainer DebugContainer::memory() const {
        return m_memory_retriever;
    }

    bool DebugContainer::is_memory_set() const {
        return m_is_memory_set;
    }

    void DebugContainer::add_pc(const std::function<u16()> &value_retriever) {
        m_pc_retriever = value_retriever;
        m_is_pc_set = true;
    }

    u16 DebugContainer::pc() const {
        return m_pc_retriever();
    }

    bool DebugContainer::is_pc_set() const {
        return m_is_pc_set;
    }

    void DebugContainer::add_sp(const std::function<u16()> &value_retriever) {
        m_sp_retriever = value_retriever;
        m_is_sp_set = true;
    }

    u16 DebugContainer::sp() const {
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

    void DebugContainer::add_interrupt_mode(const std::function<std::string()> &value_retriever) {
        m_interrupt_mode_retriever = value_retriever;
        m_is_interrupt_mode_set = true;
    }

    std::string DebugContainer::interrupt_mode() const {
        return m_interrupt_mode_retriever();
    }

    bool DebugContainer::is_interrupt_mode_set() const {
        return m_is_interrupt_mode_set;
    }

    void DebugContainer::add_disassembled_program(std::vector<DisassembledLine> disassembled_program) {
        m_disassembled_program = std::move(disassembled_program);
        m_is_disassembled_program_set = true;
    }

    std::vector<DisassembledLine> DebugContainer::disassembled_program() {
        return m_disassembled_program;
    }

    bool DebugContainer::is_disassembled_program_set() const {
        return m_is_disassembled_program_set;
    }

    void DebugContainer::add_tilemap(std::vector<std::vector<std::shared_ptr<Tile>>> tiles) {
        m_tiles = std::move(tiles);
        m_is_tilemap_set = true;
    }

    std::vector<std::vector<std::shared_ptr<Tile>>> DebugContainer::tiles() {
        return m_tiles;
    }

    bool DebugContainer::is_tilemap_set() const {
        return m_is_tilemap_set;
    }

    void DebugContainer::add_spritemap(const std::tuple<
            std::vector<std::vector<std::shared_ptr<Sprite>>>,
            std::vector<std::vector<std::shared_ptr<Sprite>>>,
            std::vector<std::vector<std::shared_ptr<Sprite>>>,
            std::vector<std::vector<std::shared_ptr<Sprite>>>
    > &sprites) {
        m_sprites = sprites;
        m_is_spritemap_set = true;
    }

    std::tuple<
            std::vector<std::vector<std::shared_ptr<Sprite>>>,
            std::vector<std::vector<std::shared_ptr<Sprite>>>,
            std::vector<std::vector<std::shared_ptr<Sprite>>>,
            std::vector<std::vector<std::shared_ptr<Sprite>>>
    > DebugContainer::sprites() {
        return m_sprites;
    }

    bool DebugContainer::is_spritemap_set() const {
        return m_is_spritemap_set;
    }
}
