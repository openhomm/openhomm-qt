// openhomm - open source clone of Heroes of Might and Magic III
// Copyright (C) 2009 openhomm developers team (see AUTHORS)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#pragma once

#ifdef _MSC_VER
#   define COMPILER_MSC
#elif __INTEL_COMPILER
#   define COMPILER_ICC
#elif __GNUC__
#   define COMPILER_GCC
#else
#   error "Unknown compiler!"
#endif

#if defined(Q_WS_WIN32) && defined(COMPILER_GCC)
#   define COMPILER_MINGW
#endif

// Definitions
#ifdef COMPILER_MSC
#   define HR_INLINE __forceinline
#else
#   define HR_INLINE inline
#endif
