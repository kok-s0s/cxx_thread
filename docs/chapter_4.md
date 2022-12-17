# 关键

C++标准库提供了一些工具可用于同步，形式上表现为*条件变量*(condition variables)和future。并发技术规范中，为future添加了非常多的操作，并可与新工具*锁存器*(latches)(轻量级锁资源)和*栅栏*(barriers)一起使用。

## 等待事件或条件

当一个线程等待另一个线程完成时，可以持续的检查共享数据标志(用于做保护工作的互斥量)，直到另一线程完成工作时对这个标识进行重置。不过，这种方式会消耗线程的执行时间检查标识，并且当互斥量上锁后，其他线程就没有办法获取锁，就会持续等待。因为对等待线程资源的限制，并且在任务完成时阻碍对标识的设置。类似于保持清醒状态和列车驾驶员聊了一晚上：驾驶员不得不缓慢驾驶，因为你分散了他的注意力，所以火车需要更长的时间，才能到站。同样，等待的线程会等待更长的时间，也会消耗更多的系统资源。

另外，在等待线程在检查间隙，使用`std::this_thread::sleep_for()`进行周期性的间歇。

**优先选择的** -- 使用C++标准库提供的工具去等待事件的发生。通过另一线程触发等待事件的机制是最基本的唤醒方式(例如：流水线上存在额外的任务时)，这种 机制就称为“条件变量”。从概念上来说，条件变量会与多个事件或其他条件相关，并且一个或多个线程会等待条件的达成。当某些线程被终止时，为了唤醒等待线程(允许等待线程继续执行)，终止线程将会向等待着的线程广播“条件达成”的信息。

### 等待条件达成

C++标准库对条件变量有两套实现：`std::condition_variable`和`std::condition_variable_any`，这两个实现都包含在`<condition_variable>`头文件的声明中。两者都需要与互斥量一起才能工作(互斥量是为了同步)，前者仅能与`std::mutex`一起工作，而后者可以和合适的互斥量一起工作，从而加上了`_any`的后缀。因为` std::condition_variable_any`更加通用，不过在性能和系统资源的使用方面会有更多的开销，所以通常会将`std::condition_variable`作为首选类型。当对灵活性有要求时，才会考虑`std::condition_variable_any`。

使用`std::condition_variable`处理数据等待

```c++
std::mutex mut;
std::queue<data_chunk> data_queue;  // 1
std::condition_variable data_cond;

void data_preparation_thread()
{
  while(more_data_to_prepare())
  {
    data_chunk const data=prepare_data();
    std::lock_guard<std::mutex> lk(mut);
    data_queue.push(data);  // 2
    data_cond.notify_one();  // 3
  }
}

void data_processing_thread()
{
  while(true)
  {
    std::unique_lock<std::mutex> lk(mut);  // 4
    data_cond.wait(
         lk,[]{return !data_queue.empty();});  // 5
    data_chunk data=data_queue.front();
    data_queue.pop();
    lk.unlock();  // 6
    process(data);
    if(is_last_chunk(data))
      break;
  }
}
```

使用了简单的Lambda函数用于等待⑤(用于检查队列何时不为空)，不过任意的函数和可调用对象都可以传入wait()。当写好函数做为检查条件时，不一定非要放在一个Lambda表达式中，也可以直接将这个函数传入wait()。调用wait()的过程中，在互斥量锁定时，可能会去检查条件变量若干次，当提供测试条件的函数返回true就会立即返回。当等待线程重新获取互斥量并检查条件变量时，并非直接响应另一个线程的通知，就是所谓的*伪唤醒*(spurious wakeup)。因为任何伪唤醒的数量和频率都是不确定的，所以不建议使用有副作用的函数做条件检查。

本质上，`std::condition_variable::wait`是“忙碌-等待”的优化。

### 构建线程安全队列

```c++
#include <memory> // 为了使用std::shared_ptr

template<typename T>
class threadsafe_queue
{
public:
  threadsafe_queue();
  threadsafe_queue(const threadsafe_queue&);
  threadsafe_queue& operator=(
      const threadsafe_queue&) = delete;  // 不允许简单的赋值

  void push(T new_value);

  bool try_pop(T& value);  // 1
  std::shared_ptr<T> try_pop();  // 2

  void wait_and_pop(T& value);
  std::shared_ptr<T> wait_and_pop();

  bool empty() const;
};
```

[full_code_threadsafe_queue](../code/threadsafe_queue.h)

条件变量在多个线程等待同一个事件时也很有用。当线程用来分解工作负载，并且只有一个线程可以对通知做出反应时，与代码中结构完全相同。当数据准备完成时，调用notify_one()将会唤醒一个正在wait()的线程，检查条件和wait()函数的返回状态(因为仅是向data_queue添加了一个数据项)。 这里不保证线程一定会被通知到，即使只有一个等待线程收到通知，其他处理线程也有可能因为在处理数据，而忽略了这个通知。

