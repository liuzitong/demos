#ifndef QMLBASE_DEF_H
#define QMLBASE_DEF_H

#include <stdint.h>
#include <float.h>

#ifdef __cplusplus
	extern "C" {
#endif


/* ////////////////////////////////////////////////////////////////////////////////
                                Export or Import
   ////////////////////////////////////////////////////////////////////////////  */
/* this helper definitions for shared libary support */
#if !defined( QMLBASE_SYM_EXPORT_DEF )
#define QMLBASE_SYM_EXPORT_DEF

#if defined( _WIN32 ) || defined( __CYGWIN__ )
  #define QMLBASE_DLL_IMPORT  __declspec(dllimport)
  #define QMLBASE_DLL_EXPORT  __declspec(dllexport)
  #define QMLBASE_DLL_HIDDEN
#else
  #if __GNUC__ >= 4
     #define QMLBASE_DLL_IMPORT __attribute__ ((visibility ("default")))
     #define QMLBASE_DLL_EXPORT __attribute__ ((visibility ("default")))
     #define QMLBASE_DLL_HIDDEN __attribute__ ((visibility ("hidden")))
  #else
     #define QMLBASE_DLL_IMPORT
     #define QMLBASE_DLL_EXPORT
     #define QMLBASE_DLL_HIDDEN
  #endif
#endif

/* check if defined QMLBASE_DLL */
#if defined( QMLBASE_CFG_STATIC )  && !defined( QMLBASE_CFG_STATIC )  && !defined( QMLBASE_CFG_DLL )
  #define QMLBASE_CFG_STATIC
#endif 
#if defined( QMLBASE_CFG_DLL ) && !defined( QMLBASE_CFG_STATIC )  &&  !defined( QMLBASE_CFG_DLL )
  #define QMLBASE_CFG_DLL
#endif

#undef QMLBASE_API
#undef QMLBASE_HIDDEN
#if defined( QMLBASE_CFG_STATIC )  /* normally build as static library */
  #define QMLBASE_API
  #define QMLBASE_HIDDEN
#else
  #if defined( QMLBASE_CFG_DLL )  /* we are building dll */
    #define QMLBASE_API      QMLBASE_DLL_EXPORT
    #define QMLBASE_HIDDEN   QMLBASE_DLL_HIDDEN
  #else                       /* call() and jump() when load as dll */
    #define QMLBASE_API      QMLBASE_DLL_IMPORT
    #define QMLBASE_HIDDEN
  #endif
#endif

#endif   

#ifdef __cplusplus
	}
#endif


/* output the information */
#if defined( QMLBASE_USE_QT5_INFO )
#  include <QDebug>
#  define QMLBASE_DEBUG  QDebug
#  define QMLBASE_INFO   qInfo
#  define QMLBASE_FATAL  qFatal
#else
#  include <iostream>
#  include <exception>
#  include "qmlbase_logging.hxx"
#  define QMLBASE_DEBUG  qmlbase_debug
#  define QMLBASE_INFO   qmlbase_info
#  define QMLBASE_FATAL  qmlbase_fatal
#endif

/* disable the copy in class */
#define QMLBASE_DISABLE_COPY( t ) private : t ( const t & ); t & operator = ( const t & );

#if defined( qmlbase_ptrcast )
#  error "qmlbase_ptrcast already defined, conflicit ! Abort!"
#else
#  define qmlbase_ptrcast( t, o )  ( reinterpret_cast< t >( reinterpret_cast< intptr_t >( o ))  )
#endif

#if defined( qmlbase_intptrcast )
#  error "qmlbase_intptrcast already defined, conflicit! Abort!"
#else
#  define qmlbase_intptrcast( o )  ( reinterpret_cast< intptr_t >( o ))
#endif

#if defined( qmlbase_objcast )
#  error "qmlbase_objcast already defined, conflict! Abort!"
#else
#  define qmlbase_objcast( t, o )  ( static_cast< t >( o ) )
#endif

/* define the unsued macro */
#if !defined( QMLBASE_UNUSED )
#define QMLBASE_UNUSED( x ) ((void)x)
#endif

#endif
