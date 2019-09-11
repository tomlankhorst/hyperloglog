HyperLogLog implementation in C++
=====

A [HyperLogLog](https://en.wikipedia.org/wiki/HyperLogLog) is a probabilistic structure that estimates the number of unique values (cardinality) in a set with relatively little memory.

How it works
------

What are chances that a random binary number starts with a zero? And with two zeroes? Three?

Exactly, the chance that it starts with `n` zeroes is: `P(n) = (1/2)^n`.

Consequently, to find a value that start with `n` zeroes, we need on average `2^n` values, so `2^n` is an estimate of the number of values encountered when the maximum number of zeroes is `n`.

Now, a HyperLogLog hashes values to associate a specific random number to it and uses this number for two purposes:

1. The first `B` bits are used to associate a _bucket_ number between `1` and `2^B`
2. The remaining bits are used to count the number of left-most zeroes. If this number is higher than the current maximum in the bucket, the bucket value is updated to the new value.

So, a HyperLogLog tracks `2^B` times the maximum number of leading zeroes found and a corresponding estimate of unique values. To find a single estimate, it returns the _harmonic mean_ of the bucket's estimates as the grand estimate.
There is a correction for small and low cardinality and for hash collisions. 

Example
-----

32 bit hashes with 4 bucket bits  and thus 16 buckets

```
value hash       buck 0s  bucks [0-15]
------------------------------------------------
foo   1100|100.. 12   0   0 0 0 .. 0
bar   1001|001.. 9    2   0 0 0 .. [9] 2 .. 0
baz   0010|010.. 2    1   0 0 1 .. [9] 2 .. 0
hi    0100|111.. 4    0   0 0 1 .. [9] 2 .. 0
lolo  1010|010.. 10   1   0 0 1 .. [9] 2 1 .. 0
          ,,                      ,, 
```              