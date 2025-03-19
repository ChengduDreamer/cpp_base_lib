project(yk_base_lib)

#include_directories(${CMAKE_SOURCE_DIR}/3rdparty/spdlog/include/)
#include_directories(${CMAKE_SOURCE_DIR}/3rdparty/spdlog/include/spdlog)

include_directories(${CMAKE_SOURCE_DIR}/3rdparty/spdlog/include)

include_directories(
		${OPENSSL_ROOT_DIR}/include
		${CMAKE_BINARY_DIR}
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/)
add_definitions(-DSPDLOG_WCHAR_SUPPORT -DSPDLOG_WCHAR_FILENAMES)

add_definitions("-DUNICODE" "-D_UNICODE")
#add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
#add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options(
    /bigobj       # large object file format
    #/permissive-  # strict C++
    #/wd4503      # decorated name length exceeded, name was truncated
    /W4           # enable all warnings
    /JMC
)



set(BASELIB_SOURCES
	yk_logger.cpp
    str_util.cpp
    path_util.cpp
    win32_helper.cpp
    msg_notifier.cpp
    thread.cpp
    file.cpp
    data.cpp
)

set(D3DTexture_SOURCES
    d3d_helper.cpp
    d3d_render.cpp
)

if(${YK_ENABLE_D3DTexture})
    list(APPEND BASELIB_SOURCES ${D3DTexture_SOURCES})
endif()


add_library(yk_base_lib STATIC ${BASELIB_SOURCES})

#为 yk_base_lib 目标设置一个接口包含目录，指向当前目录。
#任何链接到 yk_base_lib 的其他目标在编译时将会查找当前目录中的头文件。
target_include_directories(yk_base_lib INTERFACE .)

#不写这一句，无法include spdlog的头文件
target_link_libraries(yk_base_lib 
    Dexode::EventBus 
    spdlog::spdlog_header_only
    Qt5::Widgets Qt5::Core
)

if(WIN32 AND ${YK_ENABLE_D3DTexture})
    target_link_libraries(yk_base_lib DirectXTex)
endif()


