
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

//#include "../../Ishtar/src/Ishtar/Thread/ThreadImpl.h"
//#include <boost/thread.hpp>


using namespace std;
using namespace Ishtar;


// ===================================================== //

// hello �禡
/*void hello()
{
    cout << "Hello, World!! 123" << endl;
    cout << "Hello from the hello function!" << endl;
}


// �s�W�@�� inline �禡
inline void printSeparator() {
    cout << "====================" << endl << endl;
}


// marco�w�q
#define PI 3.14159

// marco�w�q����
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

// ���ի��ЩM�ޥ�
void testPointer() {
    int a = 10;
    int* p = &a; // ���� p ���V�ܼ� a ���a�}

    cout << "Value of a: " << a << endl;
    cout << "Address of a: " << &a << endl; 

    cout << "Value pointed by p: " << *p << endl;
    cout << "Address stored in p: " << p << endl;

	cout << "Size of pointer p: " << sizeof(p) << " bytes" << endl;
}

// ���դޥ�
void testAddress() 
{

    int b = 5;
    int& a = b; // �� a �O b ���O�W(alias)

    a = 6;
	cout << "Value of b: " << b << endl; // 6
    cout << "Value of a: " << a << endl; // 6

    cout << "Address of b: " << &b << endl;
    cout << "Address of a: " << &a << endl; // ��̦a�}�ۦP
	cout << "Size of reference a: " << sizeof(a) << " bytes" << endl; // �ޥΤj�p�P���ܼƬۦP
}


template <typename T>
auto add(T a, T b) -> decltype(a + b) 
{
    return a + b;
}

// ���ռҪO�禡
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

*/

// ===================================================== //

int counter = 0;  // �@���ܼ�

// Test thread
int myThreadFunction() 
{
    // �w�q�@�Ӱ�����n���檺�禡
    cout << "������}�l����I" << endl;

    // �����u�@
    Ishtar::Thread::Sleep(1000); // ��v 1 ��

    cout << "����������I" << endl;

    return 0;

}
void testThread() 
{
    Thread thread;
    
    // �ϥ� Start �Ұʰ����
    thread.Start("MyThread", myThreadFunction);

    // ���ݰ��������
    thread.Join();
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
        cout << "������i���檫��}�l����I" << endl;

        // �����u�@
        //Ishtar::Thread::Sleep(1000); // ��v 1 ��
        if (m_counter < 100) 
        {
            ++m_counter;
			cout << "������i���檫��p�ƾ�: " << m_counter << endl;
			return EXECUTE_CONTINUE; // ��^ EXECUTE_CONTINUE �H�~�����
        }
        else 
        {
			return EXECUTE_EXIT; // ��^ EXECUTE_EXIT �H��������
        }
        
        cout << "������i���檫�󵲧��I" << endl;
    }

private:
    std::string m_name; // ������W��
    int m_counter = 0; // �p�ƾ�
};
void testExecutable()
{
    ThreadExecutableClass* tc1 = new ThreadExecutableClass("MyThreadExecutable", 0);
    Ishtar::Thread t1;

    t1.Start(tc1); // �Ұʰ����
    t1.Join(); // ���ݰ��������
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
        cout << "���������}�l�I" << endl;

        if (m_counter < 100)
        {
            ++m_counter;
            cout << "������i���檫��p�ƾ�: " << m_counter << endl;
            return EXECUTE_CONTINUE; // ��^ EXECUTE_CONTINUE �H�~�����
        }
        else
        {
            return EXECUTE_EXIT; // ��^ EXECUTE_EXIT �H��������
        }

        cout << "��������浲���I" << endl;
    }

private:
    std::string m_name; // ������W��
	int m_counter = 0; // �p�ƾ�
};
void testThreadExecution()
{
    MyThreadExecution* myExec = new MyThreadExecution("MyThreadExecution");
    myExec->Start(); // �Ұʰ����
    myExec->Join(); // ���ݰ��������

    //MyThreadExecution myExec2("MyThreadExecution2");
    //myExec2.Start(); // �Ұʰ����
    //myExec2.Join(); // ���ݰ��������
}


