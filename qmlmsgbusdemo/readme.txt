【 简述 】

    qxpack/indcom/afw/qxpack_ic_msgbus 的使用。
消息总线用于管理一个应用后台消息 或者 模块需要发布出来的信息，
这些消息/信息需要被不可预知的其他模块知晓。

    检索 [HINT] 标记，在全部项目中可以检索关键代码段。


【 重要过程 】

1. 创建 msgsource 模块
   qmlmsgbusdemo_msgsource 中简单地启动了消息产生器服务, 消息产生器服务
将不断地往全局消息总线 MsgBus 中发送组名为 "MsgGroup.animal" 的消息

2. 创建 main/qmlmsgbusdemo_mm_mainvm 
   该模型视图用于向 view 层面提供数据，在这个实现文件中侦听了名为 "MsgGroup.animal"
的消息
   
3. 在 main/qmlmsgbusdemo_main 中注册
   注册 main/qmlmsgbusdemo_mm_mainvm, 使界面能够直接创建 viewmodel

   


