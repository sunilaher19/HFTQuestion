#include <bits/stdc++.h>

using namespace std;


struct alignas(64) OrderBookDetails{
    uint64_t OrderId;
    int64_t price;
    int64_t qty;
    int side;
    std::chrono::steady_clock::time_point timeStamp;
};

class OrderBook{

    private:
        std::unordered_map<uint64_t, std::list<OrderBookDetails>::iterator> m_orderTracker;
        std::map<int64_t , std::list<OrderBookDetails>> m_AskOrderBook;
        std::map<int64_t , std::list<OrderBookDetails>, greater<int64_t>> m_BidOrderBook;

    public:

        void AddOrder(uint64_t OrderId, int64_t price, int64_t qty, int side)
        {
            OrderBookDetails orderDetails {OrderId, price, qty, side, std::chrono::steady_clock::now()};
            if(side == 0){
                m_BidOrderBook[price].push_back(orderDetails);
                m_orderTracker[OrderId] = std::prev(m_BidOrderBook[price].end());
            }
            else{
                m_AskOrderBook[price].push_back(orderDetails);
                m_orderTracker[OrderId] = std::prev(m_AskOrderBook[price].end());
            }
        }

        void Cancel(uint64_t OrderId)
        {
            auto iter = m_orderTracker.find(OrderId);
            
            if(iter == m_orderTracker.end())
                return ;
            

            auto orderIter = iter->second;
            int price = orderIter->price;
            int side = orderIter->side;
            if(side == 0){
                
                auto bookIter = m_BidOrderBook.find(price);
                bookIter->second.erase(orderIter);
                if(bookIter->second.empty()) m_BidOrderBook.erase(bookIter);
            }
            else{
                auto bookIter = m_AskOrderBook.find(price);
                bookIter->second.erase(orderIter);
                if(bookIter->second.empty()) m_AskOrderBook.erase(bookIter);
            }
            m_orderTracker.erase(iter);
        }

        void Modify(uint64_t OrderId, int64_t price, int64_t qty, int side)
        {
            auto iter = m_orderTracker.find(OrderId);

            if(iter == m_orderTracker.end())
                return;
            

            if(iter->second->qty == qty  && iter->second->side == side && iter->second->price == price)
            {
                return;
            }
            else if(iter->second->qty < qty  && iter->second->side == side && iter->second->price == price){
                iter->second->qty = qty;
            }   
            else{
                Cancel(OrderId);
                AddOrder(OrderId, price, qty, side);
            }
        }

        bool OrderExists(uint64_t orderId) const {
            return m_orderTracker.find(orderId) != m_orderTracker.end();
        }

        std::vector<uint64_t> GetOrderIdsAtPriceLevel(int64_t price, int side) {
            std::vector<uint64_t> ids;
            if (side == 0) {
                if (m_BidOrderBook.find(price) != m_BidOrderBook.end()) {
                    for (const auto& order : m_BidOrderBook[price]) {
                        ids.push_back(order.OrderId);
                    }
                }
            } else {
                if (m_AskOrderBook.find(price) != m_AskOrderBook.end()) {
                    for (const auto& order : m_AskOrderBook[price]) {
                        ids.push_back(order.OrderId);
                    }
                }
            }
            return ids;
        }

        int64_t GetBestBid() const {
            if (m_BidOrderBook.empty()) return -1;
            return m_BidOrderBook.begin()->first;
        }

        int64_t GetBestAsk() const {
            if (m_AskOrderBook.empty()) return -1;
            return m_AskOrderBook.begin()->first;
        }
};

class OrderBookSymbol{
    
    private:
        std::unordered_map<std::string, std::unique_ptr<OrderBook>> symbolBook;    

    public:
        
        void CreateIn(std::string symbolName)
        {
            auto ob = std::make_unique<OrderBook>();
            symbolBook[symbolName] = std::move(ob);
        }

        void AddOrder(uint64_t OrderId, int64_t price, int64_t qty, int side, std::string symbolName)
        {
            auto iter = symbolBook.find(symbolName);
            if(iter == symbolBook.end())
                return;
            
            iter->second->AddOrder(OrderId, price, qty, side);
        }


        void Cancel(uint64_t OrderId, std::string symbolName)
        {
            auto iter = symbolBook.find(symbolName);
            if(iter == symbolBook.end())
                return;
            
            iter->second->Cancel(OrderId);
        }

        void Modify(uint64_t OrderId, int64_t price, int64_t qty, int side, std::string symbolName)
        {
            auto iter = symbolBook.find(symbolName);
            if(iter == symbolBook.end())
                return;
            
            iter->second->Modify(OrderId, price, qty, side);
        }

        // Validation Proxy Methods for Test Runner Verification
        int64_t TestGetBestBid(std::string symbolName) {
            auto iter = symbolBook.find(symbolName);
            return (iter != symbolBook.end()) ? iter->second->GetBestBid() : -1;
        }

        int64_t TestGetBestAsk(std::string symbolName) {
            auto iter = symbolBook.find(symbolName);
            return (iter != symbolBook.end()) ? iter->second->GetBestAsk() : -1;
        }
};