// Test ThreadGroup
void threadTaskFunction() 
{
    for (int i = 0; i < 1000; ++i)
    {
        ++counter;  // �D��l�ާ@�A�i�ಣ�� race condition
        cout << "now counter: " << counter << endl;
    }
}
int MyThreadGroupFunction()
{
	threadTaskFunction(); // ������Ȩ禡
    return EXECUTE_CONTINUE;
}
void MyThreadGroupVoidFunction()
{
    threadTaskFunction(); // ������Ȩ禡
}
void testThreadGroup()
{
    // ���� Ishtar::ThreadGroup ���ϥ�

    Ishtar::ThreadGroup tg;
    ThreadGroup::ExecuteFunction ef = MyThreadGroupFunction;
    ThreadGroup::VoidExecuteFunction vef = MyThreadGroupVoidFunction;

    tg.AddThread("MyThread1", ef);
    tg.AddOnceThread("MyThread2", vef);
    tg.AddLoopThread("MyThread3", vef);

    tg.Shutdown(5000); // ���� 5 �����A���Ҧ���������� // �w�] 10 ���� //

    //tg.AddThread("MyThread1", MyThreadGroupFunction);
    //tg.AddOnceThread("MyThread2", MyThreadGroupFunction);
    //tg.AddLoopThread("MyThread3", MyThreadGroupFunction, 1000);

    //boost::bind(&Ishtar::ThreadGroup::AddThread, &tg, "MyThread1", myThreadFunction)();
}


// Test ThreadPool
void threadPoolWorkFunction(int num) 
{
    int counter = 0; // �C�Ӱ�������p�ƾ�

    for (int i = 0; i < num; ++i)
    {
        ++counter;  // �D��l�ާ@�A�i�ಣ�� race condition

        cout << "work function now counter: " << counter << endl;
    }
}
class ThreadPoolWorkClass
{
public:
    void Execute(int num)
    {
        int counter = 0; // �C�Ӱ�������p�ƾ�

        for (int i = 0; i < num; ++i)
        {
            ++counter;  // �D��l�ާ@�A�i�ಣ�� race condition

            cout << "work class now counter: " << counter << endl;
        }
    }

    void Execute2(int num) 
    {
        cout << "Execute2 num: " << num << endl;
    }

};
void testThreadPool()
{
	Ishtar::ThreadPool::Startup(2); // �Ұ� ThreadPool // ���w��������ƶq

	//Ishtar::ThreadPool::AddThread(2); // ThreadPool �W�[ 2 ������� 

	//Ishtar::ThreadPool::Shutdown(); // ���� ThreadPool // �̦h���� 10 �� �פ�Ҧ��u�@
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

    
    cout << "wPtr1->IsInvoked: " << wPtr1->IsInvoked() << endl; // �ˬd�u�@�O�_ �w�Q���� //
    cout << "wPtr1->IsWorking: " << wPtr1->IsWorking() << endl; // �ˬd�u�@�O�_ ���b���� //
    cout << "wPtr1->IsComplete: " << wPtr1->IsComplete() << endl; // �ˬd�u�@�O�_ ���槹�� //

    // ����� ID // �|������Τw�Q�����A�^�� 0
    cout << "wPtr1->GetWorkingThreadId: " << wPtr1->GetWorkingThreadId() << endl;

    wPtr1->Cancel(); // �����u�@
    wPtr2->Cancel(); // �����u�@
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
	//system("pause"); // �Ȱ��{���A���ݨϥΪ̿�J
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
    //system("pause"); // �Ȱ��{���A���ݨϥΪ̿�J
	//Sleep(5000); // ���� 5 �����A���Ҧ��u�@����
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
    //system("pause"); // �Ȱ��{���A���ݨϥΪ̿�J
}


// Test SimpleMutex
void testSimpleMutex()
{
    Ishtar::SimpleMutex simpleMutex;
}

// Test Mutex
void testMutex() 
{
    // ���� Ishtar::Mutex ���ϥ�
    Ishtar::Mutex mutex;

    {
        Ishtar::Mutex::UniqueLock lock(mutex); // �����

        // �b�o�̰���ݭn�������ާ@

        cout << "�����A���椬���ާ@" << endl;
    } 
    // ��|�b UniqueLock ���R�c�禡���۰�����

    cout << "��w����" << endl;
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
        // ��w
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

        // ����
        dataLock.Unlock();
    }
private:
    int data = 0;
    Ishtar::Mutex dataMutex; //�إߤ@�� Mutex �ܼ�
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

	system("pause"); // �Ȱ��{���A���ݨϥΪ̿�J
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
        ISHTAR_LOCK_THIS();

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

    //// ���� Ishtar::Lockable ���ϥ�
    //class MyLockable : public Ishtar::Lockable<>
    //{
    //public:
    //    void DoSomething()
    //    {
    //        ISHTAR_LOCK_THIS(); // �����
    //        cout << "Doing something in MyLockable!" << endl;
    //        // �b�o�̰���ݭn�������ާ@
    //        ISHTAR_UNLOCK_THIS(); // ������
    //    }
    //};
    //MyLockable myLockable;
    //myLockable.DoSomething();
}