另一种可能是，很多线程等待同一事件。对于通知，都需要做出回应。这会发生在共享数据初始化的时候，当处理线程使用同一数据时，就要等待数据被初始化，或等待共享数据的更新，比如：*周期性初始化*(periodic reinitialization)。这些情况下，线程准备好数据时，就会通过条件变量调用notify_all()，而非调用notify_one()。顾名思义，这就是全部线程在都去执行wait()(检查他们等待的条件是否满足)的原因。

当条件为true时，等待线程只等待一次，就不会再等待条件变量了，所以尤其是在等待一组可用的数据块时，一个条件变量并非同步操作最好的选择。

## 使用future

线程需要等待特定事件时，某种程度上来说就需要知道期望的结果。之后，线程会周期性(较短的周期)的等待或检查事件是否触发(检查信息板)，检查期间也会执行其他任务(品尝昂贵的咖啡)。另外，等待任务期间也可以先执行另外的任务，直到对应的任务触发，而后等待future的状态会变为就绪状态。future可能是和数据相关(比如，登机口编号)，也可能不是。当事件发生时(状态为就绪)，这个future就不能重置了。

C++标准库中有两种future，声明在`<future>`头文件中: unique future(`std::future<>`)和shared futures(`std::shared_future<>`)，与了`std::unique_ptr`和`std::shared_ptr`非常类似。`std::future`只能与指定事件相关联，而`std::shared_future`就能关联多个事件。后者的实现中，所有实例会在同时变为就绪状态，并且可以访问与事件相关的数据。这种关联与模板有关，比如`std::unique_ptr` 和`std::shared_ptr`的模板参数就是相关的数据类型。与数据无关处的，可以使用`std::future<void>`与`std::shared_future<void>`的特化模板。虽然，我倾向于线程通讯，但future对象本身并不提供同步访问。当多个线程需要访问一个独立future对象时，必须使用互斥量或类似同步机制进行保护。不过，当多个线程对一个`std::shared_future<>`副本进行访问，即使同一个异步结果，也不需要同步future。

并行技术规范将这两个模板类在`std::experimental`命名空间中进行了扩展：`std::experimental::future<>`和`std::experimental::shared_future<> `。这个命名空间是为了将其与`std`命名空间中的模板类进行区分，实验命名空间中为这两个模板类添加了更多的功能。尤其是`std::experimental`中的内容与代码质量无关(我希望这里也会有较高质量的实现)，需要强调的是这个命名空间提供的都不是标准类和函数，这个命名空间中类和函数的语法和语义，很可能与纳入C++标准(也就是`std`命名空间)后有所不同。如果想要使用这两个试验性的模板类，需要包含`<experimental/future>`头文件。

最简单的事件，就是在后台运行的计算操作。第2章中已经清楚了`std::thread` 执行的任务不能有返回值，不过这个问题能使用future进行解决。

### 后台任务的返回值

`std::async`函数模板(也是在头文件`<future>`)

当不着急让任务结果时，可以使用`std::async`启动一个异步任务。与`std::thread`对象等待的方式不同，`std::async`会返回一个`std::future`对象，这个对象持有最终计算出来的结果。当需要这个值时，只需要调用这个对象的get()成员函数，就会阻塞线程直到future为就绪为止，并返回计算结果。

[async_to_get_answer](../demo/async_to_get_answer/)

与`std::thread`方式一样，`std::async`允许通过添加额外的调用参数，向函数传递额外的参数。第一个参数是指向成员函数的指针，第二个参数提供这个函数成员类的具体对象(是通过指针，也可以包装在`std::ref`中)，剩余的参数可作为函数的参数传入。否则，第二个和随后的参数将作为函数的参数，或作为指定可调用对象的第一个参数。和`std::thread`一样，当参数为右值时，拷贝操作将使用移动的方式转移原始数据，就可以使用“只移动”类型作为函数对象和参数。

使用`std::async`向函数传递参数

```c++
#include <string>
#include <future>
struct X
{
  void foo(int,std::string const&);
  std::string bar(std::string const&);
};
X x;
auto f1=std::async(&X::foo,&x,42,"hello");  // 调用p->foo(42, "hello")，p是指向x的指针
auto f2=std::async(&X::bar,x,"goodbye");  // 调用tmpx.bar("goodbye")， tmpx是x的拷贝副本
struct Y
{
  double operator()(double);
};
Y y;
auto f3=std::async(Y(),3.141);  // 调用tmpy(3.141)，tmpy通过Y的移动构造函数得到
auto f4=std::async(std::ref(y),2.718);  // 调用y(2.718)
X baz(X&);
std::async(baz,std::ref(x));  // 调用baz(x)
class move_only
{
public:
  move_only();
  move_only(move_only&&)
  move_only(move_only const&) = delete;
  move_only& operator=(move_only&&);
  move_only& operator=(move_only const&) = delete;

  void operator()();
};
auto f5=std::async(move_only());  // 调用tmp()，tmp是通过std::move(move_only())构造得到
```

