
#include <windows.h>

// ANSI C++ headers
#include <cstdio>
#include <deque>
#include <map>
#include <set>
#include <string>
#include <vector>


#include <Ishtar/Thread.h>
#include <Ishtar/Task/TaskFwd.h>
#include <Ishtar/ThreadPool.h>

//#include <Ishtar/Thread/Mutex.h>
#include <iostream>
#include <boost/bind.hpp>
#include <mutex>

//#include "../../Ishtar/src/Ishtar/Thread/ThreadImpl.h"
//#include <boost/thread.hpp>

#include <Ishtar/Trace.h>

// Assuming ThreadsStatus is a type alias for a container like std::vector
//using ThreadsStatus = std::vector<ThreadStatus>; // Replace ThreadStatus with the actual type


#include <Gt2/Database/DatabaseManager.h> // Include the DatabaseManager header
#include <Gt2/Database/DataSource.h> // DataReader


using namespace std;
using namespace Ishtar;


// ===================================================== //

// hello 函式
void testHello()
{
    cout << "Hello, World!! 123" << endl;
    cout << "Hello from the hello function!" << endl;
}

// 新增一個 inline 函式
inline void testPrintSeparator() 
{
    cout << "====================" << endl << endl;
}

// 測試指標和引用
void testPointer() 
{
    int a = 10;
    int* p = &a; // 指標 p 指向變數 a 的地址

    cout << "Value of a: " << a << endl;
    cout << "Address of a: " << &a << endl;

    cout << "Value pointed by p: " << *p << endl;
    cout << "Address stored in p: " << p << endl;

    cout << "Size of pointer p: " << sizeof(p) << " bytes" << endl;

}

// 測試引用
void testAddress()
{
    int b = 5;
    int& a = b; // 讓 a 是 b 的別名(alias)

    a = 6;
    cout << "Value of b: " << b << endl; // 6
    cout << "Value of a: " << a << endl; // 6

    cout << "Address of b: " << &b << endl;
    cout << "Address of a: " << &a << endl; // 兩者地址相同
    cout << "Size of reference a: " << sizeof(a) << " bytes" << endl; // 引用大小與原變數相同
}



/*

// marco定義
#define PI 3.14159

// marco定義條件
#ifdef _WIN64
void printDebugMode() {
    cout << "Debug mode _WIN64" << endl;



#ifdef _WIN64
    printDebugMode();
#endif

    hello();
    printSeparator();

    //testPointer(); // *
    printSeparator();

    //testAddress(); // &
    printSeparator();

    //testAddTemplate(); // 
    printSeparator();


}
#endif


*/

// 測試模板函式
template <typename T>
auto add(T a, T b) -> decltype(a + b)
{
    return a + b;
}
void testAddTemplate()
{
    cout << "Testing add template function:" << endl;

    std::cout << add(3, 4) << std::endl;      // 7
    std::cout << add(2.5, 3.5) << std::endl;  // 6.0
}



// Vector
void testVector()
{
    std::vector<int> v;
    std::cout << "Capacity: " << v.capacity() << std::endl;

    for (int i = 0; i < 20; ++i)
    {
        v.push_back(i);
        std::cout << "Size: " << v.size() << ", Capacity: " << v.capacity() << std::endl;
    }
}


class A
{
public:
    void print() { std::cout << "A::print()\n"; }
};
class B
{
public:
    void print() { std::cout << "B::print()\n"; }
};
class C : public A, public B
{
    // 沒有自己定義 print()
};
void testMultipleInheritanceSameFuncName()
{
    C c;

    // 使用 A 的 print()
    c.A::print(); // 輸出: A::print()

    // 使用 B 的 print()
    c.B::print(); // 輸出: B::print()

	//c.print(); // 這會導致編譯錯誤，因為 C 沒有自己的 print() 函式

}


// ===================================================== //

int counter = 0;  // 共享變數

// Test ThreadsStatus
void testThreadsStatus()
{
    // 獲取所有執行緒的狀態
    Ishtar::ThreadsStatus threads = Ishtar::ThreadManaging::GetThreadsStatus();

    std::cout << "==== 目前執行緒列表 ====" << std::endl;

    // 遍歷所有執行緒的狀態
    while (threads.MoveNext())
    {
        const Ishtar::ThreadStatus& status = threads.Current();

        std::cout << "執行緒 ID: " << status.ThreadId() << std::endl;
        std::cout << "執行緒名稱: " << status.Name() << std::endl;
        std::cout << "目前狀態文字: " << status.Text() << std::endl;
        std::cout << "是否忙碌: " << (status.IsBusy() ? "是" : "否") << std::endl;
        std::cout << "忙碌計數: " << status.TotalBusyCount() << std::endl;
        std::cout << "目前經過時間: " << status.CurrentBusyElapsed() << " ms" << std::endl;
        std::cout << "最大忙碌持續時間: " << status.MaxBusyDuration() << " ms" << std::endl;
        std::cout << "最大忙碌任務名稱: " << status.MaxBusyName() << std::endl;
        std::cout << "------------------------" << std::endl;
    }
}

// Test thread
int myThreadFunction() 
{
    // 定義一個執行緒要執行的函式
    cout << "執行緒開始執行！" << endl;

    // 模擬工作
    Ishtar::Thread::Sleep(1000); // 休眠 1 秒

    cout << "執行緒結束！" << endl;

    return EXECUTE_EXIT;

}
void testThread() 
{
    Thread thread1;
    Thread thread2;
    Thread thread3;
    
    // 使用 Start 啟動執行緒
    thread1.Start("MyThread1", myThreadFunction);
    thread2.Start("MyThread2", myThreadFunction);
    thread3.Start("MyThread3", myThreadFunction);

    testThreadsStatus();

    // 等待執行緒結束
    thread1.Join();
    thread2.Join();
    thread3.Join();
}


