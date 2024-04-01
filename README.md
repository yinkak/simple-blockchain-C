# Assignment 12: Applications for Cryptography Functions

As discussed in class, there are three popular types of cryptography, cryptographic hashing, secret
key cryptography, and public key cryptography. In this assignment, you will do some activities to
gain a better understanding of those functions.

## Task 0: SSH Authentication with Public Key Cryptography

There are many applications of public key cryptography and one popular application is SSH
authentication/login. You might have done this before for other classes, but with our class
discussion, you can now understand what is going on.

Before we proceed further, make sure that you are ***not*** recording. This part involves your own
sensitive data such as your password and you ***should not*** record anything.

* First, log in to a CSIL machine from our VM. But again, ***do not*** record anything. Note that
  you need to first use the VPN on your host (*not* on our VM) to be able to log in.
* You can use one of the [Linux CPU
  servers](https://www.sfu.ca/computing/about/support/csil/unix/how-to-use-csil-linux-cpu-server.html).
* Create a new ssh public/private key pair. We will use an algorithm called *ed25519*. The command
  is as follows.

  ```bash
  $ ssh-keygen -t ed25519 -C "your email address"
  ```

    * Replace `your email address` with an actual email address of yours. For example,

      ```bash
      $ssh-keygen -t ed25519 -C "steveyko@sfu.ca"`
      ```

    * You can use the default file location by pressing `<Enter>`.
    * Choose a passphrase and remember it. This will be used when you access your private key.
* The above command creates two files under `.ssh/`.
    * One file is `id_ed25519` and the other is `id_ed25519.pub`.
    * `id_ed25519` contains your private key. Try `cat ~/.ssh/id_ed25519` and check the content. It
      should start with `-----BEGIN OPENSSH PRIVATE KEY-----` which indicates that it is a private
      key.
    * `id_ed25519.pub` contains your public key. Try `cat ~/.ssh/id_ed25519.pub` and check the
      content. It should start with `ssh-ed25519`, indicating the algorithm.
* As discussed in class, you can share the public key with anybody. However, you need to protect the
  private key as your secret.
* SSH key pairs are typically used for SSH authentication. This is a different way of authenticating
  yourself to a server than using a password.
* The way it works is the following. On the server machine (i.e., the machine that you want to SSH
  in to), you can add your public key to the `authorized_keys` file. This file is located in the
  `$HOME/.ssh/` directory in your home directory (on the server machine). The `authorized_keys` file
  contains a list of public keys that are allowed for public/private key pair authentication. On the
  client side, you need to have your private key under the `$HOME/.ssh/` directory (on the client
  machine), which proves to the server that you are the owner of the public key.
* We will use our CSIL Linux machines to understand this further, but it requires some explanation
  first.
* A special thing about our CSIL Linux machines is that the machines all share the same home
  directory. Thus, no matter which CSIL machine you log in to, you will see the same files in your
  home directory.
* What this means is that if you add your public key to the `$HOME/.ssh/authorized_keys` file from
  one machine, this is all shared. Thus, you can log in to all other CSIL Linux machines from your
  client machine as long as you have your private key on your client machine.
* Now, a CSIL Linux machine doesn't have to be just an SSH server. You can use one CSIL Linux
  machine as an SSH client and log in to another CSIL Linux machine as an SSH server. Thus, you can
  have a private key on an CSIL Linux machine and log in to another CSIL Linux machines.
* However, since all CSIL Linux machines share the same home directory, a private key on one CSIL
  Linux machine is shared with all other CSIL Linux machines as well. Thus, as long as you have your
  private key on one CSIL Linux machine, you can log in from any CSIL Linux machine to any other
  CSIL Linux machine. since both the private key and the `authorized_keys` file are shared.
* Since you created a public/private key pair above, you only need to add your public key to the
  `authorized_keys` file on a CSIL Linux machine. You can use the following command, which appends
  your public key to the `authorized_keys` file.

  ```bash
  $ cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys
  ```

  With this, your private key and your `authorized_keys` file are all shared across all CSIL Linux
  machines. You can now use any of the CSIL Linux machines as an SSH client and log in to any other
  CSIL Linux machines as an SSH server.
* To try out SSH authentication, pick another CSIL Linux CPU server and ssh into it. For example,

  ```bash
  $ ssh -p csil-cpu8.csil.sfu.ca
  ```

* If this is your first time logging in to the server, you need to type `yes` to add the server to
  the known host list.
* After that, it will ask you to type your *passphrase* instead of your password. The passphrase is
  the same passphrase that you entered for your private key.
* Typically, one uses this in conjunction with `ssh-agent` and `ssh-add` to skip your passphrase
  typing for easy authentication.
    * [This short article](https://kb.iu.edu/d/aeww) describes the process of using `ssh-agent` and
      `ssh-add`. In a nutshell, you run `ssh-agent` and then `ssh-add` to keep your private key and
      the passphrase in memory. `ssh-agent` will run in the background and use your private key and
      the passphrase to authenticate all SSH logins. When you're done using `ssh` (or `scp`), you
      need to kill `ssh-agent` so that it removes your private key and the passphrase from memory.

## Task 1: Collisions in Cryptographic Hash Functions

For the rest of the assignment, you should start recording. It now involves no sensitive data.

In class, we have looked at the possibility of collisions in cryptographic hash functions. To recap,
there are three properties we discussed.

* One-way function: suppose we have a cryptographic hash function `h()`. Given `h(x)` (i.e., the
  hashed value of `x`), it should be difficult to find `x`. In other words, the reverse of `h()`
  should be difficult to compute.
* Weak collision resistance: given `x` and a hash function `h()`, it should be difficult to find
  `x'` where `h(x') == h(x)`. In other words, given a value and a hash function, it should be
  difficult to find another value that produces the same hash.
* Strong collision resistance: given a hash function `h()`, it should be difficult to find two
  messages `x` and `x'` where `h(x) == h(x)`. In other words, given a hash function, it should be
  difficult to find two values that produce the same hash.

