/**
 *  @file tokenset.c
 *  @version 1.3.0-dev0
 *  @date Tue Dec 10 12:49:29 CST 2019
 *  @copyright %COPYRIGHT%
 *  @brief Simple class for mapping tokens to integers and retrieving tokens.
 *  @details Map strings, i.e., tokens, to consecutive integers using
 *  tokenset_add(). Retrieve these tokens integer using tokenset_get_by_id().
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "uthash.h"
#include "tokenset.h"

#ifdef  _IS_NULL
#undef  _IS_NULL
#endif
#define _IS_NULL(p)   ((NULL == (p)) ? (1) : (0))

#ifdef  _FREE
#undef  _FREE
#endif
#define _FREE(p)      ((NULL == (p)) ? (0) : (free((p)), (p) = NULL))

static const char version[] = "1.3.0-dev0";

struct _token {
   char       *text;
   unsigned    id;
   UT_hash_handle hh;
};

struct tokenset {
   size_t      size;
   struct _token *tokens;
};

static int
_text_sort(struct _token *a, struct _token *b)
{
   return strcmp(a->text, b->text);
}

struct tokenset *
tokenset_new(void)
{
   struct tokenset *tp;

   tp = (struct tokenset *) malloc(sizeof(struct tokenset));
   if (_IS_NULL(tp))
      return NULL;

   tp->size = 0;
   tp->tokens = NULL;                            /* required by uthash */

   return tp;
}

void
tokenset_free(struct tokenset **pp)
{
   struct _token *s;
   struct _token *t;

   if (_IS_NULL(*pp))
      return;

   t = (*pp)->tokens;

   while (t != NULL) {
      s = t;
      t = s->hh.next;
      _FREE(s->text);
      HASH_DEL((*pp)->tokens, s);
      _FREE(s);
   }

   _FREE(*pp);
   *pp = NULL;
}

const char *
tokenset_version(void)
{
   return version;
}

int
tokenset_add(struct tokenset *p, char *n)
{
   struct _token *s;

   HASH_FIND_STR(p->tokens, n, s);

   if (!_IS_NULL(s))
      return s->id;

   s = (struct _token *) malloc(sizeof(struct _token));
   s->text = malloc((1 + strlen(n)) * sizeof(char));
   strcpy(s->text, n);

   s->id = p->size;

#if 0
   HASH_ADD_STR(p->tokens, text, s);
#else
   HASH_ADD_KEYPTR(hh, p->tokens, s->text, strlen(s->text), s);
#endif

   p->size += 1;                                 /* ready to map next entry */

   return s->id;
}

int
tokenset_count(struct tokenset *p)
{
   return HASH_COUNT(p->tokens);
}

int
tokenset_exists(struct tokenset *p, char *n)
{
   struct _token *s;

   HASH_FIND_STR(p->tokens, n, s);

   return _IS_NULL(s) ? 0 : 1;
}

char      **
tokenset_get(struct tokenset *p)
{
   struct _token *s = p->tokens;
   int         last = tokenset_count(p);
   char      **list = (char **) calloc(1 + last, sizeof(char *));
   int         i;

   for (i = 0; i < last; i++) {
      list[i] = (char *) calloc(1 + strlen(s->text), sizeof(char));
      strcpy(list[i], s->text);
      /* printf("REPORT: %s\n", list[i]); */
      s = s->hh.next;
   }

   list[last] = NULL;

   return list;
}

const char *
tokenset_get_by_id(struct tokenset *p, unsigned id)
{

   struct _token *s;
   struct _token *t = p->tokens;

   while (t != NULL) {

      s = t;
      t = s->hh.next;

      if (s->id == id)
         return (const char *) s->text;
   }

   return NULL;
}

int
tokenset_id(struct tokenset *p, char *n)
{
   struct _token *s;

   HASH_FIND_STR(p->tokens, n, s);

   return _IS_NULL(s) ? -1 : (int) s->id;

#if 0
   return _IS_NULL(s) ? 0 : 1;
#endif
}

void
tokenset_remove(struct tokenset *p, char *n)
{
   struct _token *s;

   HASH_FIND_STR(p->tokens, n, s);

   if (_IS_NULL(s))
      return;

   _FREE(s->text);
   HASH_DEL(p->tokens, s);

   _FREE(s);
}

void
tokenset_reset(struct tokenset *p)
{
   struct _token *s;
   struct _token *t = p->tokens;

   while (t != NULL) {
      s = t;
      t = s->hh.next;
      _FREE(s->text);
      HASH_DEL(p->tokens, s);
      _FREE(s);
   }

   p->size = 0;
}

void
tokenset_sort(struct tokenset *p)
{
   HASH_SORT(p->tokens, _text_sort);
}

#undef  _IS_NULL
#undef  _FREE