// Test Executable
class ThreadExecutableClass : public Ishtar::Executable
{
public:
    ThreadExecutableClass(const std::string& name, const int count) : m_name(name), m_counter(count) {}

    virtual ~ThreadExecutableClass() {}

    virtual std::string GetName() const override
    {
        return m_name;
    }

    virtual Int Execute() override
    {
        cout << "執行緒可執行物件開始執行！" << endl;

        // 模擬工作
        //Ishtar::Thread::Sleep(1000); // 休眠 1 秒
        if (m_counter < 100) 
        {
            ++m_counter;
			cout << "執行緒可執行物件計數器: " << m_counter << endl;
			return EXECUTE_CONTINUE; // 返回 EXECUTE_CONTINUE 以繼續執行
        }
        else 
        {
			return EXECUTE_EXIT; // 返回 EXECUTE_EXIT 以結束執行
        }
        
        cout << "執行緒可執行物件結束！" << endl;
    }

private:
    std::string m_name; // 執行緒名稱
    int m_counter = 0; // 計數器
};
void testExecutable()
{
    ThreadExecutableClass* tc1 = new ThreadExecutableClass("MyThreadExecutable", 0);
    Ishtar::Thread t1;

    t1.Start(tc1); // 啟動執行緒
    t1.Join(); // 等待執行緒結束
}


// Test ThreadExecution
class MyThreadExecution : public Ishtar::ThreadExecution
{
public:

    MyThreadExecution(const std::string& name) : m_name(name) {}

    virtual std::string GetName() const override
    {
        return m_name;
    }

    virtual Int Execute() override
    {
        cout << "執行緒執行開始！" << endl;

        if (m_counter < 100)
        {
            ++m_counter;
            cout << "執行緒可執行物件計數器: " << m_counter << endl;
            return EXECUTE_CONTINUE; // 返回 EXECUTE_CONTINUE 以繼續執行
        }
        else
        {
            return EXECUTE_EXIT; // 返回 EXECUTE_EXIT 以結束執行
        }

        cout << "執行緒執行結束！" << endl;
    }

private:
    std::string m_name; // 執行緒名稱
	int m_counter = 0; // 計數器
};
void testThreadExecution()
{
    MyThreadExecution* myExec = new MyThreadExecution("MyThreadExecution");
    myExec->Start(); // 啟動執行緒
    myExec->Join(); // 等待執行緒結束

    //MyThreadExecution myExec2("MyThreadExecution2");
    //myExec2.Start(); // 啟動執行緒
    //myExec2.Join(); // 等待執行緒結束
}


// Test ThreadGroup
void threadTaskFunction() 
{
    for (int i = 0; i < 1000; ++i)
    {
        ++counter;  // 非原子操作，可能產生 race condition
        cout << "now counter: " << counter << endl;
    }
}
int MyThreadGroupFunction()
{
	threadTaskFunction(); // 執行任務函式
    return EXECUTE_CONTINUE;
}
void MyThreadGroupVoidFunction()
{
    threadTaskFunction(); // 執行任務函式
}
void testThreadGroup()
{
    // 測試 Ishtar::ThreadGroup 的使用

    Ishtar::ThreadGroup tg;
    ThreadGroup::ExecuteFunction ef = MyThreadGroupFunction;
    ThreadGroup::VoidExecuteFunction vef = MyThreadGroupVoidFunction;

    tg.AddThread("MyThread1", ef);
    tg.AddOnceThread("MyThread2", vef);
    tg.AddLoopThread("MyThread3", vef);

    tg.Shutdown(5000); // 等待 5 秒鐘，讓所有執行緒結束 // 預設 10 秒鐘 //

    //tg.AddThread("MyThread1", MyThreadGroupFunction);
    //tg.AddOnceThread("MyThread2", MyThreadGroupFunction);
    //tg.AddLoopThread("MyThread3", MyThreadGroupFunction, 1000);

    //boost::bind(&Ishtar::ThreadGroup::AddThread, &tg, "MyThread1", myThreadFunction)();
}


// Test ThreadPool
void threadPoolWorkFunction(int num) 
{
    int counter = 0; // 每個執行緒的計數器

    for (int i = 0; i < num; ++i)
    {
        ++counter;  // 非原子操作，可能產生 race condition

        cout << "work function now counter: " << counter << endl;
    }
}
class ThreadPoolWorkClass
{
public:

	int counter = 0; // 每個執行緒的計數器

    void Execute(int num)
    {
        int counter = 0; // 每個執行緒的計數器

        for (int i = 0; i < num; ++i)
        {
            ++counter;  // 非原子操作，可能產生 race condition

            cout << "work class now counter: " << counter << endl;
        }
    }

    void Execute2(int num) 
    {
        cout << "Execute2 num: " << num << endl;
    }

    void Execute3(int& num)
    {
        // 
        for (int i = 0; i < 20; ++i)
        {
            cout << "Execute3 num: " << num << endl;
        }
	}

    void Execute4(int* num)
    {
        for (int i = 0; true; ++i)
        {
            cout << "Execute4 num: " << *num << endl;
        }
    }

