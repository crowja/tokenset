#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tokenset.h"
#include "t/tinytest.h"

#ifdef  COLOR_CODE
#undef  COLOR_CODE
#endif
#define COLOR_CODE       0x1B

#ifdef  COLOR_RED
#undef  COLOR_RED
#endif
#define COLOR_RED        "[1;31m"

#ifdef  COLOR_GREEN
#undef  COLOR_GREEN
#endif
#define COLOR_GREEN      "[1;32m"

#ifdef  COLOR_YELLOW
#undef  COLOR_YELLOW
#endif
#define COLOR_YELLOW     "[1;33m"

#ifdef  COLOR_RESET
#undef  COLOR_RESET
#endif
#define COLOR_RESET      "[0m"


static void
printf_test_name(char *name, char *info)
{
   printf("%c%s%s%c%s", COLOR_CODE, COLOR_YELLOW, name, COLOR_CODE, COLOR_RESET);

   if (NULL != info)
      printf(" [%s]\n", info);
   else
      printf("\n");
}


static void
test_constr(void)
{
   struct tokenset *z;

   printf_test_name("test_constr", NULL);

   z = tokenset_new();
   ASSERT("Constructor test", z);
   tokenset_free(&z);
   ASSERT_EQUALS(NULL, z);
}


static void
test_stress_1(void)
{
   struct tokenset *p = tokenset_new();
   char      **n;
   char       *names[] =
    { "joe", "bob", "betty", "bob", "sam", "freddy", "frank", "bob", "johnny", NULL };
   char        buff[100];
   int         iter;
   int         count1 = 0;
   int         count2 = 0;
   int         count3 = 0;
   char        version[16];
   char      **list;
   char      **lp;

   printf_test_name("test_stress_1", "tokenset_new, tokenset_add, tokenset_exists");

   for (iter = 0; iter < 500; iter++) {
      sprintf(version, "_%d", iter);
      for (n = names; *n != NULL; n++) {
         strcpy(buff, *n);
         strcat(buff, version);
         if (!tokenset_exists(p, buff))
            count1 += 1;
         tokenset_add(p, buff);
      }
   }

   if (tokenset_exists(p, "bob"))
      count1 -= 1;
   tokenset_remove(p, "bob");

   tokenset_sort(p);

   list = (char **) tokenset_get(p);

   for (lp = list; *lp != NULL; lp++) {
      count2 += 1;
      /* printf ("\"%s\"\n", *lp); */
      free(*lp);
   }

   free(list);

   count3 = tokenset_count(p);

   tokenset_reset(p);

   ASSERT_EQUALS(count1, count2);
   ASSERT_EQUALS(count1, count3);
   ASSERT_EQUALS(0, tokenset_count(p));

   tokenset_free(&p);
   ASSERT_EQUALS(NULL, p);
}

static void
test_stress_2(void)
{
   struct tokenset *p = tokenset_new();
   char        buff[100];
   int         iter;
   int         count1 = 0;
   int         count2 = 0;
   int         count3 = 0;
   char        x[10];
   char      **list;
   char      **lp;

   printf_test_name("test_stress_2",
                    "tokenset_count, tokenset_get, tokenset_remove, tokenset_reset");

   for (iter = 0; iter < 500; iter++) {
      sprintf(x, "_%d", (int) rand() % 99);
      strcpy(buff, x);
      if (!tokenset_exists(p, buff))
         count1 += 1;
      tokenset_add(p, buff);
   }

   if (tokenset_exists(p, "bob"))
      count1 -= 1;
   tokenset_remove(p, "bob");

   tokenset_sort(p);

   list = (char **) tokenset_get(p);

   for (lp = list; *lp != NULL; lp++) {
      count2 += 1;
      /* printf ("\"%s\"\n", *lp); */
      free(*lp);
   }

   free(list);

   count3 = tokenset_count(p);

   tokenset_reset(p);

   ASSERT_EQUALS(count1, count2);
   ASSERT_EQUALS(count2, count3);
   ASSERT_EQUALS(0, tokenset_count(p));

   tokenset_free(&p);
   ASSERT_EQUALS(NULL, p);
}


