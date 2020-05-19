#ifndef QMLPAGELAYOUTDEMO_DEF_H
#define QMLPAGELAYOUTDEMO_DEF_H

#include <stdint.h>
#include <float.h>

#ifdef __cplusplus
	extern "C" {
#endif


/* ////////////////////////////////////////////////////////////////////////////////
                                Export or Import
   ////////////////////////////////////////////////////////////////////////////  */
/* this helper definitions for shared libary support */
#if !defined( QMLPAGELAYOUTDEMO_SYM_EXPORT_DEF )
#define QMLPAGELAYOUTDEMO_SYM_EXPORT_DEF

#if defined( _WIN32 ) || defined( __CYGWIN__ )
  #define QMLPAGELAYOUTDEMO_DLL_IMPORT  __declspec(dllimport)
  #define QMLPAGELAYOUTDEMO_DLL_EXPORT  __declspec(dllexport)
  #define QMLPAGELAYOUTDEMO_DLL_HIDDEN
#else
  #if __GNUC__ >= 4
     #define QMLPAGELAYOUTDEMO_DLL_IMPORT __attribute__ ((visibility ("default")))
     #define QMLPAGELAYOUTDEMO_DLL_EXPORT __attribute__ ((visibility ("default")))
     #define QMLPAGELAYOUTDEMO_DLL_HIDDEN __attribute__ ((visibility ("hidden")))
  #else
     #define QMLPAGELAYOUTDEMO_DLL_IMPORT
     #define QMLPAGELAYOUTDEMO_DLL_EXPORT
     #define QMLPAGELAYOUTDEMO_DLL_HIDDEN
  #endif
#endif

/* check if defined QMLPAGELAYOUTDEMO_DLL */
#if defined( QMLPAGELAYOUTDEMO_CFG_STATIC )  && !defined( QMLPAGELAYOUTDEMO_CFG_STATIC )  && !defined( QMLPAGELAYOUTDEMO_CFG_DLL )
  #define QMLPAGELAYOUTDEMO_CFG_STATIC
#endif 
#if defined( QMLPAGELAYOUTDEMO_CFG_DLL ) && !defined( QMLPAGELAYOUTDEMO_CFG_STATIC )  &&  !defined( QMLPAGELAYOUTDEMO_CFG_DLL )
  #define QMLPAGELAYOUTDEMO_CFG_DLL
#endif

#undef QMLPAGELAYOUTDEMO_API
#undef QMLPAGELAYOUTDEMO_HIDDEN
#if defined( QMLPAGELAYOUTDEMO_CFG_STATIC )  /* normally build as static library */
  #define QMLPAGELAYOUTDEMO_API
  #define QMLPAGELAYOUTDEMO_HIDDEN
#else
  #if defined( QMLPAGELAYOUTDEMO_CFG_DLL )  /* we are building dll */
    #define QMLPAGELAYOUTDEMO_API      QMLPAGELAYOUTDEMO_DLL_EXPORT
    #define QMLPAGELAYOUTDEMO_HIDDEN   QMLPAGELAYOUTDEMO_DLL_HIDDEN
  #else                       /* call() and jump() when load as dll */
    #define QMLPAGELAYOUTDEMO_API      QMLPAGELAYOUTDEMO_DLL_IMPORT
    #define QMLPAGELAYOUTDEMO_HIDDEN
  #endif
#endif

#endif   

#ifdef __cplusplus
	}
#endif


/* output the information */
#if defined( QMLPAGELAYOUTDEMO_USE_QT5_INFO )
#  include <QDebug>
#  define QMLPAGELAYOUTDEMO_DEBUG  QDebug
#  define QMLPAGELAYOUTDEMO_INFO   qInfo
#  define QMLPAGELAYOUTDEMO_FATAL  qFatal
#else
#  include <iostream>
#  include <exception>
#  include "qmlpagelayoutdemo_logging.hxx"
#  define QMLPAGELAYOUTDEMO_DEBUG  qmlpagelayoutdemo_debug
#  define QMLPAGELAYOUTDEMO_INFO   qmlpagelayoutdemo_info
#  define QMLPAGELAYOUTDEMO_FATAL  qmlpagelayoutdemo_fatal
#endif

/* disable the copy in class */
#define QMLPAGELAYOUTDEMO_DISABLE_COPY( t ) private : t ( const t & ); t & operator = ( const t & );

#if defined( qmlpagelayoutdemo_ptrcast )
#  error "qmlpagelayoutdemo_ptrcast already defined, conflicit ! Abort!"
#else
#  define qmlpagelayoutdemo_ptrcast( t, o )  ( reinterpret_cast< t >( reinterpret_cast< intptr_t >( o ))  )
#endif

#if defined( qmlpagelayoutdemo_intptrcast )
#  error "qmlpagelayoutdemo_intptrcast already defined, conflicit! Abort!"
#else
#  define qmlpagelayoutdemo_intptrcast( o )  ( reinterpret_cast< intptr_t >( o ))
#endif

#if defined( qmlpagelayoutdemo_objcast )
#  error "qmlpagelayoutdemo_objcast already defined, conflict! Abort!"
#else
#  define qmlpagelayoutdemo_objcast( t, o )  ( static_cast< t >( o ) )
#endif

/* define the unsued macro */
#if !defined( QMLPAGELAYOUTDEMO_UNUSED )
#define QMLPAGELAYOUTDEMO_UNUSED( x ) ((void)x)
#endif

#endif
