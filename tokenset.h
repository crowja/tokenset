/**
 *  @file tokenset.h
 *  @version 1.2.2-dev0
 *  @date Tue Dec 10 12:49:29 CST 2019
 *  @copyright %COPYRIGHT%
 *  @brief FIXME
 *  @details FIXME
 */

#ifndef _TOKENSET_H_
#define _TOKENSET_H_

/**
 *  @brief Tokenset.
 *  @details A tokenset is a collection of unique strings (tokens).
 *  As a token is added it is assigned an integer identifier.
 *  The motivation is a situation in which text is being lexed
 *  into words and we want to keep track of the unique words,
 *  assigning unsigned integer identifiers to each. A tokenset
 *  can have tokens added to or removed from it.
 */

struct tokenset;

/**
 *  @brief Constructor. Create and return a new tokenset object.
 *  @details Constructor for tokenset objects.
 *  @returns On success a pointer to the new tokenset object, the
 *  NULL pointer otherwise. 
 */

struct tokenset *tokenset_new(void);

/**
 *  @brief Destructor.
 *  @details Clean up a tokenset structure, freeing allocated
 *  memory.
 *  @param[in] p Pointer to a tokenset object
 */

void        tokenset_free(struct tokenset **pp);

/**
 *  @brief Adds a string to the tokenset.
 *  @details Adds the string (token) if it isn't already in the tokenset,
 *  otherwise does nothing.
 *  @param[in] p Pointer to a tokenset object
 *  @param[in] n Pointer to a string to add.
 *  @returns Whether added or not the unsigned integer id associated
 *  with the token is returned.
 */

int         tokenset_add(struct tokenset *p, char *n);

/**
 *  @brief Number of tokens added to the tokenset.
 *  @details This provides the count of the FIXME
 *  @param p Pointer to a tokenset object
 *  @returns Returns the number of tokens in the tokenset.
 */

int         tokenset_count(struct tokenset *p);

/** 
 *  @brief Checks if a token is already in the tokenset.
 *  @details Checks if a string (token) is already in the tokenset.
 *  @param p Pointer to a tokenset object.
 *  @param n String to check.
 *  @returns Nonzero if the string exists, zero otherwise.
 */

int         tokenset_exists(struct tokenset *p, char *n);

/**
 *  @brief Return the list of tokens in a tokenset.
 *  @details Returns a NULL-terminated list of tokens in the tokenset.
 *  @param p Pointer to a tokenset object.
 *  @returns NULL-terminated list of strings.
 */

char      **tokenset_get(struct tokenset *p);

/**
 *  @brief Return the token associated with an id.
 *  @details Each token is associated with a unique id. Return the
 *  token associated with id.
 *  @param p Pointer to a tokenset object.
 *  @param id Identifier.
 *  @returns String associated with id.
 */

const char *tokenset_get_by_id(struct tokenset *p, unsigned id);

/**
 *  @brief Removes a token from the tokenset.
 *  @details If the specified token (string) is found in the tokenset,
 *  remove it.
 *  @param p Pointer to a tokenset object
 *  @param n String to remove, if found.
 */

void        tokenset_remove(struct tokenset *p, char *n);

/**
 *  @brief Returns the id associated with a token.
 *  @details Returns the id associated with a token/string.
 *  @param p Pointer to a tokenset object
 *  @param n String.
 *  @returns id of the token, if found, -1 otherwise.
 */

int         tokenset_id(struct tokenset *p, char *n);


/**
 *  @brief Removes all tokens from a tokenset.
 *  @details Removes all tokens/strings from a tokenset, but
 *  does not remove the tokenset itself as does tokenset_free().
 *  @param p Pointer to a tokenset object
 *  @returns TODO
 */

void        tokenset_reset(struct tokenset *p);

/**
 *  @brief Lexicographically sort the tokenset.
 *  @details Sort the tokenset lexicographically so that,
 *  for example, the list returned by tokenset_get() is
 *  ordered. Does not change the token-id pairing.
 *  @param p Pointer to a tokenset object
 */

void        tokenset_sort(struct tokenset *p);


/**
 *  @brief Return the version of this package
 *  @details TODO
 *  @returns TODO
 */

const char *tokenset_version(void);

#endif
