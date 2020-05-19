#ifndef QMLICQUICKIMAGEITEMDEMO_MM_IMAGELISTMODEL_HXX
#define QMLICQUICKIMAGEITEMDEMO_MM_IMAGELISTMODEL_HXX

#include "qmlicquickimageitemdemo/base/common/qmlicquickimageitemdemo_def.h"
#include <QObject>
#include <QImage>

namespace qmlIcquickimageitemDemo {

// ============================================================================
/*!
 * @brief this class provide a list of image
 */
// ============================================================================
// [HINT] here we can inhert from QAbstractListModel, so it can visit directly by view
//  but in this demo, we simply make it as simple custom model.
class QMLICQUICKIMAGEITEMDEMO_API  MmImageListModel : public QObject {
    Q_OBJECT
public:
    //! ctor
    explicit MmImageListModel ( QObject *pa = Q_NULLPTR );

    //! dtor
    virtual ~MmImageListModel( ) Q_DECL_OVERRIDE;

    //! get the count of the image
    int   count( ) const;

    //! get the image data by index
    QImage      imageAt( int ) const;

private:
    void *m_obj;
    Q_DISABLE_COPY( MmImageListModel )
};

}

#endif
