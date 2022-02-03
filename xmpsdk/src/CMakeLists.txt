add_library(xmpsdk STATIC include/XMP.hpp)
target_include_directories(xmpsdk PUBLIC include/)
set_target_properties(xmpsdk PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(xmpsdk PUBLIC cxx_std_20)