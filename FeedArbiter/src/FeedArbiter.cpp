
#include <bits/stdc++.h>


using namespace std;

struct alignas(64) MarketData{
    int64_t feedId;
    uint64_t seq;
    int64_t price;
};

std::ostream& operator<<(std::ostream& os , const MarketData& update)
{
    os << "FeedId=" << update.feedId << " sequence=" << update.seq <<  " price=" << update.price ;
    return os;
}



class FeedArb{

    private:
        uint64_t m_nextseq {0}; 
        bool m_initialized= false;
        std::unordered_map< uint64_t,MarketData > message;
    
        public:
            FeedArb() = default;
        
            void OnUpdate(const MarketData& update)
            {
                if(!m_initialized)
                {
                    m_nextseq = update.seq;
                    m_initialized = true;
                }

                cout << __func__ << " " << update << endl;

                if(update.seq < m_nextseq)
                {
                    cout << __func__ << " Already Process Dropping=" << update << endl;
                    return;
                }

                if(update.seq == m_nextseq)
                {
                    Process(update); // eveything good

                    while(true){
                        auto iter = message.find(m_nextseq);
                        if(iter == message.end())
                            break; //rather then return add time of 10ms if no seq data recived on both feed send m_nextseq + 1 or which ever after m_nextseq 
                        
                        Process(iter->second);
                        message.erase(iter);
                    }

                    return;
                }

                message.emplace(update.seq, update);
            }

            void Process(const MarketData& update)
            {
                cout << __func__ << " :: " << update << endl;
                ++m_nextseq;
            }

};



int main()
{
    cout << "\n================ Test 1 : In Order ================\n";

    {
        FeedArb arb;

        arb.OnUpdate({1,100,1000});
        arb.OnUpdate({1,101,1010});
        arb.OnUpdate({1,102,1020});
    }

    cout << "\n================ Test 2 : Out Of Order ================\n";

    {
        FeedArb arb;

        arb.OnUpdate({1,100,1000});

        arb.OnUpdate({2,102,1020});
        arb.OnUpdate({2,103,1030});

        arb.OnUpdate({1,101,1010});
    }

    cout << "\n================ Test 3 : Fast Feed Wins ================\n";

    {
        FeedArb arb;

        arb.OnUpdate({1,100,1000});

        arb.OnUpdate({1,102,1111}); // first arrival
        arb.OnUpdate({2,102,2222}); // duplicate

        arb.OnUpdate({1,101,1010});
    }

    cout << "\n================ Test 4 : Already Processed ================\n";

    {
        FeedArb arb;

        arb.OnUpdate({1,100,1000});
        arb.OnUpdate({1,101,1010});

        arb.OnUpdate({2,100,9999}); // old message
    }

    cout << "\n================ Test 5 : Gap Fill ================\n";

    {
        FeedArb arb;

        arb.OnUpdate({1,100,1000});

        arb.OnUpdate({2,102,1020});
        arb.OnUpdate({2,104,1040});
        arb.OnUpdate({2,105,1050});

        arb.OnUpdate({1,101,1010});
        arb.OnUpdate({1,103,1030});
    }

    cout << "\n================ Test 6 : Reverse Arrival ================\n";

    {
        FeedArb arb;

        arb.OnUpdate({1,100,1000});

        arb.OnUpdate({2,105,1050});
        arb.OnUpdate({2,104,1040});
        arb.OnUpdate({2,103,1030});
        arb.OnUpdate({2,102,1020});
        arb.OnUpdate({2,101,1010});
    }

    cout << "\n================ Test 7 : Interview Scenario ================\n";

    {
        FeedArb arb;

        arb.OnUpdate({1,100,1000});

        arb.OnUpdate({2,102,1020});
        arb.OnUpdate({2,104,1040});
        arb.OnUpdate({2,105,1050});
        arb.OnUpdate({2,106,1060});

        arb.OnUpdate({1,101,1010});
        arb.OnUpdate({1,103,1030});
    }

    cout << "\n================ Test 8 : Start From Non-Zero Sequence ================\n";

    {
        FeedArb arb;

        arb.OnUpdate({1,1000,1000});
        arb.OnUpdate({1,1001,1001});
        arb.OnUpdate({1,1002,1002});
    }

    return 0;
}