static void
test_stress_3(void)
{
   struct tokenset *p = tokenset_new();
   char      **n;
   char       *names[] =
    { "joe", "bob", "betty", "bob", "sam", "freddy", "frank", "bob", "johnny", "phil",
      NULL
   };
   char        buff[100];
   int         iter;
   char        version[10];

   printf_test_name("test_stress_3", "tokenset_get_by_id");

   for (iter = 0; iter < 10; iter++) {
      sprintf(version, "_%d", iter);
      for (n = names; *n != NULL; n++) {
         strcpy(buff, *n);
         strcat(buff, version);
         tokenset_add(p, buff);
      }
   }

   ASSERT_STRING_EQUALS("betty_1", tokenset_get_by_id(p, 10));
   ASSERT_STRING_EQUALS("freddy_2", tokenset_get_by_id(p, 20));
   ASSERT_STRING_EQUALS("johnny_3", tokenset_get_by_id(p, 30));
   ASSERT_STRING_EQUALS("bob_9", tokenset_get_by_id(p, 73));

   tokenset_free(&p);
   ASSERT_EQUALS(NULL, p);
}


static void
test_add(void)
{
   struct tokenset *p = tokenset_new();

   printf_test_name("test_add", "tokenset_add, tokenset_count, tokenset_exists");

   tokenset_add(p, "stephan");
   tokenset_add(p, "stephan");
   tokenset_add(p, "richard");
   tokenset_add(p, "richard");
   tokenset_add(p, "robert");
   tokenset_add(p, "robert");

   ASSERT_EQUALS(3, tokenset_count(p));
   ASSERT_EQUALS(1, tokenset_exists(p, "stephan"));
   ASSERT_EQUALS(1, tokenset_exists(p, "richard"));
   ASSERT_EQUALS(1, tokenset_exists(p, "robert"));
   ASSERT_EQUALS(0, tokenset_exists(p, "reginald"));

   tokenset_free(&p);
   ASSERT_EQUALS(NULL, p);
}


static void
test_remove_1(void)
{
   struct tokenset *p = tokenset_new();

   printf_test_name("test_remove_1",
                    "tokenset_add, tokenset_remove, tokenset_count, tokenset_exists");

   tokenset_add(p, "stephan");
   tokenset_add(p, "richard");
   tokenset_add(p, "robert");
   tokenset_remove(p, "stephan");
   tokenset_remove(p, "richard");
   tokenset_remove(p, "robert");

   ASSERT_EQUALS(0, tokenset_count(p));
   ASSERT_EQUALS(0, tokenset_exists(p, "stephan"));
   ASSERT_EQUALS(0, tokenset_exists(p, "richard"));
   ASSERT_EQUALS(0, tokenset_exists(p, "robert"));
   ASSERT_EQUALS(0, tokenset_exists(p, "reginald"));

   tokenset_free(&p);
   ASSERT_EQUALS(NULL, p);
}


static void
test_remove_2(void)
{
   struct tokenset *p = tokenset_new();

   printf_test_name("test_remove_2", "tokenset_remove, tokenset_count, tokenset_exists");

   tokenset_remove(p, "reginald");

   ASSERT_EQUALS(0, tokenset_count(p));
   ASSERT_EQUALS(0, tokenset_exists(p, "stephan"));
   ASSERT_EQUALS(0, tokenset_exists(p, "richard"));
   ASSERT_EQUALS(0, tokenset_exists(p, "robert"));
   ASSERT_EQUALS(0, tokenset_exists(p, "reginald"));

   tokenset_free(&p);
   ASSERT_EQUALS(NULL, p);
}


