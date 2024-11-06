# - Try to find GTK+ 3 Once done, this will define
#
# GTK3_FOUND - system has GTK+ 3. GTK3_INCLUDE_DIRS - the GTK+ 3. include
# directories GTK3_LIBRARIES - link these to use GTK+ 3.
#
# Copyright (C) 2012 Raphael Kubo da Costa <rakuco@webkit.org> Copyright (C)
# 2013 Igalia S.L.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met: 1.
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer. 2.  Redistributions in binary
# form must reproduce the above copyright notice, this list of conditions and
# the following disclaimer in the documentation and/or other materials provided
# with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND ITS CONTRIBUTORS ``AS
# IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR ITS CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
set(FindGTK3_SAVE_CMLOC ${CMLOC})
set(CMLOC "FindGTK3: ")

message(STATUS "${CMLOC}Finding package PkgConfig")
find_package(PkgConfig)
pkg_check_modules(GTK3 gtk+-3.0)
set(VERSION_OK TRUE)
if (GTK3_VERSION)
  message(STATUS "${CMLOC}Found GTK3_VERSION")
  if (GTK3_FIND_VERSION_EXACT)
    message(STATUS "${CMLOC}Find exact version of GTK3")
    if (NOT ("${GTK3_FIND_VERSION}" VERSION_EQUAL "${GTK3_VERSION}"))
      set(VERSION_OK FALSE)
      message(
        STATUS
          "${CMLOC}GTK3 exact version not found: GTK3_FIND_VERSION: ${GTK3_FIND_VERSION} != GTK3_VERSION: ${GTK3_VERSION}"
      )
    endif ()
  else ()
    if ("${GTK3_VERSION}" VERSION_LESS "${GTK3_FIND_VERSION}")
      set(VERSION_OK FALSE)
      message(
        STATUS
          "${CMLOC}GTK3 exact version not found: GTK3_VERSION: ${GTK3_VERSION} < GTK3_FIND_VERSION: ${GTK3_FIND_VERSION}"
      )
    endif ()
  endif ()
elseif ()
  message(STATUS "${CMLOC}GTK3 not found")
endif ()
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  GTK3 DEFAULT_MSG GTK3_INCLUDE_DIRS GTK3_LIBRARIES VERSION_OK
)
message(STATUS "${CMLOC}Finished FindGTK3")

set(CMLOC ${FindGTK3_SAVE_CMLOC})
