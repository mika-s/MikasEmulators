set(SOURCES_DOCTEST
        doctest.h
        )

add_library(Doctest INTERFACE ${SOURCES_DOCTEST})
target_include_directories(Doctest INTERFACE ./)
