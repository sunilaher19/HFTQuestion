

#include <bits/stdc++.h>


using namespace std;


class PriceTracker{

    private:
        // void updatePrice(const std::string& symbol, int64_t price);
        // int64_t getBestPrice(const std::string& symbol);
        // void getHighestPricedSymbol(const std::string& symbol);

        std::unordered_map<std::string, int64_t> m_symbolToPrice;
        std::map<int64_t, std::unordered_set<std::string>> m_priceToSymbol;

    public:
        void updatePrice(const std::string& symbol, int64_t price)
        {
            auto symbolPriceFinder = m_symbolToPrice.find(symbol);
            if(symbolPriceFinder == m_symbolToPrice.end()) //new Updates
            {
                m_symbolToPrice[symbol] = price;
                m_priceToSymbol[price].insert(symbol); 
                return;
            }

            //old symbol , First find old price, update map with new price  and rmeobe old price
            int64_t oldPrice = symbolPriceFinder->second;

            if(oldPrice == price)
                return;
            
            m_symbolToPrice[symbol] = price;
            
            auto priceToSymbilIter = m_priceToSymbol.find(oldPrice);
            
            priceToSymbilIter->second.erase(symbol);
            
             if(priceToSymbilIter->second.empty())
                m_priceToSymbol.erase(priceToSymbilIter);
            
    
            m_priceToSymbol[price].insert(symbol);
        }
        
        int64_t getBestPrice(const std::string& symbol)
        {
            auto it = m_symbolToPrice.find(symbol);
        
            if(it == m_symbolToPrice.end())
                return -1;
        
            return it->second;
        }
        
        void removeSymbol(const std::string& symbol)
        {
            auto symbolPriceFinder = m_symbolToPrice.find(symbol);
            if(symbolPriceFinder == m_symbolToPrice.end()) //new Updates
            {
                return;
            }
            
            int oldPrice = symbolPriceFinder->second;
            m_symbolToPrice.erase(symbolPriceFinder);
            
            auto priceToSymbilIter = m_priceToSymbol.find(oldPrice);
            
            priceToSymbilIter->second.erase(symbol);
            
            if(priceToSymbilIter->second.empty())
                m_priceToSymbol.erase(priceToSymbilIter);
    
        }
                
        int64_t getHighestPrice()
        {
            if(m_priceToSymbol.empty())
                return -1;
        
            return m_priceToSymbol.rbegin()->first;
        }


        int64_t getBestPrice()
        {
             if(m_priceToSymbol.empty())
                return -1; // or throw
            return m_priceToSymbol.begin()->first;
        }

        std::string getHighestPricedSymbol()
        {
            if(m_priceToSymbol.empty())
                return "";
            auto iter  = std::prev(m_priceToSymbol.end());
            return *(iter->second.begin());
        }

};


int main()
{
    cout << "Week2 : Price Tracker" << endl;

    PriceTracker tracker;

    // =========================
    // Empty Tracker
    // =========================
    assert(tracker.getHighestPrice() == -1);
    assert(tracker.getBestPrice() == -1);
    assert(tracker.getHighestPricedSymbol() == "");
    assert(tracker.getBestPrice("AAPL") == -1);

    // =========================
    // Single Symbol
    // =========================
    tracker.updatePrice("AAPL", 100);

    assert(tracker.getBestPrice("AAPL") == 100);
    assert(tracker.getHighestPrice() == 100);
    assert(tracker.getBestPrice() == 100);
    assert(tracker.getHighestPricedSymbol() == "AAPL");

    // =========================
    // Multiple Symbols
    // =========================
    tracker.updatePrice("MSFT", 200);
    tracker.updatePrice("GOOG", 150);

    assert(tracker.getHighestPrice() == 200);
    assert(tracker.getBestPrice() == 100);
    assert(tracker.getHighestPricedSymbol() == "MSFT");

    // =========================
    // Increase Existing Symbol
    // =========================
    tracker.updatePrice("AAPL", 250);

    assert(tracker.getBestPrice("AAPL") == 250);
    assert(tracker.getHighestPrice() == 250);
    assert(tracker.getHighestPricedSymbol() == "AAPL");

    // =========================
    // Decrease Existing Symbol
    // =========================
    tracker.updatePrice("AAPL", 50);

    assert(tracker.getBestPrice("AAPL") == 50);
    assert(tracker.getHighestPrice() == 200);
    assert(tracker.getHighestPricedSymbol() == "MSFT");

    // =========================
    // Duplicate Highest Price
    // =========================
    tracker.updatePrice("GOOG", 200);

    assert(tracker.getHighestPrice() == 200);

    auto symbol = tracker.getHighestPricedSymbol();

    assert(symbol == "MSFT" || symbol == "GOOG");

    // =========================
    // No-op Update
    // =========================
    tracker.updatePrice("MSFT", 200);

    assert(tracker.getHighestPrice() == 200);

    // =========================
    // Remove Non Existing Symbol
    // =========================
    tracker.removeSymbol("TSLA");

    // =========================
    // Remove Existing Symbol
    // =========================
    tracker.removeSymbol("MSFT");

    assert(tracker.getBestPrice("MSFT") == -1);

    assert(tracker.getHighestPrice() == 200);
    assert(tracker.getHighestPricedSymbol() == "GOOG");

    // =========================
    // Remove Last Symbol
    // From Highest Price Bucket
    // =========================
    tracker.removeSymbol("GOOG");

    assert(tracker.getHighestPrice() == 50);
    assert(tracker.getHighestPricedSymbol() == "AAPL");

    // =========================
    // Remove Everything
    // =========================
    tracker.removeSymbol("AAPL");

    assert(tracker.getHighestPrice() == -1);
    assert(tracker.getBestPrice() == -1);
    assert(tracker.getHighestPricedSymbol() == "");

    // =========================
    // Shared Price Bucket Test
    // =========================
    tracker.updatePrice("AAPL", 100);
    tracker.updatePrice("MSFT", 100);

    tracker.removeSymbol("AAPL");

    assert(tracker.getHighestPrice() == 100);
    assert(tracker.getHighestPricedSymbol() == "MSFT");

    // =========================
    // Update After Removal
    // =========================
    tracker.updatePrice("MSFT", 300);

    assert(tracker.getHighestPrice() == 300);
    assert(tracker.getHighestPricedSymbol() == "MSFT");
    assert(tracker.getBestPrice("MSFT") == 300);

    cout << "All Test Cases Passed!" << endl;

    return 0;
}

