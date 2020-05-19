#ifndef QXPACK_IC_JSONLOGFORMATOR_HXX
#define QXPACK_IC_JSONLOGFORMATOR_HXX

#include "qxpack/indcom/common/qxpack_ic_def.h"

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief format the log message into json format string
 */
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_API  IcJsonLogFormator {
public:
    //! messge type enum
    enum MsgTypeID {
        MsgTypeID_Debug = 0, //!< generated member: "type":"debug"
        MsgTypeID_Info,      //!< generated member: "type":"info"
        MsgTypeID_Warning,   //!< generated member: "type":"warning"
        MsgTypeID_Critical,  //!< generated member: "type":"critical"
        MsgTypeID_Fatal      //!< generated member: "type":"fatal"
    };

    //! ctor
    explicit IcJsonLogFormator ( );

    //! dtor
    virtual ~IcJsonLogFormator( );

    size_t  filePathSepaNum  ( ) const;
    size_t  filePathLimitSize( ) const;
    size_t  funcNameLimitSize( ) const;
    size_t  messageLimitSize ( ) const;

    //! configurate the parameter
    //! @param fp_sepa_num [in] file path separator number
    //! @param fp_limit_sz [in] file path length limit size
    //! @param fn_limit_sz [in] function name length limit size
    //! @param msg_limit_sz [in] the message length limit size
    //! @note  for *limit_sz, 0 means no limit
    void    config(
        size_t fp_sepa_num, size_t fp_limit_sz,
        size_t fn_limit_sz, size_t msg_limit_sz
    );

    //! do the format routine
    //! @param msg_type   [in] the message type
    //! @param msg        [in] the message
    //! @param file_path  [in] the file path string
    //! @param func_name  [in] the function name string
    //! @param line_num   [in] the line number
    //! @note  after format(), user can use  result() to get pointer
    void    format (
        MsgTypeID  msg_type, const char *msg,
        const char *file_path, const char *func_name, size_t  line_num
    );

    //! return the last formated string pointer
    const char*  result( ) const;

    //! return the last formated string lengh
    size_t       size( ) const;

private:
    void *m_obj;
    QXPACK_IC_DISABLE_COPY( IcJsonLogFormator )
};

}

#endif