    void Execute5(ThreadPoolWorkClass& obj)
    {
        for (int i = 0; true; ++i)
        {
            cout << "Execute5 obj.counter: " << obj.counter << endl;
        }
    }

};
void testThreadPool()
{
	Ishtar::ThreadPool::Startup(2); // 啟動 ThreadPool // 指定執行緒的數量

	//Ishtar::ThreadPool::AddThread(2); // ThreadPool 增加 2 條執行緒 

	//Ishtar::ThreadPool::Shutdown(); // 關閉 ThreadPool // 最多等待 10 秒 終止所有工作
}


// Test ThreadPool & Work
void testThreadPoolWork()
{
    Ishtar::ThreadPool::WorkPtr wPtr1;
    Ishtar::ThreadPool::WorkPtr wPtr2;

    // ISHTAR_TP_SUBMIT_WORK // Ishtar::ThreadPool::SubmitWork 

    ThreadPoolWorkClass tpwc;

    wPtr1 = ISHTAR_TP_SUBMIT_WORK(threadPoolWorkFunction, (15), ());
    wPtr2 = ISHTAR_TP_SUBMIT_WORK(ThreadPoolWorkClass::Execute, (tpwc, 20), ());

    
    cout << "wPtr1->IsInvoked: " << wPtr1->IsInvoked() << endl; // 檢查工作是否 已被執行 //
    cout << "wPtr1->IsWorking: " << wPtr1->IsWorking() << endl; // 檢查工作是否 正在執行 //
    cout << "wPtr1->IsComplete: " << wPtr1->IsComplete() << endl; // 檢查工作是否 執行完成 //

    // 執行緒 ID // 尚未執行或已被取消，回傳 0
    cout << "wPtr1->GetWorkingThreadId: " << wPtr1->GetWorkingThreadId() << endl;

    wPtr1->Cancel(); // 取消工作
    wPtr2->Cancel(); // 取消工作
}


// Test ThreadPool & Barrier
class BarrierOrderWorkClass : public Ishtar::ThreadPool::Schedulable {};

void testThreadPoolBarrierOrder()
{
    ThreadPoolWorkClass tpwc;
    BarrierOrderWorkClass bowc;
    Ishtar::ThreadPool::WorkPtr wPtr3;

    // Order Barrier
    for (int i = 0; i < 20; i++) 
    {
        wPtr3 = ISHTAR_TP_SUBMIT_WORK
        (
            ThreadPoolWorkClass::Execute2, 
            (tpwc, i), 
            (Ishtar::ThreadPool::Order(bowc))
        );
    }

    while (!wPtr3->IsComplete()) { Sleep(1); }

	//system("pause"); // 暫停程式，等待使用者輸入
}
void testThreadPoolBarrierDelay() 
{
    ThreadPoolWorkClass tpwc;
    Ishtar::ThreadPool::WorkPtr wPtr4;

    // Delay Barrier
    for (int i = 0; i < 30; i++)
    {
        wPtr4 = ISHTAR_TP_SUBMIT_WORK
        (
            ThreadPoolWorkClass::Execute2, 
            (tpwc, i), 
            (Ishtar::ThreadPool::Delay(100 * i))
        );
    }

    while (!wPtr4->IsComplete()) { Sleep(1); }
    //system("pause"); // 暫停程式，等待使用者輸入
	//Sleep(5000); // 等待 5 秒鐘，讓所有工作完成
}
void testThreadPoolBarrierDelayOrder()
{
    ThreadPoolWorkClass tpwc;
    BarrierOrderWorkClass bowc;
    Ishtar::ThreadPool::WorkPtr wPtr5;

    // Delayed Order Barrier
    for (int i = 0; i < 30; i++)
    {
        wPtr5 = ISHTAR_TP_SUBMIT_WORK
        (
            ThreadPoolWorkClass::Execute2, 
            (tpwc, i), 
            (Ishtar::ThreadPool::DelayedOrder(bowc, 100 * i))
        );
    }

    while (!wPtr5->IsComplete()) { Sleep(1); }
    //system("pause"); // 暫停程式，等待使用者輸入
}
void testThreadPoolBarrierHighOrder() 
{
    ThreadPoolWorkClass tpwc;
    BarrierOrderWorkClass bowc;
    Ishtar::ThreadPool::WorkPtr wPtr6;

    for (int i = 0; i < 20; i++)
    {
        wPtr6 = ISHTAR_TP_SUBMIT_WORK
        (
            ThreadPoolWorkClass::Execute2,
            (tpwc, i),
            (Ishtar::ThreadPool::HighOrder(bowc))
        );
    }
    
    while (!wPtr6->IsComplete()) { Sleep(1); }
}


// Test SimpleMutex
void testSimpleMutex()
{
    Ishtar::SimpleMutex simpleMutex;
}

// Test Mutex
void testMutex() 
{
    // 測試 Ishtar::Mutex 的使用
    Ishtar::Mutex mutex;

    {
        Ishtar::Mutex::UniqueLock lock(mutex); // 獲取鎖

        // 在這裡執行需要互斥的操作

        cout << "獲取鎖，執行互斥操作" << endl;
    } 
    // 鎖會在 UniqueLock 的析構函式中自動釋放

    cout << "鎖已釋放" << endl;
}

// Test SharedMutex
void testSharedMutex()
{
    Ishtar::SharedMutex sharedMutex;
}

// Test SpinMutex
void testSpinMutex()
{

}


// Test Mutex Marco
class ThreadClass 
{
public:
    ThreadClass() :data(0) {}

