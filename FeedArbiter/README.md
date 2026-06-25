



Claim offer
: Feed Arbiter

Challenge 10: Feed Arbiter
This is a very common HFT/Market Data system design problem.

Problem Statement
You receive market data updates for the same instrument from multiple independent feeds.

Example:

Feed A: Seq=100 Price=101
Feed B: Seq=100 Price=101

Feed A: Seq=101 Price=102

Feed B: Seq=102 Price=103

Feed A: Seq=102 Price=103
Each feed attempts to publish the same stream of events, but:

Messages may arrive out of order.

One feed may be faster than another.

Messages may be duplicated.

A feed may temporarily lag behind.

Network jitter exists.

Your task is to build a Feed Arbiter.

Requirements
Implement:

struct MarketUpdate
{
    uint64_t sequence;
    int64_t price;
};

class FeedArbiter
{
public:
    void onUpdate(int feedId, const MarketUpdate& update);

    bool getNext(MarketUpdate& update);
};
Functional Requirements
1. Duplicate suppression
If the same sequence number arrives from multiple feeds:

FeedA Seq=100
FeedB Seq=100
Only one copy should be emitted.

2. Monotonic output
Consumer must observe:

100
101
102
103
...
Never:

100
102
101
3. Fast feed wins
If:

FeedA -> Seq=105
FeedB -> Seq=105
Whichever arrives first should be used.

The second copy must be discarded.

4. Gap handling
Suppose:

100
101
103
arrives.

Sequence 102 is missing.

The arbiter must decide what to do.

Design the policy.

5. Multiple producers
Assume:

N feeds -> FeedArbiter
1 consumer -> getNext()
where N can be 2–8.

Performance Requirements
Very high update rate.

Minimize allocations.

Minimize locking.

Avoid unnecessary copies.

Predictable latency preferred over throughput.

Follow-up Questions
After coding, be prepared for:

How would you handle feed failover?

How would you detect a stale feed?

What data structure would you use for pending out-of-order messages?

How would you bound memory usage?

Can this be implemented lock-free?

How would you support 1 producer per feed thread?

What I will review in your solution
Correctness of sequence handling

Duplicate suppression logic

Gap strategy

Concurrency design

Choice of data structures

Memory efficiency

Latency characteristics

HFT suitability

Start with your requirements clarification and high-level design before writing code.


