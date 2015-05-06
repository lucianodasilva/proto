
macro (check_operating_system) 

	if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
		add_definitions (-DPROTO_OS_WINDOWS)
	elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
		add_definitions (-DPROTO_OS_LINUX)
	elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		add_definitions (-DPROTO_OS_DARWIN)
	elseif (${CMAKE_SYSTEM_NAME} MATCHES "Sun")
		add_definitions (-DPROTO_OS_SUN)
	elseif (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
		add_definitions (-DPROTO_OS_FREEBSD)
	endif ()

	if (UNIX)
		add_definitions (-DPROTO_API_UNIX)
	elseif (WIN32)
		add_definitions (-DPROTO_API_WIN32)
	endif ()

	if (${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC")
		add_definitions (-DPROTO_COMPILER_MSVC)
	elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
		add_definitions (-DPROTO_COMPILER_CLANG)
	elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
		add_definitions (-DPROTO_COMPILER_GNU)
	elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "SunPro")
		add_definitions (-DPROTO_COMPILER_SUNPRO)
	endif ()

endmacro ()