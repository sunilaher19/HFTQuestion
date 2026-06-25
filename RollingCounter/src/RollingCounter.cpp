
#include <bits/stdc++.h>

using namespace std;


class RollingContract{

    private:
        alignas(64) std::atomic<uint64_t> counter{0}; 
        uint64_t maxValue;

    public:
        RollingContract(uint64_t count): maxValue(count) {}

        uint64_t next()
        {
            uint64_t desired,count;
            do
            {
                 count = counter.load(std::memory_order_relaxed);
                 desired =
                (count == maxValue)
                ? 0
                : count + 1;

            }while(!counter.compare_exchange_weak(count, desired, std::memory_order_relaxed));

            return count;
        }
};


void Run(RollingContract& roll)
{
    for(int i =0 ; i< 100 ; ++i)
    {
        cout << std::hex << "threadid=" << std::this_thread::get_id() << " : " << roll.next() << endl;
        std::this_thread::sleep_for(500ms);
    }
}

int main()
{
    RollingContract roll(10);
    std::thread t[3];
    

    for(int i =0; i < 3 ; ++i)
    {
        t[i] = std::thread(Run, std::ref(roll));
    }

    for(int i =0; i < 3 ; ++i)
    {
        t[i].join();
    }

    return 0;
}
