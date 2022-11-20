set(SOURCES_LMC_CPP
        assembler.cpp
        cpu.cpp
        unrecognized_assembly_exception.cpp
        instructions/add.cpp
        instructions/sub.cpp
        instructions/brp.cpp
        instructions/brz.cpp
        instructions/hlt.cpp
        instructions/inp.cpp
        instructions/lda.cpp
        instructions/out.cpp
        instructions/sta.cpp
        instructions/sub.cpp
        )

set(SOURCES_LMC_H
        assembler.h
        cpu.h
        opcode.h
        unrecognized_assembly_exception.h
        instructions/instructions.h
        interfaces/gui_observer.h
        interfaces/in_observer.h
        interfaces/out_observer.h
        )

add_library(LMC STATIC ${SOURCES_LMC_CPP} ${SOURCES_LMC_H})
target_link_libraries(LMC PRIVATE Doctest Crosscutting)
target_include_directories(LMC PUBLIC ../)

if (iwyu_path)
    set_property(TARGET LMC PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif ()
