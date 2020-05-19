#ifndef QMLUSERLISTMODELDEMO_DEF_H
#define QMLUSERLISTMODELDEMO_DEF_H

#include <stdint.h>
#include <float.h>

#ifdef __cplusplus
	extern "C" {
#endif


/* ////////////////////////////////////////////////////////////////////////////////
                                Export or Import
   ////////////////////////////////////////////////////////////////////////////  */
/* this helper definitions for shared libary support */
#if !defined( QMLUSERLISTMODELDEMO_SYM_EXPORT_DEF )
#define QMLUSERLISTMODELDEMO_SYM_EXPORT_DEF

#if defined( _WIN32 ) || defined( __CYGWIN__ )
  #define QMLUSERLISTMODELDEMO_DLL_IMPORT  __declspec(dllimport)
  #define QMLUSERLISTMODELDEMO_DLL_EXPORT  __declspec(dllexport)
  #define QMLUSERLISTMODELDEMO_DLL_HIDDEN
#else
  #if __GNUC__ >= 4
     #define QMLUSERLISTMODELDEMO_DLL_IMPORT __attribute__ ((visibility ("default")))
     #define QMLUSERLISTMODELDEMO_DLL_EXPORT __attribute__ ((visibility ("default")))
     #define QMLUSERLISTMODELDEMO_DLL_HIDDEN __attribute__ ((visibility ("hidden")))
  #else
     #define QMLUSERLISTMODELDEMO_DLL_IMPORT
     #define QMLUSERLISTMODELDEMO_DLL_EXPORT
     #define QMLUSERLISTMODELDEMO_DLL_HIDDEN
  #endif
#endif

/* check if defined QMLUSERLISTMODELDEMO_DLL */
#if defined( QMLUSERLISTMODELDEMO_CFG_STATIC )  && !defined( QMLUSERLISTMODELDEMO_CFG_STATIC )  && !defined( QMLUSERLISTMODELDEMO_CFG_DLL )
  #define QMLUSERLISTMODELDEMO_CFG_STATIC
#endif 
#if defined( QMLUSERLISTMODELDEMO_CFG_DLL ) && !defined( QMLUSERLISTMODELDEMO_CFG_STATIC )  &&  !defined( QMLUSERLISTMODELDEMO_CFG_DLL )
  #define QMLUSERLISTMODELDEMO_CFG_DLL
#endif

#undef QMLUSERLISTMODELDEMO_API
#undef QMLUSERLISTMODELDEMO_HIDDEN
#if defined( QMLUSERLISTMODELDEMO_CFG_STATIC )  /* normally build as static library */
  #define QMLUSERLISTMODELDEMO_API
  #define QMLUSERLISTMODELDEMO_HIDDEN
#else
  #if defined( QMLUSERLISTMODELDEMO_CFG_DLL )  /* we are building dll */
    #define QMLUSERLISTMODELDEMO_API      QMLUSERLISTMODELDEMO_DLL_EXPORT
    #define QMLUSERLISTMODELDEMO_HIDDEN   QMLUSERLISTMODELDEMO_DLL_HIDDEN
  #else                       /* call() and jump() when load as dll */
    #define QMLUSERLISTMODELDEMO_API      QMLUSERLISTMODELDEMO_DLL_IMPORT
    #define QMLUSERLISTMODELDEMO_HIDDEN
  #endif
#endif

#endif   

#ifdef __cplusplus
	}
#endif


/* output the information */
#if defined( QMLUSERLISTMODELDEMO_USE_QT5_INFO )
#  include <QDebug>
#  define QMLUSERLISTMODELDEMO_DEBUG  QDebug
#  define QMLUSERLISTMODELDEMO_INFO   qInfo
#  define QMLUSERLISTMODELDEMO_FATAL  qFatal
#else
#  include <iostream>
#  include <exception>
#  include "qmluserlistmodeldemo_logging.hxx"
#  define QMLUSERLISTMODELDEMO_DEBUG  qmluserlistmodeldemo_debug
#  define QMLUSERLISTMODELDEMO_INFO   qmluserlistmodeldemo_info
#  define QMLUSERLISTMODELDEMO_FATAL  qmluserlistmodeldemo_fatal
#endif

/* disable the copy in class */
#define QMLUSERLISTMODELDEMO_DISABLE_COPY( t ) private : t ( const t & ); t & operator = ( const t & );

#if defined( qmluserlistmodeldemo_ptrcast )
#  error "qmluserlistmodeldemo_ptrcast already defined, conflicit ! Abort!"
#else
#  define qmluserlistmodeldemo_ptrcast( t, o )  ( reinterpret_cast< t >( reinterpret_cast< intptr_t >( o ))  )
#endif

#if defined( qmluserlistmodeldemo_intptrcast )
#  error "qmluserlistmodeldemo_intptrcast already defined, conflicit! Abort!"
#else
#  define qmluserlistmodeldemo_intptrcast( o )  ( reinterpret_cast< intptr_t >( o ))
#endif

#if defined( qmluserlistmodeldemo_objcast )
#  error "qmluserlistmodeldemo_objcast already defined, conflict! Abort!"
#else
#  define qmluserlistmodeldemo_objcast( t, o )  ( static_cast< t >( o ) )
#endif

/* define the unsued macro */
#if !defined( QMLUSERLISTMODELDEMO_UNUSED )
#define QMLUSERLISTMODELDEMO_UNUSED( x ) ((void)x)
#endif

#endif
