macro (check_environment)

	# setup environment
	if (CMAKE_SYSTEM_NAME MATCHES "Windows")
		set (proto_os_name "PROTO_OS_WINDOWS")
		set (proto_api_name "PROTO_API_WIN32")
	elseif (CMAKE_SYSTEM_NAME MATCHES "WindowsStore")
		set (proto_os_name "PROTO_OS_WINDOWS")
		set (proto_api_name "PROTO_API_WINRT")
	elseif (CMAKE_SYSTEM_NAME MATCHES "Android")
		set (proto_os_name "PROTO_OS_ANDROID")
		set (proto_api_name "PROTO_API_UNIX")
	elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
		set (proto_os_name "PROTO_OS_DARWIN")
		set (proto_api_name "PROTO_API_UNIX")
	else ()
		message (FATAL_ERROR "unsuported target system")
	endif ()

	add_definitions (
		-D${proto_os_name}
		-D${proto_api_name}
		-D${proto_cpu_name}
	)
	
	set (${proto_os_name}  TRUE)
	set (${proto_api_name} TRUE)
	set (${proto_cpu_name} TRUE)

	# workaround for cmake bug with CMAKE_SYSTEM_PROCESSOR on windows
	if (PROTO_OS_WINDOWS)
		if (MSVC_CXX_ARCHITECTURE_ID MATCHES "X86")
			set (proto_cpu_arch "PROTO_CPU_ARCH_X86")
		elseif (MSVC_CXX_ARCHITECTURE_ID MATCHES "AMD64")
			set (proto_cpu_arch "PROTO_CPU_ARCH_AMD64")
		elseif (MSVC_CXX_ARCHITECTURE_ID MATCHES "ARM")
			set (proto_cpu_arch "PROTO_CPU_ARCH_ARM")
		else()
			message (FATAL_ERROR "unsuported cpu architecture")
		endif()
	else()
		if (CMAKE_SYSTEM_PROCESSOR MATCHES "X86")
			set (proto_cpu_arch "PROTO_CPU_ARCH_X86")
		elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "AMD64")
			set (proto_cpu_arch "PROTO_CPU_ARCH_AMD64")
		elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "ARM")
			set (proto_cpu_arch "PROTO_CPU_ARCH_ARM")
		else()
			message (FATAL_ERROR "unsuported cpu architecture")
		endif()
	endif()	

	if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
		add_definitions (-DPROTO_COMPILER_MSVC)
		set (MSVC_INCREMENTAL_DEFAULT NO)
	elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		add_definitions (-DPROTO_COMPILER_CLANG)
	elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
		add_definitions (-DPROTO_COMPILER_GNU)
	else()
		message (ERROR "unsuported compiler")
	endif ()

endmacro ()