# CXX_Thread

## 设计期望

因该线程基类设计出来是为了替代 Qt 的 QThread，所以期望的功能如下：

- 线程安全的信号与槽机制；
- 继承该基类的类能有线程安全的定时器；
- 线程对象之间能够正常交互，比如线程 A 能够向线程 B 发送信号，线程 B 能够向线程 A 发送信号；

### ThreadBase

`ThreadMsg` 是一个结构体，用于封装消息。每个 `ThreadMsg` 结构体包含:

- 一个信号 `id`；
- 一个 `std::shared_ptr<void>` 类型的消息（参数）；
- 一个标志位 `_wait`。在该类中，当 `_wait` 标志位为 `true` 时，表示该信号对应的槽函数为同步运行，为 `false` 时表示该信号对应的槽函数为异步运行。

`ThreadBase` 是一个基于 `std::thread` 的线程基类，用于处理消息队列（信号），该类提供了以下功能：

- `CreateThread` 函数 - 创建工作线程；
- `ExitThread` 函数 - 退出工作线程；
- `GetThreadID` 函数 - 获取线程 ID；
- `GetCurrentThreadID` 函数 - 获取当前线程 ID；
- `UserCustomFunction` 纯虚函数 - 具体功能需要在派生类中实现。该函数用于建立信号和槽函数之间的关系，即将消息队列中的消息与对应的处理函数联系起来；
- `PostOrSendMsg` 函数 - 传递消息到消息队列中（同步 `PostMsg` or 异步 `SendMsg`）；
- `Process` 函数，用于在工作线程中处理消息队列。该函数首先从队列中取出消息，再根据消息的信号 id 调用派生类对应中的槽函数；`UserCustomFunction`，最后根据消息的 `_wait` 标志位，唤醒等待该消息处理结果的线程；

## Test

### Human 类（继承 ThreadBase）

主要要重写 `UserCustomFunction` 函数，用于建立信号和槽函数之间的关系。

使用 GoogleTest 做单元测试，测试所实现的线程能否达到设计期望。
