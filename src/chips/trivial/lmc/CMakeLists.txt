set(SOURCES_LMC_CPP
        assembler/assembler.cpp
        assembler/environment.cpp
        assembler/lmc_add.cpp
        assembler/lmc_bra.cpp
        assembler/lmc_brp.cpp
        assembler/lmc_brz.cpp
        assembler/lmc_dat.cpp
        assembler/lmc_hlt.cpp
        assembler/lmc_inp.cpp
        assembler/lmc_instruction.cpp
        assembler/lmc_integer.cpp
        assembler/lmc_label.cpp
        assembler/lmc_label_argument.cpp
        assembler/lmc_lda.cpp
        assembler/lmc_line.cpp
        assembler/lmc_operand.cpp
        assembler/lmc_otc.cpp
        assembler/lmc_out.cpp
        assembler/lmc_program.cpp
        assembler/lmc_sta.cpp
        assembler/lmc_sub.cpp
        assembler/scanner.cpp
        assembler/token.cpp
        cpu.cpp
        disassembler.cpp
        flags.cpp
        unrecognized_assembly_exception.cpp
        instructions/add.cpp
        instructions/sub.cpp
        instructions/bra.cpp
        instructions/brp.cpp
        instructions/brz.cpp
        instructions/hlt.cpp
        instructions/lda.cpp
        instructions/sta.cpp
        instructions/sub.cpp
        )

set(SOURCES_LMC_H
        assembler/assembler.h
        assembler/environment.h
        assembler/lmc_add.h
        assembler/lmc_bra.h
        assembler/lmc_brp.h
        assembler/lmc_brz.h
        assembler/lmc_dat.h
        assembler/lmc_hlt.h
        assembler/lmc_inp.h
        assembler/lmc_instruction.h
        assembler/lmc_integer.h
        assembler/lmc_label.h
        assembler/lmc_label_argument.h
        assembler/lmc_lda.h
        assembler/lmc_line.h
        assembler/lmc_operand.h
        assembler/lmc_otc.h
        assembler/lmc_out.h
        assembler/lmc_program.h
        assembler/lmc_sta.h
        assembler/lmc_sub.h
        assembler/scanner.h
        assembler/token.h
        assembler/token_kind.h
        cpu.h
        disassembler.h
        flags.h
        opcode.h
        usings.h
        unrecognized_assembly_exception.h
        instructions/instructions.h
        interfaces/in_observer.h
        interfaces/out_observer.h
        )

add_library(LMC STATIC ${SOURCES_LMC_CPP} ${SOURCES_LMC_H})
target_link_libraries(LMC PRIVATE Doctest Crosscutting)
target_include_directories(LMC PUBLIC ../)

if (iwyu_path)
    set_property(TARGET LMC PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif ()
