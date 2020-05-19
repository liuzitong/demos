【 构建顺序 】

  按下述顺序进行构建
  1. qxiccore.pro 
  2. qxicqt5.pro   ( 必须在编译 qxiccore.pro 后再打开，编译 )
  3. qxicqt5ui.pro ( 必须在编译 qxiccore.pro, qxicqt5.pro 后再打开，编译 )

【 静态编译问题 】

  当前目录下为生成动态库，没有静态库编译，
但可以和软件项目一起编译，在软件项目中像如下添加

DEFINES *= QXPACK_IC_CFG_STATIC
include( qxpack/indcom/qxiccore.pri )

 


  