    Int Execute1() 
    {
        // 鎖定
        ISHTAR_GET_MUTEX_LOCK(dataLock, dataMutex);

        if (data < 20)
        {
            cout << data++ << " ";
            return Ishtar::EXECUTE_CONTINUE;
        }
        else
        {
            return Ishtar::EXECUTE_EXIT;
        }

        // 解鎖
        dataLock.Unlock();
    }
private:
    int data = 0;
    Ishtar::Mutex dataMutex; //建立一個 Mutex 變數
};
void testMutexMarco()
{
    ThreadClass* tc = new ThreadClass();
    ThreadClass* tc2 = new ThreadClass();

    Ishtar::Thread t1;
    Ishtar::Thread t2;

    t1.Start("Thread1", boost::bind(&ThreadClass::Execute1, tc));
    t2.Start("Thread2", boost::bind(&ThreadClass::Execute1, tc));

    t1.Join();
    t2.Join();

	system("pause"); // 暫停程式，等待使用者輸入
}

//ISHTAR_LOCK_MUTEX();
//ISHTAR_GET_MUTEX_LOCK();
//ISHTAR_SHARED_LOCK_MUTEX();
//ISHTAR_GET_MUTEX_SHARED_LOCK();


// Test Lockable
class MySet : private Ishtar::Lockable<> 
{
public:

    void Insert(int num) 
    {
        ISHTAR_LOCK_THIS(); // 內部鎖定互斥鎖 (鎖定整個函式)

        cout << num << " ";

        m_set.insert(num);
    }

    typedef set<int> ::iterator Iter;

    Iter GetBegin() 
    {
        return m_set.begin();
    }

    Iter GetEnd() 
    {
        return m_set.end();
	}

private:
    set<int> m_set;
};
void PrintSet(MySet& mset) 
{
    MySet::Iter it;
    
    for (it = mset.GetBegin(); it != mset.GetEnd(); it++) 
    {
        cout << *it << " ";
    }
}
void testLockable()
{
    MySet mySet;

    Ishtar::ThreadPool::Startup(3);
    Ishtar::ThreadPool::AddThread(2);

    for (int i = 0; i < 30; i++) 
    {
        ISHTAR_TP_SUBMIT_WORK(MySet::Insert, (&mySet, i), ());
    }

    Sleep(500);
    Ishtar::ThreadPool::Shutdown();

    cout << endl;
    PrintSet(mySet);

    //// 測試 Ishtar::Lockable 的使用
    //class MyLockable : public Ishtar::Lockable<>
    //{
    //public:
    //    void DoSomething()
    //    {
    //        ISHTAR_LOCK_THIS(); // 獲取鎖
    //        cout << "Doing something in MyLockable!" << endl;
    //        // 在這裡執行需要互斥的操作
    //        ISHTAR_UNLOCK_THIS(); // 釋放鎖
    //    }
    //};
    //MyLockable myLockable;
    //myLockable.DoSomething();
}

class MySet2: public Ishtar::Lockable<>
{
public:

    void InsertUnsafe(int num)
    {
        cout << num << " ";

        m_set.insert(num);
    }

    typedef set<int> ::iterator Iter;

    Iter GetBegin()
    {
        return m_set.begin();
    }

    Iter GetEnd()
    {
        return m_set.end();
    }

private:
    set<int> m_set;
};

void testLockable2() 
{
    MySet2 mySet2;

    // 方式: 使用ISHTAR_LOCK_OBJECT宏 - 鎖定整個區塊
    {
        ISHTAR_LOCK_OBJECT(mySet2); // 外部鎖定互斥鎖（鎖定物件所在的函式區塊）
        mySet2.InsertUnsafe(42);
        mySet2.InsertUnsafe(43);
    } 
    // 自動解鎖
}


// Test Lockable Marco
void testLockableMarco()
{
    // 外部鎖定互斥鎖（鎖定物件所在的函式區塊）
    //ISHTAR_LOCK_OBJECT（obj）
    //ISHTAR_GET_OBJECT_LOCK（lock，obj）
    // 
    // 內部鎖定互斥鎖（鎖定整個函式）
    //ISHTAR_LOCK_THIS（）
    //ISHTAR_GET_THIS_LOCK（lock）



    //// 測試 Ishtar::Lockable 的宏使用
    //class MyLockableClass : public Ishtar::Lockable<>
    //{
    //public:
    //    void DoWork()
    //    {
    //        ISHTAR_LOCK_THIS(); // 獲取鎖
    //        cout << "Doing work in MyLockableClass!" << endl;
    //        // 在這裡執行需要互斥的操作
    //        ISHTAR_UNLOCK_THIS(); // 釋放鎖
    //    }
    //};
    //MyLockableClass myLockableClass;
    //myLockableClass.DoWork();
}


// Test ConditionVariable
void testConditionVariable() 
{

    // 測試 Ishtar::ConditionVariable 的使用
    Ishtar::ConditionVariable cv;
    Ishtar::Mutex mutex;

    // 在這裡可以實現一個簡單的條件變數測試
    // 使用 Ishtar::Thread 來創建多個執行緒，並使用條件變數進行同步
    // 觀察是否能正確地通知和等待事件發生
	// 注意：這裡僅提供一個簡單的框架，具體實現???要根據需求進行調整
}


// Test ISHTAR_TRACE 
void testIshtarTrace()
{
//#define ISHTAR_TRACE_ERROR   Ishtar::Trace::WriteError
//#define ISHTAR_TRACE_WARNING Ishtar::Trace::WriteWarning
//#define ISHTAR_TRACE_INFO    Ishtar::Trace::WriteInfo
//#define ISHTAR_TRACE_VERBOSE Ishtar::Trace::WriteVerbose

    ISHTAR_TRACE_VERBOSE("This is a verbose message");
    ISHTAR_TRACE_INFO("This is an info message");
    ISHTAR_TRACE_WARNING("This is a warning message");
    ISHTAR_TRACE_ERROR("This is an error message");

}


