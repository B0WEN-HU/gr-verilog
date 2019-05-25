INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_VERILOG verilog)

FIND_PATH(
    VERILOG_INCLUDE_DIRS
    NAMES verilog/api.h
    HINTS $ENV{VERILOG_DIR}/include
        ${PC_VERILOG_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    VERILOG_LIBRARIES
    NAMES gnuradio-verilog
    HINTS $ENV{VERILOG_DIR}/lib
        ${PC_VERILOG_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(VERILOG DEFAULT_MSG VERILOG_LIBRARIES VERILOG_INCLUDE_DIRS)
MARK_AS_ADVANCED(VERILOG_LIBRARIES VERILOG_INCLUDE_DIRS)

