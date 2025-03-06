# Simple Blockchain

This project implements a simple blockchain system in C, demonstrating the core concepts behind blockchain technology such as block creation, cryptographic hashing, and proof-of-work through nonce mining.

## Features

- Block Structure
  - Unique index for each block
  - Timestamp of creation
  - Arbitrary data payload
  - Previous block hash (p_hash)
  - Nonce for proof-of-work
  - SHA-256 hash of the block core
- Proof-of-Work Mining
  - Dynamically searches for a valid nonce that produces a block hash lower than or equal to a given difficulty target.
  - Supports blockchains with or without difficulty constraints.
- Blockchain Verification
  - Ensures the validity of the entire chain by checking:
  - Correct linkage between block hashes.
  - Compliance with the difficulty target.
  - Integrity of stored block data and hashes.

## How it works

1. **Initialize the blockhain**: The blockchain is initialized with an optional difficulty target. If no difficulty is set, any hash is accepted.
2. **Add block**: New blocks are created by:

- Setting the index, timestamp, and data.
- Referencing the previous block’s hash.
- Mining a nonce that satisfies the difficulty.
- Storing the final block in the blockchain.

3. **Verify the blockchain**: The verification process checks the entire chain for integrity and proof-of-work validity.

## Technologies used

- **C (Clang)** for core logic.
- **SHA-256 (OpenSSL's libcrypto)** for cryptographic hashing.
- **CMake** for build management.
- **Clock APIs** for precise block timestamps.

## Status

This project successfully demonstrates:

- Basic blockchain construction.
- Nonce mining with proof-of-work.
- Full chain verification.

## License

This project is for educational purposes and does not include any actual cryptocurrency logic or real-world blockchain capabilities.