// ===================================================== //


// Test Race Condition // Lambda
void incrementLambda()
{
    for (int i = 0; i < 10000; ++i) 
    {
        ++counter;  // 非原子操作，可能產生 race condition

        cout << "now counter: " << counter << endl;
    }
}
void testRaceConditionLambda()
{
    // 這裡可以實現一個簡單的競爭條件測試
    // 使用 Ishtar::Thread 來創建多個執行緒，並共享一個變數
    // 觀察是否會出現競爭條件問題

    counter = 0; // 重設計數器

    // 使用 lambda 函式包裝 increment 函式
    Ishtar::Thread t1;
    Ishtar::Thread t2;

    t1.Start("IncrementThread1", []() -> int { 
        incrementLambda();
        return 0; 
    });

    t2.Start("IncrementThread2", []() -> int { 
        incrementLambda();
        return 0; 
    });

    t1.Join();
    t2.Join();

    std::cout << "Final counter: " << counter << std::endl;
}


// Test Race Condition
//mutex mutexLock;
//mutexLock.lock();
//mutexLock.unlock();

void voidIncrement() 
{
    Ishtar::Mutex mutexLock; // 建立一個 Mutex 變數

    for (int i = 0; i < 100000; ++i)
    {
        ++counter;  // 非原子操作，可能產生 race condition

        cout << "now counter: " << counter << endl;
    }
}
int increment()
{
    for (int i = 0; i < 10; ++i)
    {
        ++counter;  // 非原子操作，可能產生 race condition

        cout << "now counter: " << counter << endl;
    }

    return EXECUTE_EXIT;
}
void testRaceCondition() 
{
    // 使用 Ishtar::Thread 來創建多個執行緒，並共享一個變數
    // 觀察是否會出現競爭條件問題

    counter = 0; // 重設計數器
    const int numThread = 1000;
    Ishtar::Thread threads[numThread];

    for (int i = 0; i < numThread; i++)
    {
        threads[i].Start("IncrementThread", increment);
    }

    // 然後等待所有執行緒完成
    for (int i = 0; i < numThread; i++)
    {
        threads[i].Join();
    }

    std::cout << "Final counter: " << counter << std::endl;


    //Ishtar::Thread t1;
    //Ishtar::Thread t2;
    //Ishtar::Thread t3;

    //t1.Start("IncrementThread1", increment);
    //t2.Start("IncrementThread2", increment);
    //t3.Start("IncrementThread3", increment);

    //t1.StartOnce("IncrementThread1", voidIncrement);
    //t2.StartOnce("IncrementThread2", voidIncrement);
    //t3.StartOnce("IncrementThread3", voidIncrement);

    //t1.Join();
    //t2.Join();
    //t3.Join();

    //t1.Start("IncrementThread1", increment);
    //t2.Start("IncrementThread2", increment);
    //t3.Start("IncrementThread3", increment);
 
    // 不管 increment 函式是否返回 EXECUTE_CONTINUE(0) 還是 EXECUTE_EXIT(1)，這裡都會繼續執行
    //t1.StartOnce("IncrementThread1", increment);
    //t2.StartOnce("IncrementThread2", increment);
    //t3.StartOnce("IncrementThread3", increment);

    //t1.StartLoop("IncrementThread1", increment, 3);
    //t2.StartLoop("IncrementThread2", increment, 3);
    //t3.StartLoop("IncrementThread3", increment, 3);

    //t1.Abort();
    //t2.Abort();
    //t3.Abort(); // 中止 t3 執行緒
}


// Test Deadlock 
class Car
{
public:

    Car(const string& name, Mutex* selfMutex, Mutex* rightMutex)
        : m_name(name), m_selfMutex(selfMutex), m_rightMutex(rightMutex) {}

    int operator()() 
    {
        ISHTAR_GET_MUTEX_LOCK(selfLock, *m_selfMutex);
        cout << m_name << " 已鎖住自己的 mutex" << endl;

        // 模擬等待
        //Ishtar::Thread::Sleep(100);

        // 再嘗試鎖右邊的 mutex（這裡會造成死結）
        ISHTAR_GET_MUTEX_LOCK(rightLock, *m_rightMutex);
        cout << m_name << " 已鎖住右邊的 mutex" << endl;

        // 模擬通過路口
        cout << m_name << " 通過路口" << endl;

		return EXECUTE_CONTINUE;
    }

private:
    string m_name;
    Mutex* m_selfMutex;
    Mutex* m_rightMutex;

};
void testDeadlock() 
{
    // 建立四個 mutex
    Ishtar::Mutex mutexA, mutexB, mutexC, mutexD;

    // 建立四台車，並設定每台車的「自己」與「右邊」的 mutex
    Car carA("CarA", &mutexA, &mutexB);
    Car carB("CarB", &mutexB, &mutexC);
    Car carC("CarC", &mutexC, &mutexD);
    Car carD("CarD", &mutexD, &mutexA);

    // 解鎖
    //Car carA("CarA", &mutexA, &mutexB);
    //Car carB("CarB", &mutexB, &mutexC);
    //Car carC("CarC", &mutexC, &mutexD);
    //Car carD("CarD", &mutexA, &mutexD);


    // 啟動四個執行緒，模擬四台車同時進入路口
    Ishtar::Thread tA, tB, tC, tD;

    tA.StartLoop("tCarA", boost::bind(&Car::operator(), &carA));
    tB.StartLoop("tCarB", boost::bind(&Car::operator(), &carB));
    tC.StartLoop("tCarC", boost::bind(&Car::operator(), &carC));
    tD.StartLoop("tCarD", boost::bind(&Car::operator(), &carD));
    
    tA.Join();
    tB.Join();
    tC.Join();
    tD.Join();

    cout << "所有車輛執行緒結束（理論上會死結卡住）" << endl;
}

