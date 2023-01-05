set(SOURCES_Z80_CPP
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
        instructions/cpd.cpp
        instructions/cpdr.cpp
        instructions/cpi.cpp
        instructions/cpir.cpp
        instructions/cpl.cpp
        instructions/daa.cpp
        instructions/dec.cpp
        instructions/di.cpp
        instructions/djnz.cpp
        instructions/ei.cpp
        instructions/ex.cpp
        instructions/exx.cpp
        instructions/halt.cpp
        instructions/im.cpp
        instructions/in.cpp
        instructions/inc.cpp
        instructions/ind.cpp
        instructions/indr.cpp
        instructions/ini.cpp
        instructions/inir.cpp
        instructions/instruction_util.cpp
        instructions/jp.cpp
        instructions/jr.cpp
        instructions/ld.cpp
        instructions/ldd.cpp
        instructions/lddr.cpp
        instructions/ldi.cpp
        instructions/ldir.cpp
        instructions/neg.cpp
        instructions/nop.cpp
        instructions/or.cpp
        instructions/out.cpp
        instructions/outd.cpp
        instructions/outi.cpp
        instructions/otdr.cpp
        instructions/otir.cpp
        instructions/pop.cpp
        instructions/push.cpp
        instructions/res.cpp
        instructions/ret.cpp
        instructions/reti.cpp
        instructions/retn.cpp
        instructions/rl.cpp
        instructions/rlc.cpp
        instructions/rld.cpp
        instructions/rr.cpp
        instructions/rrd.cpp
        instructions/rrc.cpp
        instructions/rst.cpp
        instructions/sbc.cpp
        instructions/scf.cpp
        instructions/set.cpp
        instructions/sla.cpp
        instructions/sll.cpp
        instructions/sra.cpp
        instructions/srl.cpp
        instructions/sub.cpp
        instructions/xor.cpp
        )

set(SOURCES_Z80_H
        cpu.h
        disassembler.h
        flags.h
        util.h
        interrupt_mode.h
        manual_state.h
        instructions/instruction_util.h
        instructions/instructions.h
        interfaces/in_observer.h
        interfaces/out_observer.h
        )

add_library(Z80 STATIC ${SOURCES_Z80_CPP} ${SOURCES_Z80_H})
target_link_libraries(Z80 PRIVATE Doctest Crosscutting)
target_include_directories(Z80 PUBLIC ../)

if (iwyu_path)
    set(iwyu_path_and_options ${iwyu_path} -Xiwyu --mapping_file=${PROJECT_SOURCE_DIR}/iwyu.imp)
    set_property(TARGET Z80 PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path_and_options})
endif ()
