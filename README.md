[zh-cn](./README_zh.md)

# CXX_Thread

## Design expectations

Since this thread base class is designed to replace Qt's `QThread`, the expected functionality is as follows:

- Signal and slot mechanism. The main thread sends an instruction (i.e., calls the public member function of the business thread), and when the business thread receives an instruction from the main thread, the business thread can send a message to the message queue encapsulating the signal based on the instruction (internal call to the public function) to handle the corresponding business logic (handle the message queue and call the corresponding slot function or business logic code based on the signal value).
- Derived business classes that inherit from this base class can hold timers to do timed tasks (asynchronous execution).
- Thread objects can interact normally, e.g. Thread A can send signals to Thread B and Thread B can send signals to Thread A.

### Core code description

`SignalMsg` - struct

| variable name | meaning                                                                                                    |
|:--------------|:-----------------------------------------------------------------------------------------------------------|
| `_wait`       | type `bool` - If `_wait` is `true`, the slot function is executed synchronously, otherwise asynchronously. |
| `_signal`     | `int` type - used to represent signals                                                                     |
| `_msg`        | `std::shared_ptr<void>` type - stores the parameter data required by the slot function                     |

`ThreadBase` is a thread base class based on `std::thread` for handling message queues (storing data of type `SignalMsg`), which provides the following function functions:

| function name  | role                    |
|:---------------|:------------------------|
| `CreateThread` | creates a worker thread |
| `DestroyThread` | destroy the thread
| `GetThreadID` | get the thread ID |
| `GetCurrentThreadID` | Get the current thread ID |
| `SendSlotFuncAsyncRunMsg` | send an object of type `SignalMsg` to be executed asynchronously by the slot function |
| `SendSlotFuncSyncRunMsg` | send an object of type `SignalMsg`, the slot function executes synchronously |
| `UserCustomFunction` | pure virtual function - the derived class binds the signal to the slot function by overriding the function |
| `SendMsg` | Producer - passes an object of type `SignalMsg` to the message queue |
| `Process` | Consumer - consumes a `SignalMsg` type object in the message queue |

The `ThreadBase` thread base class does not support copy, construct copy and `Move` behavior.

## Testing

### Test code description

The `Human` derived class inherits from the `ThreadBase` base class, defines the required signals, then implements the corresponding signal slot functions, binds the signals to the slot functions by overriding the `UserCustomFunction` function; writes some `public` functions in the derived class that can send signals for external calls (i.e., accessible to the main thread).

### Test Driven Development - TDD

Use GoogleTest to do unit tests to test that the implemented threads meet the design expectations.

## Log checking

Use a [PLOG](https://github.com/SergiusTheBest/plog) open source library to implement logging. Based on the GoogleTest unit test, look at the logs to determine if the thread is running correctly according to the design expectations.

Translated with www.DeepL.com/Translator (free version)