class CarTwo
{
public:

    CarTwo(const string& name, vector<Mutex*>& allMutexes)
        : m_name(name), m_allMutexes(allMutexes) {}

    int operator()()
    {
        cout << "===========" << m_name << " 嘗試依序鎖定所有 Mutex..." << " ===========" << endl;

        vector<Ishtar::Mutex::UniqueLock> locks;

        for (auto mtx : m_allMutexes) 
        {
            locks.emplace_back(*mtx);
            cout << m_name << " 已鎖定 mutex " << mtx << endl << endl;
            //Ishtar::Thread::Sleep(50); // 觀察用
        }

        cout << m_name << " 通過路口" << endl;

        return EXECUTE_CONTINUE;
    }
private:
    string m_name;
    vector<Mutex*> m_allMutexes;
};
void testSolDeadlock()
{
    Ishtar::Mutex mutexA, mutexB, mutexC, mutexD;

    vector<Mutex*> allMutexes = { &mutexA, &mutexB, &mutexC, &mutexD };

    CarTwo carA("CarA", allMutexes);
    CarTwo carB("CarB", allMutexes);
    CarTwo carC("CarC", allMutexes);
    CarTwo carD("CarD", allMutexes);

    Ishtar::Thread tA, tB, tC, tD;

    tA.StartLoop("tCarA", boost::bind(&CarTwo::operator(), &carA));
    tB.StartLoop("tCarB", boost::bind(&CarTwo::operator(), &carB));
    tC.StartLoop("tCarC", boost::bind(&CarTwo::operator(), &carC));
    tD.StartLoop("tCarD", boost::bind(&CarTwo::operator(), &carD));

    tA.Join();
    tB.Join();
    tC.Join();
    tD.Join();

    std::cout << "Final counter: " << counter << std::endl;
}



void testPointerReferenceBind()
{
    Ishtar::ThreadPool::Startup(2);

    Ishtar::ThreadPool::WorkPtr wPtr1;
    Ishtar::ThreadPool::WorkPtr wPtr2;
    Ishtar::ThreadPool::WorkPtr wPtr3;

    ThreadPoolWorkClass tpwc;
	int num = 0;
	int* numPtr = &num;
    
    tpwc.counter = 0;

    //wPtr1 = ISHTAR_TP_SUBMIT_WORK(ThreadPoolWorkClass::Execute3, (tpwc, num), ());
    //wPtr2 = ISHTAR_TP_SUBMIT_WORK(ThreadPoolWorkClass::Execute4, (tpwc, numPtr), ());
    wPtr3 = ISHTAR_TP_SUBMIT_WORK(ThreadPoolWorkClass::Execute5, (tpwc, tpwc), ());

	Sleep(1000); // 等待 1 秒鐘，讓工作完成

    tpwc.counter = 20;
    
	//num = 20; // 修改 num 的值
}


// ===================================================== //

void testDatabaseManager()
{

    //Gt2::DataReader dr = ds->ExecuteReader(commandText);

    //int column1;
    //dr.BindColumn<Gt2::SqlInt>(1, column1);

    //std::cout << "DataReader : " << std::endl;
    //while (dr.FetchNext())
    //{
    //    std::cout << "Employee ID: " << column1 << std::endl;
    //}





    //dr.BindColumns(
    //    Gt2::DataColumn<Gt2::SqlNvarchar<20>>(name)
    //);



    //const std::wstring commandText = Ishtar::Swprintf(L"CREATE TABLE IF NOT EXISTS TableTest1 (EmployeeID INT PRIMARY KEY, EmployeeName NVARCHAR(50))");
    //const wstring commandText = Ishtar::Swprintf(L"{?=CALL TableTest1_GetAllEmployeeID};");
    //Gt2::DataReader reader = dataManagerControl->GetDataSource()->ExecuteReader(L"{ ?= CALL JawenTest_TryLogin(?, ?)};", params);

}
void testExecuteNonQuery(Gt2::DataSource* ds)
{
    // 使用 ExecuteNonQuery 時，需要預先設定Ishtar::ThreadPool::Startup();
    // 因為 OdbcDataSource::DemandExecution 會去呼叫，ISHTAR_TP_SUBMIT_WORK

    // 要注意帶入的參數，如果是指標需要確保 不會事先釋放，或是確保 該指標能夠自動釋放
    // 否則有可能發生存取不該存取的記憶體，或是該指標不知道什麼時候釋放

	cout << "do testExecuteNonQuery..." << endl;

    Gt2::DataParameters params;
    
    params.AddInputParameter<Gt2::SqlInt>(4);
    const std::wstring paramsName = Ishtar::Swprintf(L"Cindy");
    params.AddInputParameter<Gt2::SqlNvarchar<10>>(paramsName);
    params.AddInputParameter<Gt2::SqlInt>(20000);

    const std::wstring commandText = Ishtar::Swprintf(L"{CALL TableTest1_InsertOneData(?, ?, ?)};");
    ds->ExecuteNonQuery(commandText, params);

    cout << endl << "done testExecuteNonQuery..." << endl;
}