// Test Lockable Marco
void testLockableMarco()
{
    // �~����w������]��w����Ҧb���禡�϶��^
    //ISHTAR_LOCK_OBJECT�]obj�^
    //ISHTAR_GET_OBJECT_LOCK�]lock�Aobj�^
    // 
    // ������w������]��w��Ө禡�^
    //ISHTAR_LOCK_THIS�]�^
    //ISHTAR_GET_THIS_LOCK�]lock�^



    //// ���� Ishtar::Lockable �����ϥ�
    //class MyLockableClass : public Ishtar::Lockable<>
    //{
    //public:
    //    void DoWork()
    //    {
    //        ISHTAR_LOCK_THIS(); // �����
    //        cout << "Doing work in MyLockableClass!" << endl;
    //        // �b�o�̰���ݭn�������ާ@
    //        ISHTAR_UNLOCK_THIS(); // ������
    //    }
    //};
    //MyLockableClass myLockableClass;
    //myLockableClass.DoWork();
}


// Test ConditionVariable
void testConditionVariable() 
{

    // ���� Ishtar::ConditionVariable ���ϥ�
    Ishtar::ConditionVariable cv;
    Ishtar::Mutex mutex;

    // �b�o�̥i�H��{�@��²�檺�����ܼƴ���
    // �ϥ� Ishtar::Thread �ӳЫئh�Ӱ�����A�èϥα����ܼƶi��P�B
    // �[��O�_�ॿ�T�a�q���M���ݨƥ�o��
	// �`�N�G�o�̶ȴ��Ѥ@��²�檺�ج[�A�����{???�n�ھڻݨD�i��վ�
}


// ===================================================== //


// Test Race Condition // Lambda
void incrementLambda()
{
    for (int i = 0; i < 10000; ++i) 
    {
        ++counter;  // �D��l�ާ@�A�i�ಣ�� race condition

        cout << "now counter: " << counter << endl;
    }
}
void testRaceConditionLambda()
{
    // �o�̥i�H��{�@��²�檺�v���������
    // �ϥ� Ishtar::Thread �ӳЫئh�Ӱ�����A�æ@�ɤ@���ܼ�
    // �[��O�_�|�X�{�v��������D

    counter = 0; // ���]�p�ƾ�

    // �ϥ� lambda �禡�]�� increment �禡
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
int increment()
{
    for (int i = 0; i < 100000; ++i)
    {
        ++counter;  // �D��l�ާ@�A�i�ಣ�� race condition

        cout << "now counter: " << counter << endl;
    }

    return EXECUTE_EXIT;
}
void testRaceCondition() 
{
    // �ϥ� Ishtar::Thread �ӳЫئh�Ӱ�����A�æ@�ɤ@���ܼ�
    // �[��O�_�|�X�{�v��������D

    counter = 0; // ���]�p�ƾ�

    Ishtar::Thread t1;
    Ishtar::Thread t2;
    Ishtar::Thread t3;

    //t1.Start("IncrementThread1", increment);
    //t2.Start("IncrementThread2", increment);
    //t3.Start("IncrementThread3", increment);


    // ���� increment �禡�O�_��^ EXECUTE_CONTINUE(0) �٬O EXECUTE_EXIT(1)�A�o�̳��|�~�����
    //t1.StartOnce("IncrementThread1", increment);
    //t2.StartOnce("IncrementThread2", increment);
    //t3.StartOnce("IncrementThread3", increment);
	
    //t1.StartLoop("IncrementThread1", increment, 3);
    //t2.StartLoop("IncrementThread2", increment, 3);
    //t3.StartLoop("IncrementThread3", increment, 3);

    //t1.Abort();
    //t2.Abort();
    //t3.Abort(); // ���� t3 �����

    t1.Join();
    t2.Join();
    t3.Join();

    std::cout << "Final counter: " << counter << std::endl;
}


// ===================================================== //

int main() 
{
    cout << "�D�{���}�l�I" << endl;
    
	// testVector(); // ���� vector ���e�q�M�j�p


    //testRaceConditionLambda(); // �����v������
	//testRaceCondition(); // �����v������

    //testThread(); // ���սu�{
    //testExecutable(); // ���եi���檫��
	//testThreadExecution(); // ���� ThreadExecution
	//testThreadGroup(); // ���� ThreadGroup

	//testThreadPool(); // ���� ThreadPool �Ұ�
	//testThreadPoolWork(); // ���� ThreadPool �M Work
	//testThreadPoolBarrierOrder(); // ���� ThreadPool �M Order Barrier
    //testThreadPoolBarrierDelay(); // ���� ThreadPool �M Delay Barrier
	//testThreadPoolBarrierDelayOrder(); // ���� ThreadPool �M Delayed Order Barrier

    //testSimpleMutex(); // ���� SimpleMutex
    //testMutex();
    //testSharedMutex();
    //testSpinMutex();
    testMutexMarco();
	//testLockable(); // ���� Lockable


    cout << "�D�{�������I" << endl;
    return 0;
}
