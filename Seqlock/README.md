template<typename T>
class SeqLock
{
public:
    void write(const T& value);
    T read() const;
};



Writer Requirements

When writing:

Increment sequence number → make it odd
Update the data
Increment sequence number again → make it even

seq = 100

Writer starts
seq = 101   // odd

update data

seq = 102   // even
Writer ends


Reader Requirements

Reader must:

Read sequence number (seq1)
If seq1 is odd → retry
Read the data
Read sequence number again (seq2)
If seq1 != seq2 → retry
Otherwise return the data


Constraints
No std::mutex
No std::shared_mutex
Use std::atomic
Multiple concurrent readers
Single writer
Reader must never block
Reader may retry
Correct memory ordering
Return only a consistent snapshot of T