Note that the first is what a cryptographic hash function needs to provide at the minimum. The other
two are extra properties that many cryptographic hash functions provide, but exactly which
properties you get depends on the algorithm.

In class, we have also discussed that there is "window of validity" where a cryptographic hash
function is considered secure (i.e., still collision resistant and/or still difficult to reverse). A
well-known example of a popular cryptographic hash function that is no longer considered secure is
[MD5](https://en.wikipedia.org/wiki/MD5), which was once universal but no longer used in
applications that require a secure cryptographic hash function. There is a good description of this
history on [Wikipedia](https://en.wikipedia.org/wiki/MD5#Collision_vulnerabilities).

To provide some evidence on this, let's look at one popular use case of a cryptographic hash
function, which is downloaded file verification. Many websites that host downloadable files list
their files as well as their hashes. This way, once you download a file, you can generate a hash
using the same algorithm and verify if you get the same hash as listed on the website. If the two
hashes match, then you know that you downloaded the correct file.

It used to be the case that MD5 was the most popular choice, but not anymore. For example, if you
look at Ubuntu 14.04.6, which is one of the older versions of Ubuntu Linux, [its download
page](https://releases.ubuntu.com/trusty/) shows all the downloadable files as well as the hashes
from various algorithms, including MD5, SHA1, and SHA256. Click the link and see for yourself.
However, if you look at a more recent version, e.g., [Ubuntu 20.04.6's download
page](https://releases.ubuntu.com/20.04.6/), it only shows a hash for SHA256. Also click the link
and see for yourself. This is because MD5 is no longer considered secure. (And actually SHA1 is no
longer considered secure either.)

Let's try a simple example as an activity. Don't forget to `record`.

The following example comes from [an article on MD5
collisions](https://natmchugh.blogspot.com/2015/02/create-your-own-md5-collisions.html). It shows
two images that generate the same MD5 hash. First take a look at the images and see for yourself how
different they are: the [first image](https://s3-eu-west-1.amazonaws.com/md5collisions/ship.jpg) and
the [second image](https://s3-eu-west-1.amazonaws.com/md5collisions/plane.jpg).

Then run the following commands to download those images and generate a MD5 hash for each.

```bash
$ wget https://s3-eu-west-1.amazonaws.com/md5collisions/ship.jpg
$ wget https://s3-eu-west-1.amazonaws.com/md5collisions/plane.jpg
$ openssl dgst -md5 ship.jpg
$ openssl dgst -md5 plane.jpg
```

The outputs should be identical. What this means is that theoretically, if you used MD5, downloaded
a file from a website, and verified it, it could still be a file different from the file you
intended to download.

The command we used (`openssl`) is a popular command for cryptographic functions including hashing,
encryption, and decryption. `dgst` means that we want to generate a *digest*, which is another term
commonly used to refer to a hash. Let's use a different algorithm and check the output.

```bash
$ openssl dgst -sha256 ship.jpg
$ openssl dgst -sha256 plane.jpg
```

It should give you two different outputs as we're using a different algorithm, SHA256, not MD5, that
doesn't create a collision for the given files.

## Task 2: Implementing a Simple Blockchain

A blockchain is a popular data structure that is used in many applications, most notably in the
world of cryptocurrencies. A blockchain is a data structure that consists of blocks, where each
block contains data. Each block also contains a hash of the previous block, which creates a chain of
blocks. This is a simple way to ensure that the data in the blocks is not tampered with. If you
change the data in one block, the hash of that block will change, and the hash of the next block
will no longer match the hash in the previous block. Thus, if you want to successfully tamper with
the data in a block, you need to tamper with all the blocks that come after it, which can be a
computationally challenging task.

To make it even more challenging, a typical blockchain requires that newly-generated blocks meet
certain conditions. For example, Bitcoin, a popular cryptocurrency, has a *target hash* and new
blocks must have a hash that is less than or equal to the target. To allow this, each block also
contains a *nonce*, which is a random number that one can vary to create a hash that satisfies the
condition. As you can imagine, it is computationally challenging to find a such nonce and it has a
fitting name, *mining*, analogous to mining for precious metals such as gold. One needs to try a
very large number of different nonces to find the one that creates a hash that satisfies the
condition. In Bitcoin, miners are rewarded with new Bitcoins for successfully mining a new block.

Below, we discuss the details of the requirements you need to implement.

### Block

Each block is a data structure defined in [`include/blockchain.h`](include/blockchain.h). It
contains the following fields.

* `index`: the index of the block in the blockchain.
* `timestamp`: the time when the block was created.
* `data`: the data that the block contains.
* `p_hash`: the hash of the previous block.
* `nonce`: a random number that is used to create a hash that satisfies a certain condition.
* `hash`: the hash of the current block.

Each block should satisfy the following conditions.

* The hash of a block is created by hashing the entire block (i.e., the entire `struct`) using
  SHA256.
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

### Blockchain

A blockchain is a data structure that contains an array of blocks. It is defined in
[`include/blockchain.h`](include/blockchain.h). Along with it, you need to implement the following
functions.

* `bc_init()`: initializes the blockchain. It should initialize the buffer passed as the argument
  and use the hash target also passed as an argument (`difficulty`). If `difficulty` is `NULL`, it
  means that there is no target hash. The function should return `0` for success and `-1` for
  failure.
* `bc_add_block()`: adds a new block to the blockchain. It should create a new block, set the fields
  of the block, and find the correct nonce that satisfies the hash condition. It should then add the
  block to the blockchain. It should return the index of the new block for success and `-1` for
  failure.
* `bc_verify()`: verifies the blockchain. It should check if the hash of each block matches the
  `p_hash` field of the next block. It should also check if the hash of each block satisfies the
  difficulty condition. It should return `0` for success and `-1` for failure.

### Implementation

* SHA256: you need to use the OpenSSL `libcrypto` library to generate a SHA256 hash. OpenSSL's man
  page for
  [ossl-guide-libcrypto-introduction](https://www.openssl.org/docs/man3.2/man7/ossl-guide-libcrypto-introduction.html)
  provides a good introduction to using algorithms in applications. Especially, the section [Using
  Algorithms in
  Applications](https://www.openssl.org/docs/man3.2/man7/ossl-guide-libcrypto-introduction.html#USING-ALGORITHMS-IN-APPLICATIONS)
  provides an example that shows how to generate a SHA256 hash.
* Random number generation: you can use the `rand()` function to generate a pseudo-random number.
  However, `rand()` generates the same sequence of pseudo-random numbers every time you run the
  program. This is because the random number generator is seeded with the same default value (`1`)
  every time. In order to generate a different sequence of random numbers every time you run the
  program, you should seed the random number generator with a different value. To accomplish this,
  you can use the `srand()` function to seed the random number generator with the *current time*.
  You can use the `time()` function to get the current time. (`man 3 rand` and `man 2 time` for more
  information.)

## Code Structure and CMake

* You need to use the same code structure as previous assignments with `src/` and `include/`.
* You also need to write `CMakeLists.txt` that produces one executable, `blockchain`.
* You need to set the `CC` and `CXX` environment variables to `clang` and `clang++`. If you haven't,
  open your ~/.zshrc and add the following two commands at the end.

  ```bash
  export CC=$(which clang)
  export CXX=$(which clang++)
  ```

## Grading

* Task 1: 1% (out of the total 10%)
* Task 2: 9% (out of the total 10%)
    * 3%: Blockchain with no difficulty
        * 1%: `bc_add_block()` successfully adds one block (the first block).
        * 2%: `bc_add_block()` successfully adds multiple blocks.
    * 6%: Blockchain with difficulty
        * 2%: `bc_add_block()` successfully adds one block (the first block).
        * 2%: `bc_add_block()` successfully adds multiple blocks.
        * 2%: `bc_verify()` successfully verifies the blockchain.
* Code that does not compile with CMake gets a 0.
* Code that does not generate all the required executables gets a 0.
* Memory issues have a penalty of -10 pts. Use appropriate sanitizers and enable the debug option.
  Make sure your code works without the debug option as well. Before you submit, make sure that you
  remove the debug option.
* A wrong code directory structure has a penalty of -10 pts.
* Thread/synchronization issues have a penalty of -10 pts.
* You should not hard-code or customize your implementation tailored to our test cases. Generally,
  you should consider the provided test cases as examples or specific instances of general cases.
