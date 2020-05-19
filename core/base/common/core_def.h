#ifndef CORE_DEF_H
#define CORE_DEF_H

#include <stdint.h>
#include <float.h>

#ifdef __cplusplus
	extern "C" {
#endif


/* ////////////////////////////////////////////////////////////////////////////////
                                Export or Import
   ////////////////////////////////////////////////////////////////////////////  */
/* this helper definitions for shared libary support */
#if !defined( CORE_SYM_EXPORT_DEF )
#define CORE_SYM_EXPORT_DEF

#if defined( _WIN32 ) || defined( __CYGWIN__ )
  #define CORE_DLL_IMPORT  __declspec(dllimport)
  #define CORE_DLL_EXPORT  __declspec(dllexport)
  #define CORE_DLL_HIDDEN
#else
  #if __GNUC__ >= 4
     #define CORE_DLL_IMPORT __attribute__ ((visibility ("default")))
     #define CORE_DLL_EXPORT __attribute__ ((visibility ("default")))
     #define CORE_DLL_HIDDEN __attribute__ ((visibility ("hidden")))
  #else
     #define CORE_DLL_IMPORT
     #define CORE_DLL_EXPORT
     #define CORE_DLL_HIDDEN
  #endif
#endif

/* check if defined CORE_DLL */
#if defined( CORE_CFG_STATIC )  && !defined( CORE_CFG_STATIC )  && !defined( CORE_CFG_DLL )
  #define CORE_CFG_STATIC
#endif 
#if defined( CORE_CFG_DLL ) && !defined( CORE_CFG_STATIC )  &&  !defined( CORE_CFG_DLL )
  #define CORE_CFG_DLL
#endif

#undef CORE_API
#undef CORE_HIDDEN
#if defined( CORE_CFG_STATIC )  /* normally build as static library */
  #define CORE_API
  #define CORE_HIDDEN
#else
  #if defined( CORE_CFG_DLL )  /* we are building dll */
    #define CORE_API      CORE_DLL_EXPORT
    #define CORE_HIDDEN   CORE_DLL_HIDDEN
  #else                       /* call() and jump() when load as dll */
    #define CORE_API      CORE_DLL_IMPORT
    #define CORE_HIDDEN
  #endif
#endif

#endif   

#ifdef __cplusplus
	}
#endif


/* output the information */
#if defined( CORE_USE_QT5_INFO )
#  include <QDebug>
#  define CORE_DEBUG  QDebug
#  define CORE_INFO   qInfo
#  define CORE_FATAL  qFatal
#else
#  include <iostream>
#  include <exception>
#  include "core_logging.hxx"
#  define CORE_DEBUG  core_debug
#  define CORE_INFO   core_info
#  define CORE_FATAL  core_fatal
#endif

/* disable the copy in class */
#define CORE_DISABLE_COPY( t ) private : t ( const t & ); t & operator = ( const t & );

#if defined( core_ptrcast )
#  error "core_ptrcast already defined, conflicit ! Abort!"
#else
#  define core_ptrcast( t, o )  ( reinterpret_cast< t >( reinterpret_cast< intptr_t >( o ))  )
#endif

#if defined( core_intptrcast )
#  error "core_intptrcast already defined, conflicit! Abort!"
#else
#  define core_intptrcast( o )  ( reinterpret_cast< intptr_t >( o ))
#endif

#if defined( core_objcast )
#  error "core_objcast already defined, conflict! Abort!"
#else
#  define core_objcast( t, o )  ( static_cast< t >( o ) )
#endif

/* define the unsued macro */
#if !defined( CORE_UNUSED )
#define CORE_UNUSED( x ) ((void)x)
#endif

#endif
