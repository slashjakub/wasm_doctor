# lib-wasi-littlefs

set(lib_wasi_littlefs_sources
	"wasi_vfs_impl_littlefs.c"
	"wasi_littlefs.c"
	"wasi_littlefs_impl.c"
	"wasi_littlefs_mount.c"
	"wasi_littlefs_ops.c"
)

set(lib_wasi_littlefs_headers
	"wasi_littlefs.h"
	"wasi_littlefs_mount.h"
)

if(NOT DEFINED TOYWASM_LITTLEFS_SOURCE_DIR)
set(TOYWASM_LITTLEFS_SOURCE_DIR "littlefs")
endif()

set(littlefs_sources
    "${TOYWASM_LITTLEFS_SOURCE_DIR}/lfs.c"
    "${TOYWASM_LITTLEFS_SOURCE_DIR}/lfs_util.c"
)

add_library(toywasm-lib-wasi-littlefs STATIC ${lib_wasi_littlefs_sources} ${littlefs_sources})
set_target_properties(toywasm-lib-wasi-littlefs PROPERTIES OUTPUT_NAME toywasm-wasi-littlefs)
if (USE_IPO)
# Note: -flto=full seems to produce considerably faster code
# than -flto=thin for us. However, cmake INTERPROCEDURAL_OPTIMIZATION
# always use -flto=thin for clang.
# cf. https://gitlab.kitware.com/cmake/cmake/-/issues/16808
set_property(TARGET toywasm-lib-wasi-littlefs PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
set_property(TARGET toywasm-lib-wasi-littlefs PROPERTY PUBLIC_HEADER ${lib_wasi_littlefs_headers})
target_link_libraries(toywasm-lib-wasi-littlefs toywasm-lib-wasi)
target_include_directories(toywasm-lib-wasi-littlefs
                           INTERFACE
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                           $<INSTALL_INTERFACE:include>
                           PRIVATE ${TOYWASM_LITTLEFS_SOURCE_DIR})
target_compile_definitions(toywasm-lib-wasi-littlefs PRIVATE LFS_THREADSAFE)
if (NOT CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(toywasm-lib-wasi-littlefs PRIVATE LFS_NO_DEBUG LFS_NO_ASSERT)
endif()

install(TARGETS toywasm-lib-wasi-littlefs
        EXPORT toywasm-lib-wasi-littlefs-config
        PUBLIC_HEADER DESTINATION include/toywasm)
install(EXPORT toywasm-lib-wasi-littlefs-config
        DESTINATION lib/cmake/toywasm-lib-wasi-littlefs)
install(FILES ${TOYWASM_LITTLEFS_SOURCE_DIR}/LICENSE.md DESTINATION share/doc/toywasm/littlefs)
