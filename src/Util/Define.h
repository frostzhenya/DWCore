/*-------------------------------------------------------+
| DWCore Content Management System
| Copyright (C) 2012 - 2013 lovepsone
+--------------------------------------------------------+
| Filename: Define.h
| Author: RMDC, lovepsone
+--------------------------------------------------------+
| Removal of this copyright header is strictly prohibited 
| without written permission from the original author(s).
+--------------------------------------------------------*/

#ifndef DWCORE_DEFINE_H
#define DWCORE_DEFINE_H

#include <sys/types.h>

#include <ace/Basic_Types.h>
#include <ace/Default_Constants.h>
#include <ace/OS_NS_dlfcn.h>
#include <ace/ACE_export.h>

#include "CompilerDefs.h"

#define DWCORE_LITTLEENDIAN 0
#define DWCORE_BIGENDIAN    1


#if !defined(DWCORE_ENDIAN)
#  if defined (ACE_BIG_ENDIAN)
#    define DWCORE_ENDIAN DWCORE_BIGENDIAN
#  else //ACE_BYTE_ORDER != ACE_BIG_ENDIAN
#    define DWCORE_ENDIAN DWCORE_LITTLEENDIAN
#  endif //ACE_BYTE_ORDER
#endif //DWCORE_ENDIAN

typedef ACE_SHLIB_HANDLE DWCORE_LIBRARY_HANDLE;

#define DWCORE_SCRIPT_SUFFIX ACE_DLL_SUFFIX
#define DWCORE_SCRIPT_PREFIX ACE_DLL_PREFIX
#define DWCORE_LOAD_LIBRARY(libname)    ACE_OS::dlopen(libname)
#define DWCORE_CLOSE_LIBRARY(hlib)      ACE_OS::dlclose(hlib)
#define DWCORE_GET_PROC_ADDR(hlib,name) ACE_OS::dlsym(hlib,name)

#define DWCORE_PATH_MAX PATH_MAX                            // ace/os_include/os_limits.h -> ace/Basic_Types.h

#if PLATFORM == PLATFORM_WINDOWS
#  define DWCORE_EXPORT __declspec(dllexport)
#  define DWCORE_IMPORT __cdecl
#else //PLATFORM != PLATFORM_WINDOWS
#  define DWCORE_EXPORT export
#  if defined(__APPLE_CC__) && defined(BIG_ENDIAN)
#    define DWCORE_IMPORT __attribute__ ((longcall))
#  elif defined(__x86_64__)
#    define DWCORE_IMPORT
#  else
#    define DWCORE_IMPORT __attribute__ ((cdecl))
#  endif //__APPLE_CC__ && BIG_ENDIAN
#endif //PLATFORM

#if PLATFORM == PLATFORM_WINDOWS
#  ifdef DWCORE_WIN32_DLL_IMPORT
#    define DWCORE_DLL_DECL __declspec(dllimport)
#  else //!DWCORE_WIN32_DLL_IMPORT
#    ifdef DWCORE_WIND_DLL_EXPORT
#      define DWCORE_DLL_DECL __declspec(dllexport)
#    else //!DWCORE_WIND_DLL_EXPORT
#      define DWCORE_DLL_DECL
#    endif //DWCORE_WIND_DLL_EXPORT
#  endif //DWCORE_WIN32_DLL_IMPORT
#else //PLATFORM != PLATFORM_WINDOWS
#  define DWCORE_DLL_DECL
#endif //PLATFORM

#if PLATFORM == PLATFORM_WINDOWS
#  define DWCORE_DLL_SPEC __declspec(dllexport)
#  ifndef DECLSPEC_NORETURN
#    define DECLSPEC_NORETURN __declspec(noreturn)
#  endif //DECLSPEC_NORETURN
#else //PLATFORM != PLATFORM_WINDOWS
#  define DWCORE_DLL_SPEC
#  define DECLSPEC_NORETURN
#endif //PLATFORM

#if !defined(DEBUG)
#  define DWCORE_INLINE inline
#else //DEBUG
#  if !defined(DWCORE_DEBUG)
#    define DWCORE_DEBUG
#  endif //DWCORE_DEBUG
#  define DWCORE_INLINE
#endif //!DEBUG

#if COMPILER == COMPILER_GNU
#  define ATTR_NORETURN __attribute__((noreturn))
#  define ATTR_PRINTF(F,V) __attribute__ ((format (printf, F, V)))
#else //COMPILER != COMPILER_GNU
#  define ATTR_NORETURN
#  define ATTR_PRINTF(F,V)
#endif //COMPILER == COMPILER_GNU

typedef ACE_INT64 int64;
typedef ACE_INT32 int32;
typedef ACE_INT16 int16;
typedef ACE_INT8 int8;
typedef ACE_UINT64 uint64;
typedef ACE_UINT32 uint32;
typedef ACE_UINT16 uint16;
typedef ACE_UINT8 uint8;

#if COMPILER != COMPILER_MICROSOFT
typedef uint16      WORD;
typedef uint32      DWORD;
#endif //COMPILER

#if COMPILER == COMPILER_GNU
#  if !defined(__GXX_EXPERIMENTAL_CXX0X__) || (__GNUC__ < 4) || (__GNUC__ == 4) && (__GNUC_MINOR__ < 7)
#    define override
#  endif
#endif

typedef uint64 OBJECT_HANDLE;

#endif
