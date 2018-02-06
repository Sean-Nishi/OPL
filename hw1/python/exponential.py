# Naive Python program to print all combinations of k of n items.
# Runs in time O(2^n), regardless of k (this is terrible).

# Uses bignums to represent subsets of {1, 2, ..., n}:
# bit i (starting with 1 for the least significant bit) indicates
# that i is in the set.
# By running through all numbers from 1 to 2^n-1, we can consider all
# possible subsets.

import sys

def bits(k):                # how many bits are set in k?
    rtn = 0
    while k != 0:
        if k % 2 == 1:
            rtn += 1
        k //= 2
    return rtn

def stringify(k):           # return the subset represented by k
    s = ""
    v = 1
    while k != 0:
        if k % 2 == 1:
            s += " " + str(v)
        k //= 2
        v += 1
    return s

# This is an iterator.  It /yield/s all appropriate values, one at a time,
# in a way that allows us to drive a /for/ loop.
#
def combinations(k, n):
    for i in range(0, 2**n-1):
        if bits(i) == k:
            yield stringify(i)

if len(sys.argv) != 3:
    sys.stderr.write("usage: %s k n\n" % sys.argv[0])
    sys.exit(1)

for s in combinations(int(sys.argv[1]), int(sys.argv[2])): 
    print(s)
    
