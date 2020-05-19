【 简述 】

    qxpack/indcom/afw/qxpack_ic_confirmbus 的使用。
确认消息将会等候确认完成再返回给发出此确认的代码处。
侦听确认的对象可以有多个
    如：A,B,C 三个侦听对象，按加入序顺序为: C, A, B
    则形成 B->A->C 这样的顺序，如果B 没有处理确认（设置确认结果）
，则会继续调用 A，如果A 也没有处理，则调用 C

    检索 [HINT] 标记，在全部项目中可以检索关键代码段。


【 重要过程 】

1. qmlconfirmbusdemo_mm_mainvm 
   中建立 IcConfirm_onReqConfirm( ) 的SLOT 函数，用于侦听期望的组。
   在处理函数中，包装询问包，并且提交给 view，期待处理。

2. view 中的 main.qml
   在侦听 mainvm 的信号提交的询问包后，进行处理。这里采取模态对话框
处理。


   


