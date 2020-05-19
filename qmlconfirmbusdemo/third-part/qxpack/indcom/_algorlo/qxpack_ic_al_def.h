#ifndef QXPACK_IC_AL_DEF_H
#define QXPACK_IC_AL_DEF_H

#ifdef __cplusplus
namespace QxPack {
#endif

// ////////////////////////////////////////////////////////////////////////////
// system guess and memory model
// ////////////////////////////////////////////////////////////////////////////
#if defined(_WIN32) && !defined(_WIN64) && !defined(_amd64) 
#  define QXPACK_IC_AL_CFG_WIN32
#  define QXPACK_IC_AL_CFG_ILP32
#endif

#if defined(_WIN64) || defined(_amd64)
#  define QXPACK_IC_AL_CFG_WIN64
#  define QXPACK_IC_AL_CFG_LLP64
#endif

#if defined(__unix__) || defined(__linux__)
#  if defined(__LP64__)
#     define QXPACK_IC_AL_CFG_LINUX64
#     define QXPACK_IC_AL_CFG_LP64
#  else
#     define QXPACK_IC_AL_CFG_LINUX32
#     define QXPACK_IC_AL_CFG_ILP32 
#  endif
#endif

#ifdef __cplusplus
}
#endif
#endif
