set(SOURCES_8080_CPP
        cpu.cpp
        disassembler.cpp
        flags.cpp
        shift_register.cpp
        instructions/aci.cpp
        instructions/adc.cpp
        instructions/add.cpp
        instructions/adi.cpp
        instructions/ana.cpp
        instructions/ani.cpp
        instructions/call.cpp
        instructions/cc.cpp
        instructions/cm.cpp
        instructions/cma.cpp
        instructions/cmc.cpp
        instructions/cmp.cpp
        instructions/cnc.cpp
        instructions/cnz.cpp
        instructions/cp.cpp
        instructions/cpe.cpp
        instructions/cpi.cpp
        instructions/cpo.cpp
        instructions/cz.cpp
        instructions/daa.cpp
        instructions/dad.cpp
        instructions/dcr.cpp
        instructions/dcx.cpp
        instructions/di.cpp
        instructions/ei.cpp
        instructions/hlt.cpp
        instructions/in.cpp
        instructions/inr.cpp
        instructions/instruction_util.cpp
        instructions/inx.cpp
        instructions/jc.cpp
        instructions/jm.cpp
        instructions/jmp.cpp
        instructions/jnc.cpp
        instructions/jnz.cpp
        instructions/jp.cpp
        instructions/jpe.cpp
        instructions/jpo.cpp
        instructions/jz.cpp
        instructions/lda.cpp
        instructions/ldax.cpp
        instructions/lhld.cpp
        instructions/lxi.cpp
        instructions/mov.cpp
        instructions/mvi.cpp
        instructions/nop.cpp
        instructions/ora.cpp
        instructions/ori.cpp
        instructions/out.cpp
        instructions/pchl.cpp
        instructions/pop.cpp
        instructions/push.cpp
        instructions/ral.cpp
        instructions/rar.cpp
        instructions/rc.cpp
        instructions/ret.cpp
        instructions/rlc.cpp
        instructions/rm.cpp
        instructions/rnc.cpp
        instructions/rnz.cpp
        instructions/rp.cpp
        instructions/rpe.cpp
        instructions/rpo.cpp
        instructions/rrc.cpp
        instructions/rst.cpp
        instructions/rz.cpp
        instructions/sbb.cpp
        instructions/sbi.cpp
        instructions/shld.cpp
        instructions/sphl.cpp
        instructions/sta.cpp
        instructions/stax.cpp
        instructions/stc.cpp
        instructions/sub.cpp
        instructions/sui.cpp
        instructions/unused.cpp
        instructions/xchg.cpp
        instructions/xra.cpp
        instructions/xri.cpp
        instructions/xthl.cpp
        )

set(SOURCES_8080_H
        cpu.h
        disassembler.h
        flags.h
        shift_register.h
        instructions/instruction_util.h
        instructions/instructions.h
        interfaces/in_observer.h
        interfaces/out_observer.h
        )

add_library(I8080 STATIC ${SOURCES_8080_CPP} ${SOURCES_8080_H})
target_link_libraries(I8080 PRIVATE Doctest Crosscutting)
target_include_directories(I8080 PUBLIC ../)

if (iwyu_path)
    set(iwyu_path_and_options ${iwyu_path} -Xiwyu --mapping_file=${PROJECT_SOURCE_DIR}/iwyu.imp)
    set_property(TARGET I8080 PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path_and_options})
endif ()
