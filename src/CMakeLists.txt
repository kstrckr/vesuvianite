set(HEADER_LIST "${Vesuvianite_SRC_DIR}/include/vesuvianite/lib.hpp")
add_library(vesuvianite_lib lib.cxx)

find_package(OpenCV)

if(OpenCV_FOUND)
  include_directories( ${OpenCV_INCLUDE_DIRS} )
  target_link_libraries( vesuvianite_lib ${OpenCV_LIBS} )
endif()

target_include_directories(vesuvianite_lib PUBLIC ../include)
target_compile_features(vesuvianite_lib PUBLIC cxx_std_20)