void testExecuteNonQuery2(Gt2::DataSource* ds)
{
    // 建立第一個 SQL 指令的參數
    Gt2::DataParameters params1;
    params1.AddInputParameter<SqlInt>(4); // 輸入參數：員工ID
    params1.AddInputParameter<SqlNvarchar<20>>(L"Cindy"); // 輸入參數：員工姓名
    params1.AddInputParameter<SqlInt>(20000); // 輸入參數：薪資

    // 執行第一個指令（新增員工資料）
    Gt2::DataSource::ExecuteStub stub1 = ds->ExecuteNonQuery
    (
        L"{CALL TableTest1_InsertOneData(?, ?, ?)};",
        params1,
        Gt2::DataSource::ExecuteStub() // 無依賴
    );


    // 建立第二個 SQL 指令的參數（含輸出參數）
    Gt2::DataParameters params2;
    params2.AddInputParameter<SqlInt>(123); // 輸入參數：員工ID
    int affectedRows = 0;
    params2.AddOutputParameter<SqlInt>(affectedRows); // 輸出參數：受影響筆數

    // 執行第二個非查詢指令（刪除員工，依賴前一個完成）
    Gt2::DataSource::ExecuteStub stub2 = ds->ExecuteNonQuery
    (
        L"{CALL TableTest1_DeleteByEmployeeID(?)};",
        params2,
        stub1 // 依賴 stub1 完成後才執行
    );


    // 等待第二個指令完成
    while (!stub2.IsComplete()) 
    {
        Ishtar::Thread::Sleep(100);
    }

    // 輸出受影響筆數
    std::cout << "Delete affected rows: " << affectedRows << std::endl;
}



void testExecuteReader(Gt2::DataSource* ds)
{
    cout << "do testExecuteReader..." << endl;

    const wstring commandText = Ishtar::Swprintf(L"{?=CALL TableTest1_GetAllEmployeeID};");
    Gt2::DataReader dr = ds->ExecuteReader(commandText);

    int column1;
    dr.BindColumn<Gt2::SqlInt>(1, column1);

    std::cout << "DataReader : " << std::endl;
    while (dr.FetchNext())
    {
        // 逐筆讀取資料
        std::cout << "Employee ID: " << column1 << std::endl;
    }

    cout << endl << "done testExecuteReader..." << endl;
}



// 定義一個資料結構，用於 BindFurtherRecord
struct PlayerInfo {
    int id;
    std::wstring name;
    int level;
    int experience;

    // 定義綁定關係
    ISHTAR_ODBC_RECORD_DEFINE_4(
        (Gt2::SqlInt, id),
        (Gt2::SqlNvarchar<50>, name),
        (Gt2::SqlInt, level),
        (Gt2::SqlInt, experience)
    )
};
void testExecuteReader2(Gt2::DataSource* ds) 
{
    try 
    {
        // 1. 基本的 ExecuteReader 和 BindColumn
        {
            const std::wstring query = L"SELECT PlayerId, PlayerName, Level FROM Players WHERE Level > 10";
            Gt2::DataReader reader = ds->ExecuteReader(query);

            int playerId;
            std::wstring playerName;
            int level;

            // 單一欄位綁定方式
            reader.BindColumn<Gt2::SqlInt>(1, playerId);
            reader.BindColumn<Gt2::SqlNvarchar<50>>(2, playerName);
            reader.BindColumn<Gt2::SqlInt>(3, level);

            std::cout << "高等級玩家列表：" << std::endl;
            while (reader.FetchNext()) {
                std::wcout << L"ID: " << playerId << L", 名稱: " << playerName
                    << L", 等級: " << level << std::endl;
            }
        }

        // 2. 使用 BindColumns 和 DataColumn
        {
            const std::wstring query = L"SELECT PlayerId, PlayerName, Level, Experience, LastLogin FROM Players WHERE PlayerId = 123";
            Gt2::DataReader reader = ds->ExecuteReader(query);

            int playerId;
            std::wstring playerName;
            int level;
            int exp;

            // 多欄位同時綁定
            reader.BindColumns(
                Gt2::DataColumn<Gt2::SqlInt>(playerId),
                Gt2::DataColumn<Gt2::SqlNvarchar<50>>(playerName),
                Gt2::DataColumn<Gt2::SqlInt>(level),
                Gt2::DataColumn<Gt2::SqlInt>(exp),
                Gt2::DataColumn<Gt2::SqlDatetime>(Gt2::Ignored()) // 忽略第5個欄位
            );

            if (reader.FetchNext()) {
                std::wcout << L"玩家資料：ID=" << playerId << L", 名稱=" << playerName
                    << L", 等級=" << level << L", 經驗值=" << exp << std::endl;
            }
        }

        // 3. 使用 BindColumnsAt 從特定位置開始綁定
        {
            const std::wstring query = L"SELECT * FROM Players WHERE PlayerId = 456"; // * 包含很多欄位
            Gt2::DataReader reader = ds->ExecuteReader(query);

            std::wstring playerName;
            int level;

            // 從第2欄開始綁定（跳過第1欄 PlayerId）
            reader.BindColumnsAt(2,
                Gt2::DataColumn<Gt2::SqlNvarchar<50>>(playerName),
                Gt2::DataColumn<Gt2::SqlInt>(level)
            );

            if (reader.FetchNext()) {
                std::wcout << L"選擇性資料：名稱=" << playerName << L", 等級=" << level << std::endl;
            }
        }

        // 4. 使用 BindFurtherRecord 綁定整個結構
        {
            const std::wstring query = L"SELECT PlayerId, PlayerName, Level, Experience FROM Players WHERE PlayerId = 789";
            Gt2::DataReader reader = ds->ExecuteReader(query);

            PlayerInfo player;
            reader.BindFurtherRecord(player);

            if (reader.FetchNext()) {
                std::wcout << L"玩家結構：ID=" << player.id << L", 名稱=" << player.name
                    << L", 等級=" << player.level << L", 經驗值=" << player.experience << std::endl;
            }
        }

        // 5. 從存儲過程取得回傳值 GetReturnValue
        {
            const std::wstring spCall = L"{?=CALL CalculatePlayerRank(789)}";
            Gt2::DataReader reader = ds->ExecuteReader(spCall);

            // 取得存儲過程的回傳值
            int rank = reader.GetReturnValue();
            std::cout << "玩家排名: " << rank << std::endl;
        }
    }
    catch (const std::exception& e) 
    {
        std::cerr << "資料庫錯誤: " << e.what() << std::endl;
    }

    //範例特點說明：
    //    1.	ExecuteReader：每個區塊都使用了 ds->ExecuteReader 執行 SQL 查詢。
    //    2.	BindColumn：展示如何綁定單個欄位，指定欄位位置和資料型別。
    //    3.	BindColumns + DataColumn：展示如何同時綁定多個欄位。
    //    4.	Ignored：在第2個範例中示範如何使用 Gt2::Ignored() 跳過不需要的欄位。
    //    5.	BindColumnsAt：展示如何從特定位置（非第1欄）開始綁定欄位。
    //    6.	BindFurtherRecord：展示如何將整個結果集綁定到自定義結構，使用 ISHTAR_ODBC_RECORD_DEFINE_4 宏來定義綁定關係。
    //    7.	GetReturnValue：在存儲過程呼叫後，使用 GetReturnValue() 獲取返回值。
    //    這個範例涵蓋了所有需要的功能，展示了如何靈活使用 DataReader 來處理不同的查詢需求。


}

