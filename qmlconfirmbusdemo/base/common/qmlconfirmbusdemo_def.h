#ifndef QMLCONFIRMBUSDEMO_DEF_H
#define QMLCONFIRMBUSDEMO_DEF_H

#include <stdint.h>
#include <float.h>

#ifdef __cplusplus
	extern "C" {
#endif


/* ////////////////////////////////////////////////////////////////////////////////
                                Export or Import
   ////////////////////////////////////////////////////////////////////////////  */
/* this helper definitions for shared libary support */
#if !defined( QMLCONFIRMBUSDEMO_SYM_EXPORT_DEF )
#define QMLCONFIRMBUSDEMO_SYM_EXPORT_DEF

#if defined( _WIN32 ) || defined( __CYGWIN__ )
  #define QMLCONFIRMBUSDEMO_DLL_IMPORT  __declspec(dllimport)
  #define QMLCONFIRMBUSDEMO_DLL_EXPORT  __declspec(dllexport)
  #define QMLCONFIRMBUSDEMO_DLL_HIDDEN
#else
  #if __GNUC__ >= 4
     #define QMLCONFIRMBUSDEMO_DLL_IMPORT __attribute__ ((visibility ("default")))
     #define QMLCONFIRMBUSDEMO_DLL_EXPORT __attribute__ ((visibility ("default")))
     #define QMLCONFIRMBUSDEMO_DLL_HIDDEN __attribute__ ((visibility ("hidden")))
  #else
     #define QMLCONFIRMBUSDEMO_DLL_IMPORT
     #define QMLCONFIRMBUSDEMO_DLL_EXPORT
     #define QMLCONFIRMBUSDEMO_DLL_HIDDEN
  #endif
#endif

/* check if defined QMLCONFIRMBUSDEMO_DLL */
#if defined( QMLCONFIRMBUSDEMO_CFG_STATIC )  && !defined( QMLCONFIRMBUSDEMO_CFG_STATIC )  && !defined( QMLCONFIRMBUSDEMO_CFG_DLL )
  #define QMLCONFIRMBUSDEMO_CFG_STATIC
#endif 
#if defined( QMLCONFIRMBUSDEMO_CFG_DLL ) && !defined( QMLCONFIRMBUSDEMO_CFG_STATIC )  &&  !defined( QMLCONFIRMBUSDEMO_CFG_DLL )
  #define QMLCONFIRMBUSDEMO_CFG_DLL
#endif

#undef QMLCONFIRMBUSDEMO_API
#undef QMLCONFIRMBUSDEMO_HIDDEN
#if defined( QMLCONFIRMBUSDEMO_CFG_STATIC )  /* normally build as static library */
  #define QMLCONFIRMBUSDEMO_API
  #define QMLCONFIRMBUSDEMO_HIDDEN
#else
  #if defined( QMLCONFIRMBUSDEMO_CFG_DLL )  /* we are building dll */
    #define QMLCONFIRMBUSDEMO_API      QMLCONFIRMBUSDEMO_DLL_EXPORT
    #define QMLCONFIRMBUSDEMO_HIDDEN   QMLCONFIRMBUSDEMO_DLL_HIDDEN
  #else                       /* call() and jump() when load as dll */
    #define QMLCONFIRMBUSDEMO_API      QMLCONFIRMBUSDEMO_DLL_IMPORT
    #define QMLCONFIRMBUSDEMO_HIDDEN
  #endif
#endif

#endif   

#ifdef __cplusplus
	}
#endif


/* output the information */
#if defined( QMLCONFIRMBUSDEMO_USE_QT5_INFO )
#  include <QDebug>
#  define QMLCONFIRMBUSDEMO_DEBUG  QDebug
#  define QMLCONFIRMBUSDEMO_INFO   qInfo
#  define QMLCONFIRMBUSDEMO_FATAL  qFatal
#else
#  include <iostream>
#  include <exception>
#  include "qmlconfirmbusdemo_logging.hxx"
#  define QMLCONFIRMBUSDEMO_DEBUG  qmlconfirmbusdemo_debug
#  define QMLCONFIRMBUSDEMO_INFO   qmlconfirmbusdemo_info
#  define QMLCONFIRMBUSDEMO_FATAL  qmlconfirmbusdemo_fatal
#endif

/* disable the copy in class */
#define QMLCONFIRMBUSDEMO_DISABLE_COPY( t ) private : t ( const t & ); t & operator = ( const t & );

#if defined( qmlconfirmbusdemo_ptrcast )
#  error "qmlconfirmbusdemo_ptrcast already defined, conflicit ! Abort!"
#else
#  define qmlconfirmbusdemo_ptrcast( t, o )  ( reinterpret_cast< t >( reinterpret_cast< intptr_t >( o ))  )
#endif

#if defined( qmlconfirmbusdemo_intptrcast )
#  error "qmlconfirmbusdemo_intptrcast already defined, conflicit! Abort!"
#else
#  define qmlconfirmbusdemo_intptrcast( o )  ( reinterpret_cast< intptr_t >( o ))
#endif

#if defined( qmlconfirmbusdemo_objcast )
#  error "qmlconfirmbusdemo_objcast already defined, conflict! Abort!"
#else
#  define qmlconfirmbusdemo_objcast( t, o )  ( static_cast< t >( o ) )
#endif

/* define the unsued macro */
#if !defined( QMLCONFIRMBUSDEMO_UNUSED )
#define QMLCONFIRMBUSDEMO_UNUSED( x ) ((void)x)
#endif

#endif
