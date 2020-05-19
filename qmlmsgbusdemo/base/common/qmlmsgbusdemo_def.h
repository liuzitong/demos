#ifndef QMLMSGBUSDEMO_DEF_H
#define QMLMSGBUSDEMO_DEF_H

#include <stdint.h>
#include <float.h>

#ifdef __cplusplus
	extern "C" {
#endif


/* ////////////////////////////////////////////////////////////////////////////////
                                Export or Import
   ////////////////////////////////////////////////////////////////////////////  */
/* this helper definitions for shared libary support */
#if !defined( QMLMSGBUSDEMO_SYM_EXPORT_DEF )
#define QMLMSGBUSDEMO_SYM_EXPORT_DEF

#if defined( _WIN32 ) || defined( __CYGWIN__ )
  #define QMLMSGBUSDEMO_DLL_IMPORT  __declspec(dllimport)
  #define QMLMSGBUSDEMO_DLL_EXPORT  __declspec(dllexport)
  #define QMLMSGBUSDEMO_DLL_HIDDEN
#else
  #if __GNUC__ >= 4
     #define QMLMSGBUSDEMO_DLL_IMPORT __attribute__ ((visibility ("default")))
     #define QMLMSGBUSDEMO_DLL_EXPORT __attribute__ ((visibility ("default")))
     #define QMLMSGBUSDEMO_DLL_HIDDEN __attribute__ ((visibility ("hidden")))
  #else
     #define QMLMSGBUSDEMO_DLL_IMPORT
     #define QMLMSGBUSDEMO_DLL_EXPORT
     #define QMLMSGBUSDEMO_DLL_HIDDEN
  #endif
#endif

/* check if defined QMLMSGBUSDEMO_DLL */
#if defined( QMLMSGBUSDEMO_CFG_STATIC )  && !defined( QMLMSGBUSDEMO_CFG_STATIC )  && !defined( QMLMSGBUSDEMO_CFG_DLL )
  #define QMLMSGBUSDEMO_CFG_STATIC
#endif 
#if defined( QMLMSGBUSDEMO_CFG_DLL ) && !defined( QMLMSGBUSDEMO_CFG_STATIC )  &&  !defined( QMLMSGBUSDEMO_CFG_DLL )
  #define QMLMSGBUSDEMO_CFG_DLL
#endif

#undef QMLMSGBUSDEMO_API
#undef QMLMSGBUSDEMO_HIDDEN
#if defined( QMLMSGBUSDEMO_CFG_STATIC )  /* normally build as static library */
  #define QMLMSGBUSDEMO_API
  #define QMLMSGBUSDEMO_HIDDEN
#else
  #if defined( QMLMSGBUSDEMO_CFG_DLL )  /* we are building dll */
    #define QMLMSGBUSDEMO_API      QMLMSGBUSDEMO_DLL_EXPORT
    #define QMLMSGBUSDEMO_HIDDEN   QMLMSGBUSDEMO_DLL_HIDDEN
  #else                       /* call() and jump() when load as dll */
    #define QMLMSGBUSDEMO_API      QMLMSGBUSDEMO_DLL_IMPORT
    #define QMLMSGBUSDEMO_HIDDEN
  #endif
#endif

#endif   

#ifdef __cplusplus
	}
#endif


/* output the information */
#if defined( QMLMSGBUSDEMO_USE_QT5_INFO )
#  include <QDebug>
#  define QMLMSGBUSDEMO_DEBUG  QDebug
#  define QMLMSGBUSDEMO_INFO   qInfo
#  define QMLMSGBUSDEMO_FATAL  qFatal
#else
#  include <iostream>
#  include <exception>
#  include "qmlmsgbusdemo_logging.hxx"
#  define QMLMSGBUSDEMO_DEBUG  qmlmsgbusdemo_debug
#  define QMLMSGBUSDEMO_INFO   qmlmsgbusdemo_info
#  define QMLMSGBUSDEMO_FATAL  qmlmsgbusdemo_fatal
#endif

/* disable the copy in class */
#define QMLMSGBUSDEMO_DISABLE_COPY( t ) private : t ( const t & ); t & operator = ( const t & );

#if defined( qmlmsgbusdemo_ptrcast )
#  error "qmlmsgbusdemo_ptrcast already defined, conflicit ! Abort!"
#else
#  define qmlmsgbusdemo_ptrcast( t, o )  ( reinterpret_cast< t >( reinterpret_cast< intptr_t >( o ))  )
#endif

#if defined( qmlmsgbusdemo_intptrcast )
#  error "qmlmsgbusdemo_intptrcast already defined, conflicit! Abort!"
#else
#  define qmlmsgbusdemo_intptrcast( o )  ( reinterpret_cast< intptr_t >( o ))
#endif

#if defined( qmlmsgbusdemo_objcast )
#  error "qmlmsgbusdemo_objcast already defined, conflict! Abort!"
#else
#  define qmlmsgbusdemo_objcast( t, o )  ( static_cast< t >( o ) )
#endif

/* define the unsued macro */
#if !defined( QMLMSGBUSDEMO_UNUSED )
#define QMLMSGBUSDEMO_UNUSED( x ) ((void)x)
#endif

#endif
