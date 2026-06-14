#include <bits/stdc++.h>


using namespace std;

class ITask
{
public:
    virtual void Execute(int64_t eventId) = 0;
};


class ThreadTask : public ITask{

    public:
        void Execute(int64_t eventId){
            cout << " Event=" << eventId << endl;
        }

        void Run(int64_t eventId, int64_t ts){
            
        }


};


template < typename T>
struct EventDetails{
    int64_t eventId;
    std::shared_ptr<T> object;
};


template < typename T>
class EventScheudle{

    private:
        std::map<std::chrono::steady_clock::time_point ,EventDetails<T>> m_timeToEventpair;
        std::unordered_map<int64_t, std::chrono::steady_clock::time_point> m_eventToTime;
        std::thread exuectionThread;
        atomic<bool> isRunning{false};


    public:

        EventScheudle(){
            isRunning.store(true);
        }
        ~EventScheudle() = default;

        
        void Schedule(int64_t eventId, int64_t timeSecond, std::shared_ptr<T> object)
        {
            auto executeAt = std::chrono::steady_clock::now() + std::chrono::seconds(timeSecond);
            m_timeToEventpair[executeAt] = {eventId, object};
            m_eventToTime[eventId] = executeAt;
        }

        void Cancel(int64_t id){
            auto executeTime = m_eventToTime[id];
            m_timeToEventpair.erase(executeTime);
            m_eventToTime.erase(id);
        }

        void Reschedule(int64_t eventId, int64_t newExecuteAt)
        {
            auto executeAt = std::chrono::steady_clock::now() + std::chrono::seconds(newExecuteAt);
            auto executeTime = m_eventToTime[eventId];
            m_eventToTime.erase(eventId);
            
            Schedule(eventId, newExecuteAt, m_timeToEventpair[executeTime].object);

            m_timeToEventpair.erase(executeTime);
            
        }

        size_t PendingEventCount() const
        {
            return m_timeToEventpair.size();
        }

        void Start(){
            exuectionThread = std::thread(&EventScheudle::Run , this);
        }

        void Stop(){
            isRunning.store(false);
            exuectionThread.join();
            
        }

        void Run()
        {
            while(isRunning.load(std::memory_order_acquire)){
                
                auto now = std::chrono::steady_clock::now();
                if(!m_timeToEventpair.empty() && now >= m_timeToEventpair.begin()->first)
                {
                    auto executeTime = m_timeToEventpair.begin()->first;
                    auto object = m_timeToEventpair.begin()->second.object;
                    auto eventId = m_timeToEventpair.begin()->second.eventId;
                    object->Execute(eventId);
                    m_timeToEventpair.erase(executeTime);
                    m_eventToTime.erase(eventId);
                }
            }
        }


};






int main()
{
    cout << "Starting Even Schedule for Muktiple Thread" << endl;
}