// Architecture detection macros
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__)
    #include <x86intrin.h>
#elif defined(__aarch64__)
    // No special header needed for ARM64 inline assembly assembly
#endif

inline unsigned long long get_cpu_cycles() {
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__)
    // x86 paths
    return __rdtsc();
#elif defined(__aarch64__)
    // ARM64 Assembly path for Apple Silicon
    unsigned long long virtual_timer_value;
    asm volatile("mrs %0, cntvct_el0" : "=r" (virtual_timer_value));
    return virtual_timer_value;
#else
    #error "Unsupported hardware architecture context"
#endif
}

void PrintStats(const string& opName, const vector<double>& times, const vector<unsigned long long>& cycles) {
    double totalTime = std::accumulate(times.begin(), times.end(), 0.0);
    double avgTime = totalTime / times.size();
    double maxTime = *std::max_element(times.begin(), times.end());

    unsigned long long totalCycles = std::accumulate(cycles.begin(), cycles.end(), 0ULL);
    double avgCycles = static_cast<double>(totalCycles) / cycles.size();
    unsigned long long maxCycles = *std::max_element(cycles.begin(), cycles.end());

    cout << "\n============================================\n";
    cout << " PERFORMANCE REPORT: " << opName << "\n";
    cout << "============================================\n";
    cout << " Latency (Time):\n";
    cout << "   - Average: " << avgTime << " ns\n";
    cout << "   - Maximum: " << maxTime << " ns (Worst-case spike)\n\n";
    cout << " Hardware footprint (CPU Clock Cycles):\n";
    cout << "   - Average: " << avgCycles << " cycles\n";
    cout << "   - Maximum: " << maxCycles << " cycles\n";
}


int main() {
    OrderBook ob;
    const int WARMUP_RUNS = 1000;
    const int TEST_RUNS = 10000;

    // Warm up the caches and branch predictors [cite: 173]
    for (int i = 0; i < WARMUP_RUNS; ++i) {
        ob.AddOrder(i, 100 + (i % 10), 10, 0);
        ob.Cancel(i);
    }

    cout << "--- Starting Dynamic Telemetry Framework (" << TEST_RUNS << " runs) ---" << endl;

    // 1. PROFILE: AddOrder
    vector<double> addTimes;
    vector<unsigned long long> addCycles;
    addTimes.reserve(TEST_RUNS);
    addCycles.reserve(TEST_RUNS);

    for (int i = 0; i < TEST_RUNS; ++i) {
        uint64_t id = i + WARMUP_RUNS;
        int64_t price = 100 + (i % 50); // Simulates 50 active price levels

        auto startTime = std::chrono::high_resolution_clock::now();
        unsigned long long startCycles = get_cpu_cycles();

        ob.AddOrder(id, price, 10, 0);

        unsigned long long endCycles = get_cpu_cycles();
        auto endTime = std::chrono::high_resolution_clock::now();

        addTimes.push_back(std::chrono::duration<double, std::nano>(endTime - startTime).count());
        addCycles.push_back(endCycles - startCycles);
    }

    // 2. PROFILE: Modify (In-place priority preservation)
    vector<double> modifyTimes;
    vector<unsigned long long> modifyCycles;
    modifyTimes.reserve(TEST_RUNS);
    modifyCycles.reserve(TEST_RUNS);

    for (int i = 0; i < TEST_RUNS; ++i) {
        uint64_t id = i + WARMUP_RUNS;
        int64_t price = 100 + (i % 50);

        auto startTime = std::chrono::high_resolution_clock::now();
        unsigned long long startCycles = get_cpu_cycles();

        ob.Modify(id, price, 5, 0); // Reduce quantity down to 5

        unsigned long long endCycles = get_cpu_cycles();
        auto endTime = std::chrono::high_resolution_clock::now();

        modifyTimes.push_back(std::chrono::duration<double, std::nano>(endTime - startTime).count());
        modifyCycles.push_back(endCycles - startCycles);
    }

    // 3. PROFILE: Cancel
    vector<double> cancelTimes;
    vector<unsigned long long> cancelCycles;
    cancelTimes.reserve(TEST_RUNS);
    cancelCycles.reserve(TEST_RUNS);

    for (int i = 0; i < TEST_RUNS; ++i) {
        uint64_t id = i + WARMUP_RUNS;

        auto startTime = std::chrono::high_resolution_clock::now();
        unsigned long long startCycles = get_cpu_cycles();

        ob.Cancel(id);

        unsigned long long endCycles = get_cpu_cycles();
        auto endTime = std::chrono::high_resolution_clock::now();

        cancelTimes.push_back(std::chrono::duration<double, std::nano>(endTime - startTime).count());
        cancelCycles.push_back(endCycles - startCycles);
    }

    // Print out statistical summaries
    PrintStats("ADD ORDER", addTimes, addCycles);
    PrintStats("MODIFY ORDER (Size Decrease)", modifyTimes, modifyCycles);
    PrintStats("CANCEL ORDER", cancelTimes, cancelCycles);

    return 0;
}