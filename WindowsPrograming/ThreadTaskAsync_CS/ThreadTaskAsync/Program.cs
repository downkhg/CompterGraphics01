using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;

namespace ThreadTaskAsync
{
    class TestThread
    {
        public int nId;

        public TestThread(int id)
        {
            nId = id;
        }

        public void ThreadCallBack()
        {
            for (int i = 0; i < 9999; i++)
            {
                //Console.WriteLine(nId+":ThreadCallBack:" + i);
            }
        }
    }

    class Program
    {
        //각 쓰레드 풀을 2000개씩 만들어 작동시키기고 끝날때까지 프로그램 지연하기
        static int nFullCount = 4000;
        //쓰레드가 많아지면 느려진다.
        static void ThreadTestMain()
        {
            //쓰레드를 지정한 개수만큼 만들어서 처리
            List<Thread> listThread = new List<Thread>(nFullCount);
            int nTime =  DateTime.Now.Millisecond;
            for (int i = 0; i < nFullCount; i++)
            {
                TestThread testThread = new TestThread(i);
                ThreadStart threadStart = new ThreadStart(testThread.ThreadCallBack);
                Thread thread = new Thread(threadStart);
                listThread.Add(thread);
                thread.Start();
            };
            //모든 스레드가 종료될때까지 대기한다.
            for (int i = 0; i < nFullCount; i++)
            {
                listThread[i].Join();
            };

            Console.WriteLine("ThreadDelayTime:{0}", DateTime.Now.Millisecond - nTime);
        }
        //내부에서 스레드를 관리하므로 쓰레드에비해 빠르게 처리된다.
        static void TaskTestMain()
        {
            //테스크를 지정개수만큼 만들어서 처리
            List<Task> listTask = new List<Task>(nFullCount);
            int nTime = DateTime.Now.Millisecond;
            for (int i = 0; i < nFullCount; i++)
            {
                TestThread testThread = new TestThread(i);
                Task thread = new Task(testThread.ThreadCallBack);
                listTask.Add(thread);
                thread.Start();
            };
            //모든 스레드가 종료될때까지 대기한다.
            for (int i = 0; i < nFullCount; i++)
            {
                listTask[i].Wait();
            };
            Console.WriteLine("TaskDelayTime:{0}",  DateTime.Now.Millisecond - nTime);
        }

        static void Main(string[] args)
        {
            ThreadTestMain();
            //TaskTestMain();
        }
    }
}
