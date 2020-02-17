/**
 *  @file tokenset.c
 *  @version 1.4.0-dev0
 *  @date Sun Feb 16, 2020 07:19:21 PM CST
 *  @copyright 2018-2020 John A. Crow <crowja@gmail.com>
 *  @license Unlicense <http://unlicense.org/>
 *  @brief Simple class for mapping tokens to integers and retrieving tokens.
 *  @details Map strings, i.e., tokens, to consecutive integers using
 *  tokenset_add(). Retrieve these tokens integer using tokenset_get_by_id().
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "uthash.h"
#include "tokenset.h"

#ifdef  IS_NULL
#undef  IS_NULL
#endif
#define IS_NULL(p)   ((NULL == (p)) ? (1) : (0))

#ifdef  FREE
#undef  FREE
#endif
#define FREE(p)      ((NULL == (p)) ? (0) : (free((p)), (p) = NULL))

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
   if (IS_NULL(tp))
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

   if (IS_NULL(*pp))
      return;

   t = (*pp)->tokens;

   while (!IS_NULL(t)) {
      s = t;
      t = s->hh.next;
      FREE(s->text);
      HASH_DEL((*pp)->tokens, s);
      FREE(s);
   }

   FREE(*pp);
   *pp = NULL;
}

const char *
tokenset_version(void)
{
   return "1.4.0-dev0";
}

int
tokenset_add(struct tokenset *p, char *n)
{
   struct _token *s;

   HASH_FIND_STR(p->tokens, n, s);

   if (!IS_NULL(s))
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

   return IS_NULL(s) ? 0 : 1;
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

   while (!IS_NULL(t)) {

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

   return IS_NULL(s) ? -1 : (int) s->id;

#if 0
   return IS_NULL(s) ? 0 : 1;
#endif
}

void
tokenset_remove(struct tokenset *p, char *n)
{
   struct _token *s;

   HASH_FIND_STR(p->tokens, n, s);

   if (IS_NULL(s))
      return;

   FREE(s->text);
   HASH_DEL(p->tokens, s);

   FREE(s);
}

void
tokenset_reset(struct tokenset *p)
{
   struct _token *s;
   struct _token *t = p->tokens;

   while (!IS_NULL(t)) {
      s = t;
      t = s->hh.next;
      FREE(s->text);
      HASH_DEL(p->tokens, s);
      FREE(s);
   }

   p->size = 0;
}

void
tokenset_sort(struct tokenset *p)
{
   HASH_SORT(p->tokens, _text_sort);
}

#undef  IS_NULL
#undef  FREE
