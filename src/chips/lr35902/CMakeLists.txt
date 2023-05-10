set(SOURCES_LR35902_CPP
        cpu.cpp
        disassembler.cpp
        flags.cpp
        util.cpp
        instructions/adc.cpp
        instructions/add.cpp
        instructions/and.cpp
        instructions/bit.cpp
        instructions/call.cpp
        instructions/ccf.cpp
        instructions/cp.cpp
        instructions/cpl.cpp
        instructions/daa.cpp
        instructions/dec.cpp
        instructions/di.cpp
        instructions/ei.cpp
        instructions/halt.cpp
        instructions/inc.cpp
        instructions/instruction_util.cpp
        instructions/jp.cpp
        instructions/jr.cpp
        instructions/ld.cpp
        instructions/ldh.cpp
        instructions/nop.cpp
        instructions/or.cpp
        instructions/pop.cpp
        instructions/push.cpp
        instructions/res.cpp
        instructions/ret.cpp
        instructions/reti.cpp
        instructions/rl.cpp
        instructions/rlc.cpp
        instructions/rr.cpp
        instructions/rrc.cpp
        instructions/rst.cpp
        instructions/sbc.cpp
        instructions/scf.cpp
        instructions/set.cpp
        instructions/sla.cpp
        instructions/sra.cpp
        instructions/srl.cpp
        instructions/stop.cpp
        instructions/sub.cpp
        instructions/swap.cpp
        instructions/xor.cpp
        )

set(SOURCES_LR35902_H
        cpu.h
        disassembler.h
        flags.h
        util.h
        manual_state.h
        instructions/instruction_util.h
        instructions/instructions.h
        interfaces/in_observer.h
        interfaces/out_observer.h
        )

add_library(LR35902 STATIC ${SOURCES_LR35902_CPP} ${SOURCES_LR35902_H})
target_link_libraries(LR35902 PRIVATE Doctest Crosscutting)
target_include_directories(LR35902 PUBLIC ../)

if (iwyu_path)
    set(iwyu_path_and_options ${iwyu_path} -Xiwyu --mapping_file=${PROJECT_SOURCE_DIR}/iwyu.imp)
    set_property(TARGET LR35902 PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path_and_options})
endif ()
