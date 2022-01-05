file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${Vesuvianite_SRC_DIR}/include/vesuvianite/*.hpp")

add_library(rawLoaderLib RawLoader.cxx ${HEADER_LIST})
add_library(houghLines HoughLines.cxx ${HEADER_LIST})
add_library(convexHull ConvexHull.cxx ${HEADER_LIST})


find_package(OpenCV)

if(OpenCV_FOUND)
  include_directories( ${OpenCV_INCLUDE_DIRS} )
  target_link_libraries( rawLoaderLib ${OpenCV_LIBS} )
endif()

target_include_directories(rawLoaderLib PUBLIC ../include)
target_compile_features(rawLoaderLib PUBLIC cxx_std_20)

target_include_directories(houghLines PUBLIC ../include)
target_compile_features(houghLines PUBLIC cxx_std_20)

target_include_directories(convexHull PUBLIC ../include)
target_compile_features(convexHull PUBLIC cxx_std_20)