void testFunc() 
{
    Gt2::DataSource* ds;
    const wstring commandText = Ishtar::Swprintf(L"{?=CALL TableTest1_GetAllEmployeeID};");
    Gt2::DataReader dr = ds->ExecuteReader(commandText);

    int column1;
    while (dr.FetchNext())
    {
        // 逐筆讀取資料
        std::cout << "Employee ID: " << column1 << std::endl;
    }
}


// ===================================================== //

int main() 
{
    cout << "主程式開始！" << endl;
    

    // ===================================================== //
    // ====================== C++ ========================== //
    // ===================================================== //

	//testHello(); // 測試 hello 函式
	//testPrintSeparator(); // 測試分隔線函式
	//testPointer(); // 測試指標和引用
	//testAddress(); // 測試引用
    //testVector(); // 測試 vector 的容量和大小
    //testMultipleInheritanceSameFuncName();
    //testAddTemplate(); // 

    // ===================================================== //
    // ================ ISHTAR Multi Thread ================ //
    // ===================================================== //

    //testThread(); // 測試線程
    //testThreadsStatus(); // 

    //testExecutable(); // 測試可執行物件
    //testThreadExecution(); // 測試 ThreadExecution
    //testThreadGroup(); // 測試 ThreadGroup

    //testThreadPool(); // 測試 ThreadPool 啟動
    //testThreadPoolWork(); // 測試 ThreadPool 和 Work
    //testThreadPoolBarrierOrder(); // 測試 ThreadPool 和 Order Barrier
    //testThreadPoolBarrierDelay(); // 測試 ThreadPool 和 Delay Barrier
    //testThreadPoolBarrierDelayOrder(); // 測試 ThreadPool 和 Delayed Order Barrier

    //testSimpleMutex(); // 測試 SimpleMutex
    //testMutex();
    //testSharedMutex();
    //testSpinMutex();
    //testMutexMarco();
    //testLockable(); // 測試 Lockable
	//testLockable2(); // 測試 Lockable2
	//testIshtarTrace(); // 測試 Ishtar Trace

    //testRaceConditionLambda(); // 測試競爭條件
    //testRaceCondition(); // 測試競爭條件
    //testDeadlock(); // 測試死鎖
    //testSolDeadlock(); // 測試解決死鎖

    //testPointerReferenceBind();

    // ===================================================== //
    // ==================== Database ======================= //
    // ===================================================== //

    Gt2::DatabaseManager dbm; // 建立資料庫連線的元件
    Ishtar::ThreadPool::Startup(10);

    Gt2::DataSource* ds; // 建立資料連線來源
    ds = dbm.OpenDataSource("DSN=sqlserver;UID=gameuser;PWD=esabd250406", 10);
    //boost::shared_ptr<Gt2::DataSource> ds(dbm.OpenDataSource("DSN=sqlserver;UID=gameuser;PWD=esabd250406", 10));

    //testExecuteNonQuery(ds);
    //ISHTAR_TP_SUBMIT_WORK(testExecuteNonQuery, (ds), ());
    ISHTAR_TP_SUBMIT_WORK(testExecuteNonQuery2, (ds), ());

    //ISHTAR_TP_SUBMIT_WORK(testExecuteReader, (ds), ());
        



    // ===================================================== //
    // ===================================================== //

    //system("pause");
    cout << "主程式結束！" << endl;
    return 0;

    // 查看目前使用的編譯器
    // std::cout << "MSVC Version: " << _MSC_VER << std::endl;
}
