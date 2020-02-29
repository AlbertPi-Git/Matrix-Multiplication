# Matrix Multiplication Implementation

## Introduction
This is a naive multi-threading matrix multiplication implementation.

## Usage
The implementation is only included in "MatMul.h" header file, so the only thing to do is include the header file to use the "library". Then use "g++ -O3 -std=c++11 -pthread" to compile the file which includes the library.

## Matrix representation and interfaces
The representation of the matrix chosen here is nested vector container: 
```c++
std::vector<std::vector<type>>
```

The interface of transposition operation is 
```c++
MatrixTranspose(std::vector<std::vector<type>>& m, std::vector<std::vector<type>>& res)
```

The interface of matrix multiplication is 
```c++
MatrixMul(std::vector<std::vector<type>> &m1, std::vector<std::vector<type>> &m2, std::vector<std::vector<type>>&res)
```

## Implementation Detail
The implementation of transposition is a straightforward two nested loop based implementation.

The implementation of multiplication is more tricky. 

1.  Firstly, the one of the matrix is transposed to greatly increase the cache hit rate when we do the multiplication. The time complexity of transposition is only O(N^2), while the complexity of multiplication is O(N^3), so even if transposition doesn't improve the cache performance, it still won't change the time complexity of the whole multiplication operation. But according to my test, do the pre-transposition operation before multiplication can bring 5x-6x speed up.

2.  Secondly, the concurrent threads of target machine is checked before starting the multiplication. If the machine has multi cores or single core multi threads, then multi threads are used to different part of multiplication in parallel. The speed up of multi threading depends on specification of machine, the size of two matrices (since multi threading has overhead, so if two matrices are very small, the speed up may also be small. It may be even a little bit slower than single thread multiplication in some extreme cases.) 

## Testing
The correctness of matrix multiplication is validated by a tester. (The implementation of Transposition is so straightforward, so the testing is trivial and not included in tester) The result of multiplication is compared with the result computed by numpy library in Python. Simply execute run.bash will start the automatic test.