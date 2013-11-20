project(AnalitzaWidgets)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_path(IEEEFP_DIR ieeefp.h)
if(IEEEFP_DIR)
    include_directories(${IEEEFP_DIR})
    add_definitions(-DHAVE_IEEEFP_H)
endif(IEEEFP_DIR)

set(analitzaplot_SRCS
    plotsview2d.cpp
)
if(HAVE_OPENGL)
    list(APPEND analitzaplot_SRCS plotsview3d.cpp)
endif(HAVE_OPENGL)

add_library(AnalitzaWidgets SHARED ${analitzaplot_SRCS})
target_link_libraries(AnalitzaWidgets PUBLIC Qt5::Widgets Qt5::OpenGL Analitza AnalitzaPlot
                                      PRIVATE Qt5::Svg
)

set_target_properties(AnalitzaWidgets PROPERTIES VERSION ${ANALITZA_VERSION_STRING} SOVERSION ${ANALITZA_SOVERSION} )

install(TARGETS AnalitzaWidgets EXPORT AnalitzaTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
    plotsview2d.h
    analitzawidgets_export.h

    DESTINATION ${INCLUDE_INSTALL_DIR}/analitzagui/ COMPONENT Devel
)

if(HAVE_OPENGL)
    install(FILES
        plotsview3d.h

        DESTINATION ${INCLUDE_INSTALL_DIR}/analitzagui/ COMPONENT Devel
    )
endif(HAVE_OPENGL)