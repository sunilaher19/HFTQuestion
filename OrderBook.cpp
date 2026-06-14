#include <bits/stdc++.h>
#include <x86intrin.h> // Contains __rdtsc()

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


int main()
{
    OrderBook ob;
    
    // Add multiple bids
    unsigned long long startCycles = __rdtsc();
    ob.AddOrder(1, 100, 10, 0);
    ob.AddOrder(2, 105, 5, 0); // Higher bid
    ob.AddOrder(3, 98, 20, 0);  // Lower bid
    
    // Bids must be sorted descending: Best Bid should be 105
    assert(ob.GetBestBid() == 105);
    
    // Add multiple asks
    ob.AddOrder(4, 200, 15, 1);
    ob.AddOrder(5, 195, 10, 1); // Lower ask (cheaper seller)
    ob.AddOrder(6, 205, 25, 1); // Higher ask
    
    // Asks must be sorted ascending: Best Ask should be 195
    assert(ob.GetBestAsk() == 195);
    
    std::cout << "[PASS] TestOrderInsertionAndSorting" << std::endl;
    unsigned long long endCycles = __rdtsc();

    std::cout << "Cycles elapsed: " << (endCycles - startCycles) << " cycles.\n";
    return 0;

}