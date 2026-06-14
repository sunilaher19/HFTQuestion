
#include <bits/stdc++.h>

using namespace std;

struct alignas(64) MarketData{
    int64_t bid;
    int64_t ask;
    int64_t qty;
};

std::ostream& operator<<(std::ostream& os, const MarketData& mkt)
{
    os << mkt.bid << " "
       << mkt.ask << " "
       << mkt.qty;

    return os;
}

template<typename T>
class SeqLock
{

private:
    T m_data;
    std::atomic<uint64_t> m_seq{0};

public:
    void write(const T& value){
         m_seq.fetch_add(1, std::memory_order_relaxed);
        m_data = value;
        m_seq.fetch_add(1, std::memory_order_release);
    }

    T read() const{
        return m_data;
    }

    uint64_t getSequence()
    {
        return m_seq.load(std::memory_order_acquire);
    }
};

template<typename T>
void reader(SeqLock<T>& seq, std::atomic<bool>& running)
{
    while (running.load(std::memory_order_acquire))
    {
        uint64_t seq1, seq2;
        T data;

        do {
            seq1 = seq.getSequence();

            data = seq.read();

            seq2 = seq.getSequence();

        } while (seq1 != seq2 || (seq1 & 1));

        cout << "data=" << data << " seq=" << seq1 << endl;
    }
}

template<typename T>
void writer(SeqLock<T>& seq, std::atomic<bool>& running)
{
    for(int i=0 ; i < 100; ++i)
    {
        MarketData mkt;
        mkt.bid = std::rand() % 1000;
        mkt.ask = std::rand() % 1000;
        mkt.qty = std::rand() % 1000;
        seq.write(mkt);
        std::this_thread::sleep_for(std::chrono::microseconds(50));
    }
}

int main()
{
    SeqLock<MarketData> seq;
    std::atomic<bool> running{true};
    std::thread t1(writer<MarketData>, std::ref(seq), std::ref(running));
    std::thread t2(reader<MarketData>, std::ref(seq), std::ref(running));

    t1.join();
    running.store(false, std::memory_order_release);
    t2.join();
    return 0;
}
