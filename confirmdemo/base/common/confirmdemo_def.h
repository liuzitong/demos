#ifndef CONFIRMDEMO_DEF_H
#define CONFIRMDEMO_DEF_H

#include <stdint.h>
#include <float.h>

#ifdef __cplusplus
	extern "C" {
#endif


/* ////////////////////////////////////////////////////////////////////////////////
                                Export or Import
   ////////////////////////////////////////////////////////////////////////////  */
/* this helper definitions for shared libary support */
#if !defined( CONFIRMDEMO_SYM_EXPORT_DEF )
#define CONFIRMDEMO_SYM_EXPORT_DEF

#if defined( _WIN32 ) || defined( __CYGWIN__ )
  #define CONFIRMDEMO_DLL_IMPORT  __declspec(dllimport)
  #define CONFIRMDEMO_DLL_EXPORT  __declspec(dllexport)
  #define CONFIRMDEMO_DLL_HIDDEN
#else
  #if __GNUC__ >= 4
     #define CONFIRMDEMO_DLL_IMPORT __attribute__ ((visibility ("default")))
     #define CONFIRMDEMO_DLL_EXPORT __attribute__ ((visibility ("default")))
     #define CONFIRMDEMO_DLL_HIDDEN __attribute__ ((visibility ("hidden")))
  #else
     #define CONFIRMDEMO_DLL_IMPORT
     #define CONFIRMDEMO_DLL_EXPORT
     #define CONFIRMDEMO_DLL_HIDDEN
  #endif
#endif

/* check if defined CONFIRMDEMO_DLL */
#if defined( CONFIRMDEMO_CFG_STATIC )  && !defined( CONFIRMDEMO_CFG_STATIC )  && !defined( CONFIRMDEMO_CFG_DLL )
  #define CONFIRMDEMO_CFG_STATIC
#endif 
#if defined( CONFIRMDEMO_CFG_DLL ) && !defined( CONFIRMDEMO_CFG_STATIC )  &&  !defined( CONFIRMDEMO_CFG_DLL )
  #define CONFIRMDEMO_CFG_DLL
#endif

#undef CONFIRMDEMO_API
#undef CONFIRMDEMO_HIDDEN
#if defined( CONFIRMDEMO_CFG_STATIC )  /* normally build as static library */
  #define CONFIRMDEMO_API
  #define CONFIRMDEMO_HIDDEN
#else
  #if defined( CONFIRMDEMO_CFG_DLL )  /* we are building dll */
    #define CONFIRMDEMO_API      CONFIRMDEMO_DLL_EXPORT
    #define CONFIRMDEMO_HIDDEN   CONFIRMDEMO_DLL_HIDDEN
  #else                       /* call() and jump() when load as dll */
    #define CONFIRMDEMO_API      CONFIRMDEMO_DLL_IMPORT
    #define CONFIRMDEMO_HIDDEN
  #endif
#endif

#endif   

#ifdef __cplusplus
	}
#endif


/* output the information */
#if defined( CONFIRMDEMO_USE_QT5_INFO )
#  include <QDebug>
#  define CONFIRMDEMO_DEBUG  QDebug
#  define CONFIRMDEMO_INFO   qInfo
#  define CONFIRMDEMO_FATAL  qFatal
#else
#  include <iostream>
#  include <exception>
#  include "confirmdemo_logging.hxx"
#  define CONFIRMDEMO_DEBUG  confirmdemo_debug
#  define CONFIRMDEMO_INFO   confirmdemo_info
#  define CONFIRMDEMO_FATAL  confirmdemo_fatal
#endif

/* disable the copy in class */
#define CONFIRMDEMO_DISABLE_COPY( t ) private : t ( const t & ); t & operator = ( const t & );

#if defined( confirmdemo_ptrcast )
#  error "confirmdemo_ptrcast already defined, conflicit ! Abort!"
#else
#  define confirmdemo_ptrcast( t, o )  ( reinterpret_cast< t >( reinterpret_cast< intptr_t >( o ))  )
#endif

#if defined( confirmdemo_intptrcast )
#  error "confirmdemo_intptrcast already defined, conflicit! Abort!"
#else
#  define confirmdemo_intptrcast( o )  ( reinterpret_cast< intptr_t >( o ))
#endif

#if defined( confirmdemo_objcast )
#  error "confirmdemo_objcast already defined, conflict! Abort!"
#else
#  define confirmdemo_objcast( t, o )  ( static_cast< t >( o ) )
#endif

/* define the unsued macro */
#if !defined( CONFIRMDEMO_UNUSED )
#define CONFIRMDEMO_UNUSED( x ) ((void)x)
#endif

#endif
