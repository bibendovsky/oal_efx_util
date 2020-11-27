#[[

EAX OpenAL Extension

Copyright (c) 2020 Boris I. Bendovsky (bibendovsky@hotmail.com) and Contributors.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
OR OTHER DEALINGS IN THE SOFTWARE.

]]


cmake_minimum_required (VERSION 3.1.0 FATAL_ERROR)
project (oal_efx_util VERSION 1.0.0 LANGUAGES C)


#
# Module path.
#
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


#
# Includes.
#
include (make_version_string)


#
# Version.
#
make_version_string (OAL_EFX_UTIL_VERSION)
message (STATUS "[${PROJECT_NAME}] v${OAL_EFX_UTIL_VERSION}")


#
# Options.
#


#
# Packages.
#


#
# Target.
#
add_library (${PROJECT_NAME} STATIC "")
add_library (oal_efx_util::lib ALIAS ${PROJECT_NAME})


#
# Properties.
#
set_target_properties (
	${PROJECT_NAME}
	PROPERTIES
		C_STANDARD 99
		C_STANDARD_REQUIRED ON
		C_EXTENSIONS OFF
)


#
# Include directories.
#
target_include_directories (
	${PROJECT_NAME}
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
)


#
# Compile definitions.
#


#
# Compile options.
#


#
# Public header files.
#
set_target_properties (
	${PROJECT_NAME}
	PROPERTIES
		PUBLIC_HEADER "include/EFX-Util.h"
)


#
# Header files.
#
target_sources (
	${PROJECT_NAME}
	PRIVATE
		include/EFX-Util.h
)


#
# Source files.
#
target_sources (
	${PROJECT_NAME}
	PRIVATE
		src/EFX-Util.c
)


#
# Install.
#
install (
	TARGETS
		${PROJECT_NAME}
	ARCHIVE
		DESTINATION lib
	PUBLIC_HEADER
		DESTINATION include
)

install (
	FILES
		README.md
	DESTINATION
		"."
	RENAME
		"${PROJECT_NAME}_readme.txt"
)

install (
	FILES
		LICENSE.md
	DESTINATION
		"."
	RENAME
		"${PROJECT_NAME}_license.txt"
)

install (
	FILES
		CHANGELOG.md
	DESTINATION
		"."
	RENAME
		"${PROJECT_NAME}_changelog.txt"
)
