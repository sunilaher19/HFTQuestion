

Design a RollingCounter that generates sequence numbers:

0, 1, 2, 3, 4, ...

but when it reaches a maximum value, it wraps back:

Example:

max = 7

0 1 2 3 4 5 6 7 0 1 2 3 ...

//

Now assume:

max = UINT32_MAX

Question:

Can you still implement wrapping correctly without:

if(counter == UINT32_MAX)

Think about what unsigned integer overflow guarantees in C++.

//
Now make it thread-safe.

Multiple threads call:

next()

Requirements:

Every returned value must be unique until wrap occurs.
No mutex.
Lock-free preferred.

Questions:

Which atomic operation would you use?
Is fetch_add() sufficient?
What memory ordering would you choose and why?
What happens when wrap occurs under heavy contention?

//
