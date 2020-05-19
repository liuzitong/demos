#ifndef $D{Key2_ProjNameU}_DEF_H
#define $D{Key2_ProjNameU}_DEF_H

#include <stdint.h>
#include <float.h>

#ifdef __cplusplus
	extern "C" {
#endif


/* ////////////////////////////////////////////////////////////////////////////////
                                Export or Import
   ////////////////////////////////////////////////////////////////////////////  */
/* this helper definitions for shared libary support */
#if !defined( $D{Key2_ProjNameU}_SYM_EXPORT_DEF )
#define $D{Key2_ProjNameU}_SYM_EXPORT_DEF

#if defined( _WIN32 ) || defined( __CYGWIN__ )
  #define $D{Key2_ProjNameU}_DLL_IMPORT  __declspec(dllimport)
  #define $D{Key2_ProjNameU}_DLL_EXPORT  __declspec(dllexport)
  #define $D{Key2_ProjNameU}_DLL_HIDDEN
#else
  #if __GNUC__ >= 4
     #define $D{Key2_ProjNameU}_DLL_IMPORT __attribute__ ((visibility ("default")))
     #define $D{Key2_ProjNameU}_DLL_EXPORT __attribute__ ((visibility ("default")))
     #define $D{Key2_ProjNameU}_DLL_HIDDEN __attribute__ ((visibility ("hidden")))
  #else
     #define $D{Key2_ProjNameU}_DLL_IMPORT
     #define $D{Key2_ProjNameU}_DLL_EXPORT
     #define $D{Key2_ProjNameU}_DLL_HIDDEN
  #endif
#endif

/* check if defined $D{Key2_ProjNameU}_DLL */
#if defined( $D{Key2_ProjNameU}_CFG_STATIC )  && !defined( $D{Key2_ProjNameU}_CFG_STATIC )  && !defined( $D{Key2_ProjNameU}_CFG_DLL )
  #define $D{Key2_ProjNameU}_CFG_STATIC
#endif 
#if defined( $D{Key2_ProjNameU}_CFG_DLL ) && !defined( $D{Key2_ProjNameU}_CFG_STATIC )  &&  !defined( $D{Key2_ProjNameU}_CFG_DLL )
  #define $D{Key2_ProjNameU}_CFG_DLL
#endif

#undef $D{Key2_ProjNameU}_API
#undef $D{Key2_ProjNameU}_HIDDEN
#if defined( $D{Key2_ProjNameU}_CFG_STATIC )  /* normally build as static library */
  #define $D{Key2_ProjNameU}_API
  #define $D{Key2_ProjNameU}_HIDDEN
#else
  #if defined( $D{Key2_ProjNameU}_CFG_DLL )  /* we are building dll */
    #define $D{Key2_ProjNameU}_API      $D{Key2_ProjNameU}_DLL_EXPORT
    #define $D{Key2_ProjNameU}_HIDDEN   $D{Key2_ProjNameU}_DLL_HIDDEN
  #else                       /* call() and jump() when load as dll */
    #define $D{Key2_ProjNameU}_API      $D{Key2_ProjNameU}_DLL_IMPORT
    #define $D{Key2_ProjNameU}_HIDDEN
  #endif
#endif

#endif   

#ifdef __cplusplus
	}
#endif


/* output the information */
#if defined( $D{Key2_ProjNameU}_USE_QT5_INFO )
#  include <QDebug>
#  define $D{Key2_ProjNameU}_DEBUG  QDebug
#  define $D{Key2_ProjNameU}_INFO   qInfo
#  define $D{Key2_ProjNameU}_FATAL  qFatal
#else
#  include <iostream>
#  include <exception>
#  include "$D{Key2_ProjNameL}_logging.hxx"
#  define $D{Key2_ProjNameU}_DEBUG  $D{Key2_ProjNameL}_debug
#  define $D{Key2_ProjNameU}_INFO   $D{Key2_ProjNameL}_info
#  define $D{Key2_ProjNameU}_FATAL  $D{Key2_ProjNameL}_fatal
#endif

/* disable the copy in class */
#define $D{Key2_ProjNameU}_DISABLE_COPY( t ) private : t ( const t & ); t & operator = ( const t & );

#if defined( $D{Key2_ProjNameL}_ptrcast )
#  error "$D{Key2_ProjNameL}_ptrcast already defined, conflicit ! Abort!"
#else
#  define $D{Key2_ProjNameL}_ptrcast( t, o )  ( reinterpret_cast< t >( reinterpret_cast< intptr_t >( o ))  )
#endif

#if defined( $D{Key2_ProjNameL}_intptrcast )
#  error "$D{Key2_ProjNameL}_intptrcast already defined, conflicit! Abort!"
#else
#  define $D{Key2_ProjNameL}_intptrcast( o )  ( reinterpret_cast< intptr_t >( o ))
#endif

#if defined( $D{Key2_ProjNameL}_objcast )
#  error "$D{Key2_ProjNameL}_objcast already defined, conflict! Abort!"
#else
#  define $D{Key2_ProjNameL}_objcast( t, o )  ( static_cast< t >( o ) )
#endif

/* define the unsued macro */
#if !defined( $D{Key2_ProjNameU}_UNUSED )
#define $D{Key2_ProjNameU}_UNUSED( x ) ((void)x)
#endif

#endif
