

/*
A feed handler receives ticks from the exchange and publishes them into a fixed-size ring buffer.
*/

#include <bits/stdc++.h>

using namespace std;


template <typename T>
class RingBuffer{

    private:
        int capacity;
        std::vector<T> vect; // Lock-free SPSC ring buffer

        struct alignas(64) CacheLineAtomic
        {
            std::atomic<size_t> value{0};
        };

        CacheLineAtomic tail{0};
        CacheLineAtomic head{0};

    public:
        RingBuffer(int capacity) : capacity(capacity){
            vect.resize(capacity);
        }

        ~RingBuffer() {}

        bool push(T data){

            size_t currTail = tail.value.load(std::memory_order_relaxed); //Memory ordering
            {
                size_t nextTail = (currTail + 1)%capacity;

                if(nextTail == head.value.load(std::memory_order_acquire))
                {
                    //Queue is full
                    return false;
                }
                vect[currTail] = data;
                tail.value.store(nextTail, std::memory_order_release);
            }
            return true;
        }

        bool pop(T& data){

            size_t currHead = head.value.load(std::memory_order_relaxed);
            {
                size_t nextHead = (currHead + 1)%capacity;

                if(currHead == tail.value.load(std::memory_order_acquire))
                {
                    //Queue is Empty
                    return false;
                }
                data = vect[currHead];
                head.value.store(nextHead, std::memory_order_release);
            }
            return true;
        }

        bool empty() const
        {
            return head.value.load(std::memory_order_acquire) == tail.value.load(std::memory_order_acquire);
        }
        bool full()
        {
            size_t currTail = tail.value.load(std::memory_order_acquire);
            size_t nextTail = (currTail + 1)%capacity;
            return nextTail == head.value.load(std::memory_order_acquire);
        }

        size_t size()
        {
            return (tail.value.load() + capacity - head.value.load()) % capacity;
        }
};

int main()
{
    // -------------------------------------------------
    // Test 1 : Basic Push / Pop
    // -------------------------------------------------
    {
        RingBuffer<int> rb(8);

        assert(rb.push(10));
        assert(rb.push(20));

        int x;

        assert(rb.pop(x));
        assert(x == 10);

        assert(rb.pop(x));
        assert(x == 20);

        assert(rb.empty());

        std::cout << "Test 1 Passed\n";
    }

    // -------------------------------------------------
    // Test 2 : Empty Queue
    // -------------------------------------------------
    {
        RingBuffer<int> rb(8);

        int x;

        assert(!rb.pop(x));
        assert(rb.empty());

        std::cout << "Test 2 Passed\n";
    }

    // -------------------------------------------------
    // Test 3 : Full Queue
    // -------------------------------------------------
    {
        RingBuffer<int> rb(8);

        for (int i = 0; i < 7; ++i)
        {
            assert(rb.push(i));
        }

        assert(rb.full());

        assert(!rb.push(100));

        std::cout << "Test 3 Passed\n";
    }

    // -------------------------------------------------
    // Test 4 : FIFO
    // -------------------------------------------------
    {
        RingBuffer<int> rb(8);

        for (int i = 1; i <= 5; ++i)
        {
            assert(rb.push(i));
        }

        int x;

        for (int i = 1; i <= 5; ++i)
        {
            assert(rb.pop(x));
            assert(x == i);
        }

        std::cout << "Test 4 Passed\n";
    }

    // -------------------------------------------------
    // Test 5 : Wrap Around
    // -------------------------------------------------
    {
        RingBuffer<int> rb(8);

        for (int i = 0; i < 5; ++i)
        {
            assert(rb.push(i));
        }

        int x;

        for (int i = 0; i < 3; ++i)
        {
            assert(rb.pop(x));
            assert(x == i);
        }

        for (int i = 5; i < 8; ++i)
        {
            assert(rb.push(i));
        }

        std::vector<int> result;

        while (rb.pop(x))
        {
            result.push_back(x);
        }

        std::vector<int> expected =
        {
            3, 4, 5, 6, 7
        };

        assert(result == expected);

        std::cout << "Test 5 Passed\n";
    }

    // -------------------------------------------------
    // Test 6 : Size Validation
    // -------------------------------------------------
    {
        RingBuffer<int> rb(8);

        assert(rb.size() == 0);

        rb.push(1);
        assert(rb.size() == 1);

        rb.push(2);
        assert(rb.size() == 2);

        int x;

        rb.pop(x);
        assert(rb.size() == 1);

        rb.pop(x);
        assert(rb.size() == 0);

        std::cout << "Test 6 Passed\n";
    }

    // -------------------------------------------------
    // Test 7 : Wrap Around Size
    // -------------------------------------------------
    {
        RingBuffer<int> rb(8);

        for (int i = 0; i < 5; ++i)
        {
            rb.push(i);
        }

        int x;

        for (int i = 0; i < 3; ++i)
        {
            rb.pop(x);
        }

        assert(rb.size() == 2);

        rb.push(10);
        rb.push(11);

        assert(rb.size() == 4);

        std::cout << "Test 7 Passed\n";
    }

    // -------------------------------------------------
    // Test 8 : SPSC Stress Test
    // -------------------------------------------------
    {
        constexpr int N = 1'000'000;

        RingBuffer<int> rb(1024);

        std::thread producer([&]()
        {
            for (int i = 0; i < N;)
            {
                if (rb.push(i))
                {
                    ++i;
                }
            }
        });

        std::thread consumer([&]()
        {
            int expected = 0;
            int value;

            while (expected < N)
            {
                if (rb.pop(value))
                {
                    assert(value == expected);
                    ++expected;
                }
            }
        });

        producer.join();
        consumer.join();

        std::cout << "Test 8 Passed (SPSC Stress)\n";
    }

    std::cout << "\nAll Tests Passed Successfully\n";

    return 0;
}