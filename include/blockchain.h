#pragma once

#include <openssl/evp.h>
#include <stddef.h>
#include <time.h>

#define DATA_SIZE 64
#define MAX_NUM_BLOCKS 1024

struct block {
  size_t index;                  // the index of the block in the blockchain
  struct timespec timestamp;     // the time when the block was created
  unsigned char data[DATA_SIZE]; // the data that the block contains
  unsigned char p_hash[EVP_MAX_MD_SIZE]; // the hash of the previous block.
  uint32_t nonce; // a random number to satisfy the difficulty
  unsigned char hash[EVP_MAX_MD_SIZE]; // the hash of the current block.
};

struct blockchain {
  struct block[MAX_NUM_BLOCKS];
  size_t count;
  unsigned char difficulty[EVP_MAX_MD_SIZE]; // the hash of the current block.
};

int bc_init(struct blockchain *bc, unsigned char difficulty[EVP_MAX_MD_SIZE]);
int bc_add_block(struct blockchain *bc, const unsigned char data[DATA_SIZE]);
int bc_verify(struct blockchain *bc);
