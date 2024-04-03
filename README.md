# Assignment 12: Simple Blockchain

A blockchain is a popular data structure that is used in many applications, most notably in the
world of cryptocurrencies. A blockchain is a data structure that consists of blocks, where each
block contains data. Each block also contains a hash of the previous block, which creates a chain of
blocks. This is a simple way to ensure that the data in the blocks is not tampered with. If you
change the data in one block, the hash of that block will change, and it will no longer match the
hash stored in the next block. Thus, if you want to successfully tamper with the data in a block,
you need to tamper with all the blocks that come after it, which can be a computationally
challenging task.

To make it even more challenging, a typical blockchain requires that newly-generated blocks meet
certain conditions. For example, Bitcoin, a popular cryptocurrency, has a *target hash* and new
blocks must have a hash that is less than or equal to the target. To allow this, each block also
contains a *nonce*, which is a number that one can vary to create a hash that satisfies the
condition. As you can imagine, it is computationally challenging to find a such nonce. Thus, the
process of finding a correct nonce has a fitting name, *mining*, analogous to mining for precious
metals such as gold. One needs to try a very large number of different nonces to find the one that
creates a hash that satisfies the condition. In Bitcoin, miners are rewarded with new Bitcoins for
successfully mining a new block.

In this assignment, you will implement a simple blockchain.

## Block

Each block is a data structure defined in [`include/blockchain.h`](include/blockchain.h). It
contains the following fields.

* `core`: the core of the block that contains the following fields.
    * `index`: the index of the block in the blockchain.
    * `timestamp`: the time when the block was created.
    * `data`: the data that the block contains.
    * `p_hash`: the hash of the previous block.
    * `nonce`: a number that is used to create a hash that satisfies a certain condition.
* `hash`: the hash of the current block.

Each block should satisfy the following conditions.

* The hash of a block is created by hashing the block core (i.e., the entire `struct block_core`)
  using SHA256.
* `index` should start from `0` (for the first block) and increase monotonically by one for each new
  block.
* `timestamp` should contain the creation time of each block. You should use `clock_gettime()` with
  `CLOCK_REALTIME` to get the current time. (`man clock_gettime` for more information.)
* `data` should store the bytes passed as an argument for the `bc_add_block()` function described
  below.
* The hash of a block should be less than or equal to the target hash (`difficulty`) specified in
  the `bc_init()` function described below. Your program needs to find a correct nonce by trying
  different nonces until it finds the one that creates a hash that satisfies the difficulty. In a
  typical blockchain, the target hash decreases over time to make it more and more difficult to find
  a correct nonce. However, in this assignment, the target hash is fixed per blockchain and you
  don't need to worry about changing it. The target hash can differ between blockchains, but it is
  fixed for each blockchain.
* The hash of the current block should match the `p_hash` field of the next block.
* The `p_hash` field of the first block is `0`. You still need to find the correct nonce for the
  first block that satisfies the hash condition.

## Blockchain

A blockchain is a data structure that contains an array of blocks. It is defined in
[`include/blockchain.h`](include/blockchain.h). Along with it, you need to implement the following
functions.

* `bc_init()`: initializes the blockchain. It should initialize the buffer passed as the argument
  and use the hash target also passed as an argument (`difficulty`). If `difficulty` is `NULL`, it
  means that there is no target hash. `bc_init()` should also perform any initialization necessary
  for your code. The function should return `0` for success and `-1` for failure.
* `bc_add_block()`: adds a new block to the blockchain. It should create a new block, set the fields
  of the block, and find the correct nonce that satisfies the hash condition. It should then add the
  block to the blockchain. It should return the index of the new block for success and `-1` for
  failure.
* `bc_verify()`: verifies the blockchain. It should check if the hash of each block is correct and
  matches the `p_hash` field of the next block. It should also check if the hash of each block
  satisfies the difficulty condition. It should return `0` for success and `-1` for failure.

## Implementation

* Nonce: you need to implement your own way of finding a correct nonce. You can use any method you
  like, but you need to find a nonce that satisfies the hash condition.
* SHA256: you need to use the OpenSSL `libcrypto` library to generate a SHA256 hash. OpenSSL's man
  page for
  [ossl-guide-libcrypto-introduction](https://www.openssl.org/docs/man3.2/man7/ossl-guide-libcrypto-introduction.html)
  provides a good introduction to using algorithms in applications. Especially, the section [Using
  Algorithms in
  Applications](https://www.openssl.org/docs/man3.2/man7/ossl-guide-libcrypto-introduction.html#USING-ALGORITHMS-IN-APPLICATIONS)
  provides an example that shows how to generate a SHA256 hash.
* You need to link your code against `libcrypto` as well.
* You need to link against a shared library under `lib/` that the test cases use, which is
  platform-dependent. You need to use the appropriate one for your platform.
    * For Intel machines, use `libcheck-x86_64.so`.
    * For ARM machines, use `libcheck-aarch64.so`.

## Code Structure and CMake

* You need to use the same code structure as previous assignments with `src/` and `include/`.
* You also need to write `CMakeLists.txt` that produces one executable, `blockchain`. The executable
  should run the main function in `src/main.c`.
* You need to set the `CC` and `CXX` environment variables to `clang` and `clang++`. If you haven't,
  open your ~/.zshrc and add the following two commands at the end.

  ```bash
  export CC=$(which clang)
  export CXX=$(which clang++)
  ```

* When you open a source file, you might see an error saying that a header file is not found. If
  that's the case, generate a `compile_commands.json` file by adding the following line to your
  `CMakeLists.txt` and running `cmake`.

    ```cmake
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    ```

## Grading

* [40 pts] Blockchain with no difficulty
    * [20 pts] `bc_add_block()` successfully adds one block (the first block).
    * [20 pts] `bc_add_block()` successfully adds multiple blocks.
* [60 pts] Blockchain with a difficulty
    * [20 pts] `bc_add_block()` successfully adds one block (the first block).
    * [20 pts] `bc_add_block()` successfully adds multiple blocks.
    * [20 pts] `bc_verify()` successfully verifies the blockchain.
* Code that does not compile with CMake gets a 0.
* Code that does not generate all the required executables gets a 0.
* Memory issues have a penalty of -10 pts. Use appropriate sanitizers and enable the debug option.
  Make sure your code works without the debug option as well. Before you submit, make sure that you
  remove the debug option.
* A wrong code directory structure has a penalty of -10 pts.
* Thread/synchronization issues have a penalty of -10 pts.
* You should not hard-code or customize your implementation tailored to our test cases. Generally,
  you should consider the provided test cases as examples or specific instances of general cases.
