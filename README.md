# Assignment 13: More about Cryptography Function Applications

As discussed in class, there are three popular types of cryptography, cryptographic hashing, secret
key cryptography, and public key cryptography. In this assignment, you will do some activities to
gain a better understanding of those functions.

As you follow this assignment, don't forget to `record` and later submit your `.record/` and
`.nvim/` directories.

# Task 0: Collisions in Cryptographic Hash Functions

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
two images that generate the same MD5 hash. First take a look at the images: the [first
image](https://s3-eu-west-1.amazonaws.com/md5collisions/ship.jpg) and the [second
image](https://s3-eu-west-1.amazonaws.com/md5collisions/plane.jpg).

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
$ openssl dsgt -sha256 ship.jpg
$ openssl dgst -sha256 plane.jpg
```

It should give you two different outputs as we're using a different algorithm, SHA256, not MD5.

For the next part, you ***should not*** record as explained below. Stop recording from here and
submit all the files you created up to this point, including `.record/` and `.nvim/`.

# Task 1: SSH Authentication with Public Cryptography

There are many applications of public cryptography and one popular application is SSH
authentication/login. You might have done this before for other classes, but with our class
discussion, you can now understand what is going on.

Before we proceed further, make sure that you have stopped recording. This part involves your own
sensitive data such as your password and you ***should not*** record anything.

* First, log in to a CSIL machine from our VM. But again, ***do not*** record anything. Note that
  you need to first use the VPN on your host (*not* our VM) to be able to log in.
* You can use one of the [Linux CPU
  servers](https://www.sfu.ca/computing/about/support/csil/unix/how-to-use-csil-linux-cpu-server.html).
* Create a new ssh public/private key pair. We will use an algorithm called *ed25519*. The command
  is as follows.

  ```bash
  $ ssh-keygen -t ed25519 -C "your email address"
  ```

    * Replace `your email address` with an actual email address of yours. For example, `$ssh-keygen
      -t ed25519 -C "steveyko@sfu.ca"`.
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
* SSH key pairs are typically used for SSH authentication. The following command allows you to use
  your public key for SSH authentication.

  ```bash
  $ cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys
  ```

* To try out SSH authentication, pick another CSIL Linux CPU server and ssh into it. For example,

  ```bash
  $ ssh -p csil-cpu8.csil.sfu.ca
  ```

* If this is your first time logging in to the server, you need to type `yes` to add the server to
  the known host list.
* After that, it will ask you to type your *passphrase* instead of your password. The passphrase is
  the same passphrase that you entered for your private key.
* You can use this in conjunction with `ssh-agent` and `ssh-add` to skip your passphrase typing,
  which is what people frequently use for easy authentication.
    * [This short article](https://kb.iu.edu/d/aeww) describes the process of using `ssh-agent` and
      `ssh-add`. In a nutshell, you run `ssh-agent` and then `ssh-add` to keep your private key and
      the passphrase in memory. `ssh-agent` will run in the background and use your private key and
      the passphrase to authenticate all SSH logins. When you're done using `ssh` or `scp`, you need
      to kill `ssh-agent` so that it removes your private key and the passphrase from memory.
