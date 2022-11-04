set(SOURCES_NAMCO_WSG3_CPP
        voice.cpp
        wsg3.cpp
        )

set(SOURCES_NAMCO_WSG3_H
        voice.h
        wsg3.h
        )

add_library(NamcoWsg3 STATIC ${SOURCES_NAMCO_WSG3_CPP} ${SOURCES_NAMCO_WSG3_H})
target_link_libraries(NamcoWsg3 PRIVATE Doctest Crosscutting)
target_include_directories(NamcoWsg3 PUBLIC ../)

if(iwyu_path)
    set_property(TARGET NamcoWsg3 PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()
