#include <QDebug>
#include <QGuiApplication>
#include <QByteArray>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include <QImage>

#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#include <qxpack/indcom/common/qxpack_ic_tripleobjtemp.hpp>
#include <qxpack/indcom/sys/qxpack_ic_rmtobjcreator_priv.hxx>
#include <qxpack/indcom/sys/qxpack_ic_imagedata.hxx>
#include <qxpack/indcom/ui/base/qxpack_ic_quickimagesource.hxx>

// /////////////////////////////////////////////////
// grabber
// /////////////////////////////////////////////////
class  Grabber : public QObject {
    Q_OBJECT
public:
    Grabber( )
    {
        m_im_idx = 0;
        m_im[0].load( ":/bmp0.bmp"); m_im[0] = m_im[0].convertToFormat( QImage::Format_RGBA8888 );
        m_im[1].load( ":/bmp1.bmp"); m_im[1] = m_im[1].convertToFormat( QImage::Format_RGBA8888 );
        m_im[2].load( ":/bmp2.bmp"); m_im[2] = m_im[2].convertToFormat( QImage::Format_RGBA8888 );
        m_im[3].load( ":/bmp3.bmp"); m_im[3] = m_im[3].convertToFormat( QImage::Format_RGBA8888 );
    }
    virtual ~Grabber ( ) { }

    QxPack::IcTripleObjTemp<QxPack::IcImageData> &  frameSwapper( ){ return m_frame_swapper; }
    Q_INVOKABLE void doWork( ) { startTimer(45); }
    Q_SIGNAL    void imageSwapped( );

protected:

    virtual void  timerEvent( QTimerEvent* ) Q_DECL_OVERRIDE {
        ++ m_im_idx;
        // -----------------------------------------
        // in here, fill the data
        // -----------------------------------------
        QxPack::IcImageData *im_data = m_frame_swapper.writable();
        QImage im = m_im[ m_im_idx % 4  ];
        uchar *buff = im_data->writableImageBuffer( im.width(), im.height(), im.format() );
        std::memcpy( buff, im.constBits(),im.byteCount());
        m_frame_swapper.swap(true);

        emit this->imageSwapped();
    }
private:
    uint m_im_idx; QImage m_im[4];
    QxPack::IcTripleObjTemp<QxPack::IcImageData>  m_frame_swapper;
};

// ////////////////////////////////////////////////////
// handler in ui thread
// ///////////////////////////////////////////////////
class  GrabberHandler : public QObject {
    Q_OBJECT
private:
    QxPack::IcTripleObjTemp<QxPack::IcImageData> m_frame_swapper;
    QxPack::IcQuickImageSource  m_im_src;
public:
    GrabberHandler(
        const QxPack::IcTripleObjTemp<QxPack::IcImageData> &swp,
        const QxPack::IcQuickImageSource &src
    )
    {
        m_frame_swapper = swp; m_im_src = src;
    }

    Q_SLOT void onImageSwapper( )
    {
        QxPack::IcImageData *im_data = m_frame_swapper.readable();
        if ( im_data != Q_NULLPTR ) {
            QImage im = im_data->image();
            qInfo() << "thread:" << QThread::currentThread() << " use image pixel ptr:" << im.constBits();
            m_im_src.updateImage( im_data->image() );
        }
    }
};


// /////////////////////////////////////////////////
// main entry
// /////////////////////////////////////////////////
int main( int argc, char *argv[])
{
    int ret;
    QGuiApplication app( argc, argv );
    qDebug() << "main thread id:" << QThread::currentThread();

   // QxPack::IcMemCntr::enableMemTrace( true );

    qInfo() << " the current mem cntr:" << QxPack::IcMemCntr::currNewCntr();
    {
        QxPack::IcQuickImageSource  im_src( QxPack::IcQuickImageSource::FlagID_DeleteAfterPick );
        QThread  grab_thread; grab_thread.start();

        // create grabber
        Grabber *grab = qobject_cast< Grabber *>(
            QxPack::IcRmtObjCreator::createObjInThread(
                & grab_thread,
                []( void* ) -> QObject* {
                    Grabber *gr = new Grabber( );
                    QObject::connect( QThread::currentThread(), SIGNAL(finished()), gr, SLOT(deleteLater()) );
                    return gr;
                }, nullptr
            )
        );

        // create grabber handler
        GrabberHandler grab_handler( grab->frameSwapper(), im_src );
        QObject::connect( grab, SIGNAL(imageSwapped()), & grab_handler, SLOT(onImageSwapper()));

        // start
        QMetaObject::invokeMethod( grab, "doWork", Qt::BlockingQueuedConnection);
        QQmlApplicationEngine *eg = new QQmlApplicationEngine( );
        eg->rootContext()->setContextProperty("gQuickImageSrc", &im_src );
        eg->addImportPath("qrc:/");
        eg->load(QUrl(QStringLiteral("qrc:/main.qml")));
        ret = app.exec();

        // finish
        delete eg;
        grab_thread.quit();
        grab_thread.wait();
    }

   //QxPack::IcMemCntr::saveTraceInfoToFile("z:/mem_cntr.txt" );
   //QxPack::IcMemCntr::enableMemTrace(false);
    qInfo() << " the last mem cntr:" << QxPack::IcMemCntr::currNewCntr();

    return ret;
}


#include "main.moc"
