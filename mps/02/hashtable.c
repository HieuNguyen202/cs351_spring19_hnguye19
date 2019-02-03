#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

/* Daniel J. Bernstein's "times 33" string hash function, from comp.lang.C;
   See https://groups.google.com/forum/#!topic/comp.lang.c/lSKWXiuNOAk */
unsigned long hash(char *str) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}

/// Creates a hashtable with size buckets.
/// \param size
/// \return
hashtable_t *make_hashtable(unsigned long size) {
  hashtable_t *ht = malloc(sizeof(hashtable_t));
  ht->size = size;
  ht->buckets = calloc(sizeof(bucket_t *), size);
  return ht;
}

/// Inserts the key => val mapping, or updates the value for key, if it is already in the hashtable.
/// \param ht
/// \param key
/// \param val
void ht_put(hashtable_t *ht, char *key, void *val) {
    /* FIXME: the current implementation doesn't update existing entries */
    unsigned int idx = hash(key) % ht->size;
    bucket_t *b = ht->buckets[idx]; //HEAD
    if (b==NULL) {//If HEAD is NULL
        bucket_t *newB = malloc(sizeof(bucket_t));
        newB->key = key;
        newB->val = val;
        newB->next = NULL; //to make it null
        ht->buckets[idx] = newB;
        return;
    } else {//If HEAD is not NULL
        while (b) {
            if (strcmp(b->key, key) == 0) { //if key matches
                b->val = val;                   //update val
                return;
            } else if (b->next == NULL) {//If key doesn't match and b->next is NULL
                bucket_t *newB = malloc(sizeof(bucket_t));
                newB->key = key;
                newB->val = val;
                newB->next = NULL;
                b->next = newB;
                return;
            } else {//If key doesn't match and b->next is not NULL
                b = b->next;                    //next bucket
            }
        }
    }
}

/// Get the tail of the bucket link
/// \param b
/// \return
bucket_t *get_last_bucket(bucket_t *b){
    if(b==NULL)
        return NULL;
    while(b->next!=NULL)
        b = b->next;
    return b;
}


void _ht_put(hashtable_t *ht, bucket_t *nb) {
    //If NULL, return right away
    if(nb==NULL)
        return;
    bucket_t *temp_b;
    unsigned int idx;

    do{
        idx = hash(nb->key) % ht->size;
        if (ht->buckets[idx] == NULL) {//If HEAD is NULL
            ht->buckets[idx] = nb;
        } else {//If HEAD is not NULL
            get_last_bucket(ht->buckets[idx])->next = nb;
        }
        //set the the next b to NULL
        temp_b = nb->next;
        nb->next = NULL;
        //Prep to put the next link
        nb = temp_b;
    } while(nb!=NULL);
}

/// Returns the value for key, or NULL if key doesn't exist.
/// \param ht
/// \param key
/// \return
void *ht_get(hashtable_t *ht, char *key) {
  unsigned int idx = hash(key) % ht->size;
  bucket_t *b = ht->buckets[idx];
  while (b) {
    if (strcmp(b->key, key) == 0) {
      return b->val;
    }
    b = b->next;
  }
  return NULL;
}

/// Calls f with all key/value mappings in the hashtable; iteration can be terminated early if f returns 0.
/// \param ht
/// \param f
void ht_iter(hashtable_t *ht, int (*f)(char *, void *)) {
  bucket_t *b;
  unsigned long i;
  for (i=0; i<ht->size; i++) {
    b = ht->buckets[i];
    while (b) {
      if (!f(b->key, b->val)) {
        return ; // abort iteration
      }
      b = b->next;
    }
  }
}

/// Frees all keys, values, buckets, and the underlying bucket array of the hashtable.
/// \param ht
void free_hashtable(hashtable_t *ht) {
    free(ht->size);
    for (int i = 0; i < ht->size; ++i) {
        freeBucket(ht->buckets[i]);
    }
    free(ht->buckets);
    free(ht); // FIXME: must free all substructures!
}

/* TODO */
/// Removes the mapping for key
/// \param ht
/// \param key
void  ht_del(hashtable_t *ht, char *key) {
    unsigned int idx = hash(key) % ht->size;
    bucket_t *b = ht->buckets[idx];
    if (strcmp(b->key, key) == 0) {
        ht->buckets[idx] = b->next;
        free(b);
        //TODO maybe deallocate here
        return;
    } else {
        bucket_t *prevB;
        prevB = b;
        b = b->next;
        while (b) {
            if (strcmp(b->key, key) == 0) {
                prevB->next = b->next;
                free(b);
                //TODO maybe deallocate here
                return;
            }
            prevB = b;
            b = b->next;
        }
    }
}

/// next have to be NULL before calling this funciton
/// \param b
void freeBucket(bucket_t *b){
    if(b==NULL)
        return;
    free(b->key);
    free(b->val);
    freeBucket(b->next);
    free(b);
}

/// Resizes the hashtable to contain newsize buckets, rehashing all keys and moving them into new buckets as needed.
/// \param ht
/// \param newsize
void  ht_rehash(hashtable_t *ht, unsigned long newsize) {
    //Alloc memory for new buckets
    bucket_t **oldBukets = ht->buckets;
    int oldSize = ht->size;

    //Change size of ht
    ht->size=newsize;
    //Alloc new buckets for the ht
    bucket_t **newBuckets = calloc(sizeof(bucket_t *), newsize);
    ht->buckets = newBuckets;

    //Transfer buckets from the old mem to new mem
    for (int i = 0; i < oldSize; ++i) {
        _ht_put(ht, oldBukets[i]);
    }
    //Free resources used by the old buckets
    free(oldBukets);
}