static void
test_add_remove_add(void)
{
   struct tokenset *p = tokenset_new();
   int         id1, id2, id3, id4;

   printf_test_name("test_add_remove_add",
                    "tokenset_add, tokenset_remove, tokenset_id, tokenset_exists");

   id1 = tokenset_add(p, "jim");
   id2 = tokenset_add(p, "james");
   tokenset_remove(p, "james");
   id3 = tokenset_add(p, "jimmy");
   id4 = tokenset_add(p, "jaime");

   ASSERT_EQUALS(id1, 0);
   ASSERT_EQUALS(id2, 1);
   ASSERT_EQUALS(id3, 2);
   ASSERT_EQUALS(id4, 3);

   ASSERT_EQUALS(0, tokenset_id(p, "jim"));
   ASSERT_EQUALS(-1, tokenset_id(p, "james"));
   ASSERT_EQUALS(2, tokenset_id(p, "jimmy"));
   ASSERT_EQUALS(3, tokenset_id(p, "jaime"));

   ASSERT_EQUALS(0, tokenset_exists(p, "james"));

   tokenset_free(&p);
   ASSERT_EQUALS(NULL, p);
}


static void
test_get(void)
{
   struct tokenset *p;
   char      **cpp;
   char      **list1, **list2;

   printf_test_name("test_get", "tokenset_add, tokenset_get, tokenset_id, tokenset_sort");

   p = tokenset_new();

   tokenset_add(p, "dog");
   tokenset_add(p, "cat");
   tokenset_add(p, "mouse");
   tokenset_add(p, "moose");
   tokenset_add(p, "squirrel");
   tokenset_add(p, "rooster");
   tokenset_add(p, "hen");
   tokenset_add(p, "chicken");

   list1 = tokenset_get(p);
#if 0
   printf("---\n");
   cpp = list1;
   while (NULL != *cpp) {
      printf("%s\t%d\n", *cpp, tokenset_id(p, *cpp));
      cpp++;
   }
#endif

   tokenset_sort(p);

   list2 = tokenset_get(p);

   /* Original list ordered as added */
   ASSERT_STRING_EQUALS("dog", list1[0]);
   ASSERT_STRING_EQUALS("cat", list1[1]);
   ASSERT_STRING_EQUALS("mouse", list1[2]);

   /* Sorted tokenset ought to be in alphabetical order */
   ASSERT_STRING_EQUALS("cat", list2[0]);
   ASSERT_STRING_EQUALS("chicken", list2[1]);
   ASSERT_STRING_EQUALS("dog", list2[2]);

#if 0
   printf("---\n");
   cpp = list2;
   while (NULL != *cpp) {
      printf("%s\t%d\n", *cpp, tokenset_id(p, *cpp));
      cpp++;
   }
#endif

   cpp = list1;
   while (NULL != *cpp) {
      free(*cpp);
      cpp++;
   }

   free(list1);

   cpp = list2;
   while (NULL != *cpp) {
      free(*cpp);
      cpp++;
   }

   free(list2);
#if 0
#endif

   tokenset_free(&p);
   ASSERT_EQUALS(NULL, p);
}

static void
test_reset(void)
{
   struct tokenset *z = tokenset_new();

   printf_test_name("test_reset", "tokenset_reset");

   tokenset_reset(z);

   ASSERT_EQUALS(tokenset_count(z), 0);

   tokenset_free(&z);
   ASSERT_EQUALS(NULL, z);
}

#if 0
/* 12 yy */
static void
test_stub(void)
{
   struct tokenset *z = tokenset_new();

   printf_test_name("test_stub", NULL);

   ASSERT("Constructor test", z);

   tokenset_free(&z);
   ASSERT_EQUALS(NULL, p);
}
#endif

int
main(void)
{
   printf("%s\n", tokenset_version());
   RUN(test_constr);
   RUN(test_stress_1);
   RUN(test_stress_2);
   RUN(test_stress_3);
   RUN(test_add);
   RUN(test_remove_1);
   RUN(test_remove_2);
   RUN(test_add_remove_add);
   RUN(test_get);
   RUN(test_reset);

   return TEST_REPORT();
}
