#ifndef LIBANDQMLTESTDEMO_DEF_H
#define LIBANDQMLTESTDEMO_DEF_H

#include <stdint.h>
#include <float.h>

#ifdef __cplusplus
	extern "C" {
#endif


/* ////////////////////////////////////////////////////////////////////////////////
                                Export or Import
   ////////////////////////////////////////////////////////////////////////////  */
/* this helper definitions for shared libary support */
#if !defined( LIBANDQMLTESTDEMO_SYM_EXPORT_DEF )
#define LIBANDQMLTESTDEMO_SYM_EXPORT_DEF

#if defined( _WIN32 ) || defined( __CYGWIN__ )
  #define LIBANDQMLTESTDEMO_DLL_IMPORT  __declspec(dllimport)
  #define LIBANDQMLTESTDEMO_DLL_EXPORT  __declspec(dllexport)
  #define LIBANDQMLTESTDEMO_DLL_HIDDEN
#else
  #if __GNUC__ >= 4
     #define LIBANDQMLTESTDEMO_DLL_IMPORT __attribute__ ((visibility ("default")))
     #define LIBANDQMLTESTDEMO_DLL_EXPORT __attribute__ ((visibility ("default")))
     #define LIBANDQMLTESTDEMO_DLL_HIDDEN __attribute__ ((visibility ("hidden")))
  #else
     #define LIBANDQMLTESTDEMO_DLL_IMPORT
     #define LIBANDQMLTESTDEMO_DLL_EXPORT
     #define LIBANDQMLTESTDEMO_DLL_HIDDEN
  #endif
#endif

/* check if defined LIBANDQMLTESTDEMO_DLL */
#if defined( LIBANDQMLTESTDEMO_CFG_STATIC )  && !defined( LIBANDQMLTESTDEMO_CFG_STATIC )  && !defined( LIBANDQMLTESTDEMO_CFG_DLL )
  #define LIBANDQMLTESTDEMO_CFG_STATIC
#endif 
#if defined( LIBANDQMLTESTDEMO_CFG_DLL ) && !defined( LIBANDQMLTESTDEMO_CFG_STATIC )  &&  !defined( LIBANDQMLTESTDEMO_CFG_DLL )
  #define LIBANDQMLTESTDEMO_CFG_DLL
#endif

#undef LIBANDQMLTESTDEMO_API
#undef LIBANDQMLTESTDEMO_HIDDEN
#if defined( LIBANDQMLTESTDEMO_CFG_STATIC )  /* normally build as static library */
  #define LIBANDQMLTESTDEMO_API
  #define LIBANDQMLTESTDEMO_HIDDEN
#else
  #if defined( LIBANDQMLTESTDEMO_CFG_DLL )  /* we are building dll */
    #define LIBANDQMLTESTDEMO_API      LIBANDQMLTESTDEMO_DLL_EXPORT
    #define LIBANDQMLTESTDEMO_HIDDEN   LIBANDQMLTESTDEMO_DLL_HIDDEN
  #else                       /* call() and jump() when load as dll */
    #define LIBANDQMLTESTDEMO_API      LIBANDQMLTESTDEMO_DLL_IMPORT
    #define LIBANDQMLTESTDEMO_HIDDEN
  #endif
#endif

#endif   

#ifdef __cplusplus
	}
#endif


/* output the information */
#if defined( LIBANDQMLTESTDEMO_USE_QT5_INFO )
#  include <QDebug>
#  define LIBANDQMLTESTDEMO_DEBUG  QDebug
#  define LIBANDQMLTESTDEMO_INFO   qInfo
#  define LIBANDQMLTESTDEMO_FATAL  qFatal
#else
#  include <iostream>
#  include <exception>
#  include "libandqmltestdemo_logging.hxx"
#  define LIBANDQMLTESTDEMO_DEBUG  libandqmltestdemo_debug
#  define LIBANDQMLTESTDEMO_INFO   libandqmltestdemo_info
#  define LIBANDQMLTESTDEMO_FATAL  libandqmltestdemo_fatal
#endif

/* disable the copy in class */
#define LIBANDQMLTESTDEMO_DISABLE_COPY( t ) private : t ( const t & ); t & operator = ( const t & );

#if defined( libandqmltestdemo_ptrcast )
#  error "libandqmltestdemo_ptrcast already defined, conflicit ! Abort!"
#else
#  define libandqmltestdemo_ptrcast( t, o )  ( reinterpret_cast< t >( reinterpret_cast< intptr_t >( o ))  )
#endif

#if defined( libandqmltestdemo_intptrcast )
#  error "libandqmltestdemo_intptrcast already defined, conflicit! Abort!"
#else
#  define libandqmltestdemo_intptrcast( o )  ( reinterpret_cast< intptr_t >( o ))
#endif

#if defined( libandqmltestdemo_objcast )
#  error "libandqmltestdemo_objcast already defined, conflict! Abort!"
#else
#  define libandqmltestdemo_objcast( t, o )  ( static_cast< t >( o ) )
#endif

/* define the unsued macro */
#if !defined( LIBANDQMLTESTDEMO_UNUSED )
#define LIBANDQMLTESTDEMO_UNUSED( x ) ((void)x)
#endif

#endif
