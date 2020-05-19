#ifndef QMLMSGBUSMULTIWINDEMO_DEF_H
#define QMLMSGBUSMULTIWINDEMO_DEF_H

#include <stdint.h>
#include <float.h>

#ifdef __cplusplus
	extern "C" {
#endif


/* ////////////////////////////////////////////////////////////////////////////////
                                Export or Import
   ////////////////////////////////////////////////////////////////////////////  */
/* this helper definitions for shared libary support */
#if !defined( QMLMSGBUSMULTIWINDEMO_SYM_EXPORT_DEF )
#define QMLMSGBUSMULTIWINDEMO_SYM_EXPORT_DEF

#if defined( _WIN32 ) || defined( __CYGWIN__ )
  #define QMLMSGBUSMULTIWINDEMO_DLL_IMPORT  __declspec(dllimport)
  #define QMLMSGBUSMULTIWINDEMO_DLL_EXPORT  __declspec(dllexport)
  #define QMLMSGBUSMULTIWINDEMO_DLL_HIDDEN
#else
  #if __GNUC__ >= 4
     #define QMLMSGBUSMULTIWINDEMO_DLL_IMPORT __attribute__ ((visibility ("default")))
     #define QMLMSGBUSMULTIWINDEMO_DLL_EXPORT __attribute__ ((visibility ("default")))
     #define QMLMSGBUSMULTIWINDEMO_DLL_HIDDEN __attribute__ ((visibility ("hidden")))
  #else
     #define QMLMSGBUSMULTIWINDEMO_DLL_IMPORT
     #define QMLMSGBUSMULTIWINDEMO_DLL_EXPORT
     #define QMLMSGBUSMULTIWINDEMO_DLL_HIDDEN
  #endif
#endif

/* check if defined QMLMSGBUSMULTIWINDEMO_DLL */
#if defined( QMLMSGBUSMULTIWINDEMO_CFG_STATIC )  && !defined( QMLMSGBUSMULTIWINDEMO_CFG_STATIC )  && !defined( QMLMSGBUSMULTIWINDEMO_CFG_DLL )
  #define QMLMSGBUSMULTIWINDEMO_CFG_STATIC
#endif 
#if defined( QMLMSGBUSMULTIWINDEMO_CFG_DLL ) && !defined( QMLMSGBUSMULTIWINDEMO_CFG_STATIC )  &&  !defined( QMLMSGBUSMULTIWINDEMO_CFG_DLL )
  #define QMLMSGBUSMULTIWINDEMO_CFG_DLL
#endif

#undef QMLMSGBUSMULTIWINDEMO_API
#undef QMLMSGBUSMULTIWINDEMO_HIDDEN
#if defined( QMLMSGBUSMULTIWINDEMO_CFG_STATIC )  /* normally build as static library */
  #define QMLMSGBUSMULTIWINDEMO_API
  #define QMLMSGBUSMULTIWINDEMO_HIDDEN
#else
  #if defined( QMLMSGBUSMULTIWINDEMO_CFG_DLL )  /* we are building dll */
    #define QMLMSGBUSMULTIWINDEMO_API      QMLMSGBUSMULTIWINDEMO_DLL_EXPORT
    #define QMLMSGBUSMULTIWINDEMO_HIDDEN   QMLMSGBUSMULTIWINDEMO_DLL_HIDDEN
  #else                       /* call() and jump() when load as dll */
    #define QMLMSGBUSMULTIWINDEMO_API      QMLMSGBUSMULTIWINDEMO_DLL_IMPORT
    #define QMLMSGBUSMULTIWINDEMO_HIDDEN
  #endif
#endif

#endif   

#ifdef __cplusplus
	}
#endif


/* output the information */
#if defined( QMLMSGBUSMULTIWINDEMO_USE_QT5_INFO )
#  include <QDebug>
#  define QMLMSGBUSMULTIWINDEMO_DEBUG  QDebug
#  define QMLMSGBUSMULTIWINDEMO_INFO   qInfo
#  define QMLMSGBUSMULTIWINDEMO_FATAL  qFatal
#else
#  include <iostream>
#  include <exception>
#  include "qmlmsgbusmultiwindemo_logging.hxx"
#  define QMLMSGBUSMULTIWINDEMO_DEBUG  qmlmsgbusmultiwindemo_debug
#  define QMLMSGBUSMULTIWINDEMO_INFO   qmlmsgbusmultiwindemo_info
#  define QMLMSGBUSMULTIWINDEMO_FATAL  qmlmsgbusmultiwindemo_fatal
#endif

/* disable the copy in class */
#define QMLMSGBUSMULTIWINDEMO_DISABLE_COPY( t ) private : t ( const t & ); t & operator = ( const t & );

#if defined( qmlmsgbusmultiwindemo_ptrcast )
#  error "qmlmsgbusmultiwindemo_ptrcast already defined, conflicit ! Abort!"
#else
#  define qmlmsgbusmultiwindemo_ptrcast( t, o )  ( reinterpret_cast< t >( reinterpret_cast< intptr_t >( o ))  )
#endif

#if defined( qmlmsgbusmultiwindemo_intptrcast )
#  error "qmlmsgbusmultiwindemo_intptrcast already defined, conflicit! Abort!"
#else
#  define qmlmsgbusmultiwindemo_intptrcast( o )  ( reinterpret_cast< intptr_t >( o ))
#endif

#if defined( qmlmsgbusmultiwindemo_objcast )
#  error "qmlmsgbusmultiwindemo_objcast already defined, conflict! Abort!"
#else
#  define qmlmsgbusmultiwindemo_objcast( t, o )  ( static_cast< t >( o ) )
#endif

/* define the unsued macro */
#if !defined( QMLMSGBUSMULTIWINDEMO_UNUSED )
#define QMLMSGBUSMULTIWINDEMO_UNUSED( x ) ((void)x)
#endif

#endif
