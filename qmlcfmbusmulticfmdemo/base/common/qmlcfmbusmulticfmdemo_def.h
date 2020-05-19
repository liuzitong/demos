#ifndef QMLCFMBUSMULTICFMDEMO_DEF_H
#define QMLCFMBUSMULTICFMDEMO_DEF_H

#include <stdint.h>
#include <float.h>

#ifdef __cplusplus
	extern "C" {
#endif


/* ////////////////////////////////////////////////////////////////////////////////
                                Export or Import
   ////////////////////////////////////////////////////////////////////////////  */
/* this helper definitions for shared libary support */
#if !defined( QMLCFMBUSMULTICFMDEMO_SYM_EXPORT_DEF )
#define QMLCFMBUSMULTICFMDEMO_SYM_EXPORT_DEF

#if defined( _WIN32 ) || defined( __CYGWIN__ )
  #define QMLCFMBUSMULTICFMDEMO_DLL_IMPORT  __declspec(dllimport)
  #define QMLCFMBUSMULTICFMDEMO_DLL_EXPORT  __declspec(dllexport)
  #define QMLCFMBUSMULTICFMDEMO_DLL_HIDDEN
#else
  #if __GNUC__ >= 4
     #define QMLCFMBUSMULTICFMDEMO_DLL_IMPORT __attribute__ ((visibility ("default")))
     #define QMLCFMBUSMULTICFMDEMO_DLL_EXPORT __attribute__ ((visibility ("default")))
     #define QMLCFMBUSMULTICFMDEMO_DLL_HIDDEN __attribute__ ((visibility ("hidden")))
  #else
     #define QMLCFMBUSMULTICFMDEMO_DLL_IMPORT
     #define QMLCFMBUSMULTICFMDEMO_DLL_EXPORT
     #define QMLCFMBUSMULTICFMDEMO_DLL_HIDDEN
  #endif
#endif

/* check if defined QMLCFMBUSMULTICFMDEMO_DLL */
#if defined( QMLCFMBUSMULTICFMDEMO_CFG_STATIC )  && !defined( QMLCFMBUSMULTICFMDEMO_CFG_STATIC )  && !defined( QMLCFMBUSMULTICFMDEMO_CFG_DLL )
  #define QMLCFMBUSMULTICFMDEMO_CFG_STATIC
#endif 
#if defined( QMLCFMBUSMULTICFMDEMO_CFG_DLL ) && !defined( QMLCFMBUSMULTICFMDEMO_CFG_STATIC )  &&  !defined( QMLCFMBUSMULTICFMDEMO_CFG_DLL )
  #define QMLCFMBUSMULTICFMDEMO_CFG_DLL
#endif

#undef QMLCFMBUSMULTICFMDEMO_API
#undef QMLCFMBUSMULTICFMDEMO_HIDDEN
#if defined( QMLCFMBUSMULTICFMDEMO_CFG_STATIC )  /* normally build as static library */
  #define QMLCFMBUSMULTICFMDEMO_API
  #define QMLCFMBUSMULTICFMDEMO_HIDDEN
#else
  #if defined( QMLCFMBUSMULTICFMDEMO_CFG_DLL )  /* we are building dll */
    #define QMLCFMBUSMULTICFMDEMO_API      QMLCFMBUSMULTICFMDEMO_DLL_EXPORT
    #define QMLCFMBUSMULTICFMDEMO_HIDDEN   QMLCFMBUSMULTICFMDEMO_DLL_HIDDEN
  #else                       /* call() and jump() when load as dll */
    #define QMLCFMBUSMULTICFMDEMO_API      QMLCFMBUSMULTICFMDEMO_DLL_IMPORT
    #define QMLCFMBUSMULTICFMDEMO_HIDDEN
  #endif
#endif

#endif   

#ifdef __cplusplus
	}
#endif


/* output the information */
#if defined( QMLCFMBUSMULTICFMDEMO_USE_QT5_INFO )
#  include <QDebug>
#  define QMLCFMBUSMULTICFMDEMO_DEBUG  QDebug
#  define QMLCFMBUSMULTICFMDEMO_INFO   qInfo
#  define QMLCFMBUSMULTICFMDEMO_FATAL  qFatal
#else
#  include <iostream>
#  include <exception>
#  include "qmlcfmbusmulticfmdemo_logging.hxx"
#  define QMLCFMBUSMULTICFMDEMO_DEBUG  qmlcfmbusmulticfmdemo_debug
#  define QMLCFMBUSMULTICFMDEMO_INFO   qmlcfmbusmulticfmdemo_info
#  define QMLCFMBUSMULTICFMDEMO_FATAL  qmlcfmbusmulticfmdemo_fatal
#endif

/* disable the copy in class */
#define QMLCFMBUSMULTICFMDEMO_DISABLE_COPY( t ) private : t ( const t & ); t & operator = ( const t & );

#if defined( qmlcfmbusmulticfmdemo_ptrcast )
#  error "qmlcfmbusmulticfmdemo_ptrcast already defined, conflicit ! Abort!"
#else
#  define qmlcfmbusmulticfmdemo_ptrcast( t, o )  ( reinterpret_cast< t >( reinterpret_cast< intptr_t >( o ))  )
#endif

#if defined( qmlcfmbusmulticfmdemo_intptrcast )
#  error "qmlcfmbusmulticfmdemo_intptrcast already defined, conflicit! Abort!"
#else
#  define qmlcfmbusmulticfmdemo_intptrcast( o )  ( reinterpret_cast< intptr_t >( o ))
#endif

#if defined( qmlcfmbusmulticfmdemo_objcast )
#  error "qmlcfmbusmulticfmdemo_objcast already defined, conflict! Abort!"
#else
#  define qmlcfmbusmulticfmdemo_objcast( t, o )  ( static_cast< t >( o ) )
#endif

/* define the unsued macro */
#if !defined( QMLCFMBUSMULTICFMDEMO_UNUSED )
#define QMLCFMBUSMULTICFMDEMO_UNUSED( x ) ((void)x)
#endif

#endif