future的等待取决于`std::async`是否启动一个线程，或是否有任务在进行同步。大多数情况下，也可以在函数调用之前向`std::async`传递一个额外参数，这个参数的类型是`std::launch`，还可以是`std::launch::defered`，表明函数调用延迟到wait()或get()函数调用时才执行，`std::launch::async`表明函数必须在其所在的独立线程上执行，`std::launch::deferred | std::launch::async`表明实现可以选择这两种方式的一种。最后一个选项是默认的，当函数调用延迟，就可能不会再运行了。如下所示：

```c++
auto f6=std::async(std::launch::async,Y(),1.2);  // 在新线程上执行
auto f7=std::async(std::launch::deferred,baz,std::ref(x));  // 在wait()或get()调用时执行
auto f8=std::async(
              std::launch::deferred | std::launch::async,
              baz,std::ref(x));  // 实现选择执行方式
auto f9=std::async(baz,std::ref(x));
f7.wait();  //  调用延迟函数
```

### future与任务关联

`std::packaged_task<>`会将future与函数或可调用对象进行绑定。当调用`std::packaged_task<>`对象时，就会调用相关函数或可调用对象，当future状态为就绪时，会存储返回值。这可以用在构建线程池或其他任务的管理中，比如：在任务所在线程上运行其他任务，或将它们串行运行在一个特殊的后台线程上。当粒度较大的操作被分解为独立的子任务时，每个子任务都可以包含在`std::packaged_task<>`实例中，之后将实例传递到任务调度器或线程池中。对任务细节进行抽象，调度器仅处理`std::packaged_task<>`实例，而非处理单独的函数。

函数签名的返回类型可以用来标识从get_future()返回的`std::future<>`的类型，而函数签名的参数列表，可用来指定packaged_task的函数调用操作符。

`std::packaged_task<>`的偏特化

```c++
template<>
class packaged_task<std::string(std::vector<char>*,int)>
{
public:
  template<typename Callable>
  explicit packaged_task(Callable&& f);
  std::future<std::string> get_future();
  void operator()(std::vector<char>*,int);
};
```

`std::packaged_task`是个可调用对象，可以封装在`std::function`对象中，从而作为线程函数传递到`std::thread`对象中，或作为可调用对象传递到另一个函数中或直接调用。当`std::packaged_task`作为函数调用时，实参将由函数调用操作符传递至底层函数，并且返回值作为异步结果存储在`std::future`中，并且可通过get_future()获取。因此可以用`std::packaged_task`对任务进行打包，并适时的取回future。当异步任务需要返回值时，可以等待future状态变为“就绪”。

**线程间传递任务**

很多图形架构需要特定的线程去更新界面，所以当线程对界面更新时，需要发出一条信息给正确的线程，让相应的线程来做界面更新。`std::packaged_task`提供了这种功能，且不需要发送一条自定义信息给图形界面线程。

## 使用std::promises

当需要处理很多网络连接时，会使用不同线程尝试连接每个接口，能使网络尽早联通。不幸的是，随着连接数量的增长，这种方式变的越来越不合适。因为大量的线程会消耗大量的系统资源，还有可能造成线程上下文频繁切换(当线程数量超出硬件可接受的并发数时)，这都会对性能有影响。最极端的例子：线程会将系统资源消耗殆尽，系统连接网络的能力会变的极差。因此通过少数线程处理网络连接，每个线程同时处理多个连接，对需要处理大量网络连接的应用而言，这是一种比较普遍的做法。

当线程处理多个连接事件，来自不同的端口连接的数据包基本上以乱序方式进行处理。同样的，数据包也将以乱序的方式进入队列。很多情况下，一些应用不是等待数据成功的发送，就是等待(新的)指定网络接口数据的接收成功。

`std::promise<T>`提供设定值的方式(类型为T)，这个类型会和后面看到的`std::future<T>`对象相关联。`std::promise/std::future`对提供一种机制：future可以阻塞等待线程，提供数据的线程可以使用promise对相关值进行设置，并将future的状态置为“就绪”。

可以通过给定的`std::promise`的get_future()成员函数来获取与之相关的`std::future`对象，与`std::packaged_task`的用法类似。当promise设置完毕(使用set_value()成员函数)时，对应的future状态就变为“就绪”，并且可用于检索已存储的值。当设置值之前销毁`std::promise`，将会存储一个异常。
