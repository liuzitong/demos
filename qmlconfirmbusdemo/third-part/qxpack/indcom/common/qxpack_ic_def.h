#ifndef QXPACK_IC_DEF_H
#define QXPACK_IC_DEF_H

#include <stdint.h>
#include <float.h>

#ifdef __cplusplus
	extern "C" {
#endif


/* ////////////////////////////////////////////////////////////////////////////////
                                Export or Import
   ////////////////////////////////////////////////////////////////////////////  */
/* this helper definitions for shared libary support */
#if !defined( QXPACK_IC_SYM_EXPORT_DEF )
#define QXPACK_IC_SYM_EXPORT_DEF

#if defined( _WIN32 ) || defined( __CYGWIN__ )
  #define QXPACK_IC_DLL_IMPORT  __declspec(dllimport)
  #define QXPACK_IC_DLL_EXPORT  __declspec(dllexport)
  #define QXPACK_IC_DLL_HIDDEN
#else
  #if __GNUC__ >= 4
     #define QXPACK_IC_DLL_IMPORT __attribute__ ((visibility ("default")))
     #define QXPACK_IC_DLL_EXPORT __attribute__ ((visibility ("default")))
     #define QXPACK_IC_DLL_HIDDEN __attribute__ ((visibility ("hidden")))
  #else
     #define QXPACK_IC_DLL_IMPORT
     #define QXPACK_IC_DLL_EXPORT
     #define QXPACK_IC_DLL_HIDDEN
  #endif
#endif

/* check if defined QXPACK_IC_DLL */
#if defined( QXPACK_CFG_STATIC )  && !defined( QXPACK_IC_CFG_STATIC )  && !defined( QXPACK_IC_CFG_DLL )
  #define QXPACK_IC_CFG_STATIC
#endif 
#if defined( QXPACK_CFG_DLL ) && !defined( QXPACK_IC_CFG_STATIC )  &&  !defined( QXPACK_IC_CFG_DLL )
  #define QXPACK_IC_CFG_DLL
#endif

#undef QXPACK_IC_API
#undef QXPACK_IC_HIDDEN
#if defined( QXPACK_IC_CFG_STATIC )  /* normally build as static library */ 
  #define QXPACK_IC_API
  #define QXPACK_IC_HIDDEN
#else
  #if defined( QXPACK_IC_CFG_DLL )  /* we are building dll */
    #define QXPACK_IC_API      QXPACK_IC_DLL_EXPORT
    #define QXPACK_IC_HIDDEN   QXPACK_IC_DLL_HIDDEN
  #else                       /* call() and jump() when load as dll */
    #define QXPACK_IC_API      QXPACK_IC_DLL_IMPORT  
    #define QXPACK_IC_HIDDEN  
  #endif
#endif

#endif   

#ifdef __cplusplus
	}
#endif


/* output the information */
#if defined( QXPACK_IC_USE_QT5_INFO )
#  include <QDebug>
#  define QXPACK_IC_DEBUG  QDebug
#  define QXPACK_IC_INFO   qInfo
#  define QXPACK_IC_FATAL  qFatal
#else
#  include <iostream>
#  include <exception>
#  include "qxpack_ic_logging.hxx"
#  define QXPACK_IC_DEBUG  qxpack_ic_debug
#  define QXPACK_IC_INFO   qxpack_ic_info
#  define QXPACK_IC_FATAL  qxpack_ic_fatal
#endif

/* disable the copy in class */
#define QXPACK_IC_DISABLE_COPY( t ) private : t ( const t & ); t & operator = ( const t & );

#if defined( T_PtrCast )
#  error "T_PtrCast already defined, conflicit ! Abort!"
#else
#  define T_PtrCast( t, o )  ( reinterpret_cast< t >( reinterpret_cast< intptr_t >( o ))  )
#endif

#if defined( T_IntPtrCast )
#  error "T_IntPtrCast already defined, conflicit! Abort!"
#else
#  define T_IntPtrCast( o )  ( reinterpret_cast< intptr_t >( o ))
#endif

#if defined( T_ObjCast )
#  error "T_ObjCast already defined, conflict! Abort!"
#else
#  define T_ObjCast( t, o )  ( static_cast< t >( o ) )
#endif

/* define the unsued macro */
#if !defined( QXPACK_IC_UNUSED )
#define QXPACK_IC_UNUSED( x ) ((void)x)
#endif

#endif
