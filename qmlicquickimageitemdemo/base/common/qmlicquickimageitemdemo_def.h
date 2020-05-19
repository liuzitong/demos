#ifndef QMLICQUICKIMAGEITEMDEMO_DEF_H
#define QMLICQUICKIMAGEITEMDEMO_DEF_H

#include <stdint.h>
#include <float.h>

#ifdef __cplusplus
	extern "C" {
#endif


/* ////////////////////////////////////////////////////////////////////////////////
                                Export or Import
   ////////////////////////////////////////////////////////////////////////////  */
/* this helper definitions for shared libary support */
#if !defined( QMLICQUICKIMAGEITEMDEMO_SYM_EXPORT_DEF )
#define QMLICQUICKIMAGEITEMDEMO_SYM_EXPORT_DEF

#if defined( _WIN32 ) || defined( __CYGWIN__ )
  #define QMLICQUICKIMAGEITEMDEMO_DLL_IMPORT  __declspec(dllimport)
  #define QMLICQUICKIMAGEITEMDEMO_DLL_EXPORT  __declspec(dllexport)
  #define QMLICQUICKIMAGEITEMDEMO_DLL_HIDDEN
#else
  #if __GNUC__ >= 4
     #define QMLICQUICKIMAGEITEMDEMO_DLL_IMPORT __attribute__ ((visibility ("default")))
     #define QMLICQUICKIMAGEITEMDEMO_DLL_EXPORT __attribute__ ((visibility ("default")))
     #define QMLICQUICKIMAGEITEMDEMO_DLL_HIDDEN __attribute__ ((visibility ("hidden")))
  #else
     #define QMLICQUICKIMAGEITEMDEMO_DLL_IMPORT
     #define QMLICQUICKIMAGEITEMDEMO_DLL_EXPORT
     #define QMLICQUICKIMAGEITEMDEMO_DLL_HIDDEN
  #endif
#endif

/* check if defined QMLICQUICKIMAGEITEMDEMO_DLL */
#if defined( QMLICQUICKIMAGEITEMDEMO_CFG_STATIC )  && !defined( QMLICQUICKIMAGEITEMDEMO_CFG_STATIC )  && !defined( QMLICQUICKIMAGEITEMDEMO_CFG_DLL )
  #define QMLICQUICKIMAGEITEMDEMO_CFG_STATIC
#endif 
#if defined( QMLICQUICKIMAGEITEMDEMO_CFG_DLL ) && !defined( QMLICQUICKIMAGEITEMDEMO_CFG_STATIC )  &&  !defined( QMLICQUICKIMAGEITEMDEMO_CFG_DLL )
  #define QMLICQUICKIMAGEITEMDEMO_CFG_DLL
#endif

#undef QMLICQUICKIMAGEITEMDEMO_API
#undef QMLICQUICKIMAGEITEMDEMO_HIDDEN
#if defined( QMLICQUICKIMAGEITEMDEMO_CFG_STATIC )  /* normally build as static library */
  #define QMLICQUICKIMAGEITEMDEMO_API
  #define QMLICQUICKIMAGEITEMDEMO_HIDDEN
#else
  #if defined( QMLICQUICKIMAGEITEMDEMO_CFG_DLL )  /* we are building dll */
    #define QMLICQUICKIMAGEITEMDEMO_API      QMLICQUICKIMAGEITEMDEMO_DLL_EXPORT
    #define QMLICQUICKIMAGEITEMDEMO_HIDDEN   QMLICQUICKIMAGEITEMDEMO_DLL_HIDDEN
  #else                       /* call() and jump() when load as dll */
    #define QMLICQUICKIMAGEITEMDEMO_API      QMLICQUICKIMAGEITEMDEMO_DLL_IMPORT
    #define QMLICQUICKIMAGEITEMDEMO_HIDDEN
  #endif
#endif

#endif   

#ifdef __cplusplus
	}
#endif


/* output the information */
#if defined( QMLICQUICKIMAGEITEMDEMO_USE_QT5_INFO )
#  include <QDebug>
#  define QMLICQUICKIMAGEITEMDEMO_DEBUG  QDebug
#  define QMLICQUICKIMAGEITEMDEMO_INFO   qInfo
#  define QMLICQUICKIMAGEITEMDEMO_FATAL  qFatal
#else
#  include <iostream>
#  include <exception>
#  include "qmlicquickimageitemdemo_logging.hxx"
#  define QMLICQUICKIMAGEITEMDEMO_DEBUG  qmlicquickimageitemdemo_debug
#  define QMLICQUICKIMAGEITEMDEMO_INFO   qmlicquickimageitemdemo_info
#  define QMLICQUICKIMAGEITEMDEMO_FATAL  qmlicquickimageitemdemo_fatal
#endif

/* disable the copy in class */
#define QMLICQUICKIMAGEITEMDEMO_DISABLE_COPY( t ) private : t ( const t & ); t & operator = ( const t & );

#if defined( qmlicquickimageitemdemo_ptrcast )
#  error "qmlicquickimageitemdemo_ptrcast already defined, conflicit ! Abort!"
#else
#  define qmlicquickimageitemdemo_ptrcast( t, o )  ( reinterpret_cast< t >( reinterpret_cast< intptr_t >( o ))  )
#endif

#if defined( qmlicquickimageitemdemo_intptrcast )
#  error "qmlicquickimageitemdemo_intptrcast already defined, conflicit! Abort!"
#else
#  define qmlicquickimageitemdemo_intptrcast( o )  ( reinterpret_cast< intptr_t >( o ))
#endif

#if defined( qmlicquickimageitemdemo_objcast )
#  error "qmlicquickimageitemdemo_objcast already defined, conflict! Abort!"
#else
#  define qmlicquickimageitemdemo_objcast( t, o )  ( static_cast< t >( o ) )
#endif

/* define the unsued macro */
#if !defined( QMLICQUICKIMAGEITEMDEMO_UNUSED )
#define QMLICQUICKIMAGEITEMDEMO_UNUSED( x ) ((void)x)
#endif

#endif
