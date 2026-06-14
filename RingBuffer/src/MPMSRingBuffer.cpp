

#include <bits/stdc++.h>


using namespace std;

class MPMS
{
    private:
        struct alignas(64) CacheLineAtomic
        {
            std::atomic<size_t> value{0};
        }
        CacheLineAtomic head{0};
        CacheLineAtomic tail{0};

        std::vector<T> buffer;
    
    public:
        
}


int main()
{
    return 0;
}