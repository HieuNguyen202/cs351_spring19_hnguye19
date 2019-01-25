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
  bucket_t **tempB;
  tempB = &ht->buckets[idx];
  //bpp
  //while(1)
  while(1) {
    if (*tempB == NULL) {                     //If tempB == NULL
      //Create a new bucket b
      bucket_t *b = malloc(sizeof(bucket_t));
      b->key = key;
      b->val = val;
      //*tempB points to bucket b
      *tempB = b;
      return;
    } else {                                  //If tempB != NULL
        if (strcmp((*tempB)->key, key) == 0) {              //  If tempB->key == key
          //Update new val
          (*tempB)->val = val;
          return;
        } else{                                 //  If tempB->key != key
          if((*tempB)->next == NULL) {            //    If tempB->next == NULL
            //Create a new bucket b
            bucket_t *b = malloc(sizeof(bucket_t));
            b->key = key;
            b->val = val;
            //tempB->next points to bucket b
            (*tempB)->next = b;
            return;
          } else {                                //    If tempB->next != NULL
            //next node
            *tempB = &((*tempB)->next);
          }
        }
    }
  }
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
  free(ht); // FIXME: must free all substructures!
}

/* TODO */
/// Removes the mapping for key
/// \param ht
/// \param key
void  ht_del(hashtable_t *ht, char *key) {
}

/// Resizes the hashtable to contain newsize buckets, rehashing all keys and moving them into new buckets as needed.
/// \param ht
/// \param newsize
void  ht_rehash(hashtable_t *ht, unsigned long newsize) {
}
