set(SOURCES_SYNACOR_CPP
        cpu.cpp
        disassembler.cpp
        instructions/add.cpp
        instructions/and.cpp
        instructions/call.cpp
        instructions/eq.cpp
        instructions/gt.cpp
        instructions/halt.cpp
        instructions/in.cpp
        instructions/jf.cpp
        instructions/jmp.cpp
        instructions/jt.cpp
        instructions/mod.cpp
        instructions/mult.cpp
        instructions/noop.cpp
        instructions/not.cpp
        instructions/or.cpp
        instructions/out.cpp
        instructions/pop.cpp
        instructions/push.cpp
        instructions/ret.cpp
        instructions/rmem.cpp
        instructions/set.cpp
        instructions/wmem.cpp
        )

set(SOURCES_SYNACOR_H
        cpu.h
        disassembler.h
        usings.h
        instructions/instructions.h
        interfaces/in_observer.h
        interfaces/out_observer.h
        )

add_library(Synacor STATIC ${SOURCES_SYNACOR_CPP} ${SOURCES_SYNACOR_H})
target_link_libraries(Synacor PRIVATE Doctest Crosscutting)
target_include_directories(Synacor PUBLIC ../)

if (iwyu_path)
    set(iwyu_path_and_options ${iwyu_path} -Xiwyu --mapping_file=${PROJECT_SOURCE_DIR}/iwyu.imp)
    set_property(TARGET LMC PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path_and_options})
endif ()
