#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Structure type that encapsulates our safe int array. */
typedef struct {
  int* data;
  unsigned int len;
} intarr_t;

/* A type for returning status codes */
typedef enum {
  INTARR_OK,
  INTARR_BADARRAY,
  INTARR_BADINDEX,
  INTARR_BADALLOC,
  INTARR_NOTFOUND
} intarr_result_t;

/* LAB 5 TASK 1 */

// Create a new intarr_t with initial size len.  If successful
// (i.e. memory allocation succeeds), returns a pointer to a
// newly-allocated intarr_t.  If unsuccessful, returns a null pointer.
intarr_t* intarr_create( unsigned int len )
{
	intarr_t *array = malloc(sizeof (intarr_t));
	if (array == NULL)
	{
		return NULL;
	}
	array->len = len;
	array->data = malloc((array->len)*sizeof(int));
	if (array->data == NULL)
	{
		return NULL;
	}
	return array;
}
// frees all memory allocated for ia. If the pointer is null, do
// nothing. If the ia->data is null, do not attempt to free it.
void intarr_destroy( intarr_t* ia )
{
	if (ia == NULL || ia->data == NULL)
	{
		return;
	}
	free(ia->data);
	free(ia);
}
/* LAB 5 TASK 2 */

// If index is valid, set the value at ia->data[index] to val and return
// INTARR_OK. Otherwise, leave the array unmodified and return
// INTARR_BADINDEX. If ia is null, return INTARR_BADARRAY.
intarr_result_t intarr_set( intarr_t* ia,
                            unsigned int index,
                            int val )
{
	if (ia == NULL)
	{
		return INTARR_BADARRAY;
	}
	if (index < ia->len)
	{
		ia->data[index] = val;
		return INTARR_OK;
	}
	return INTARR_BADINDEX;
}

// If index is valid and i is non-null, set *i to ia->data[index] and return
// INTARR_OK. Otherwise no not modify *i and return
// INTARR_BADINDEX. If ia is null, return INTARR_BADARRAY.
intarr_result_t intarr_get( const intarr_t* ia,
                            unsigned int index,
                            int* i )
{
	if (ia == NULL)
	{
		return INTARR_BADARRAY;
	}
	if (index < ia->len && i != NULL)
	{
		*i = ia->data[index];
		return INTARR_OK;
	}
	return INTARR_BADINDEX;
}
/* LAB 5 TASK 3 */

// Return a duplicate of ia, allocating new storage for the duplicate
// data (we call this a "deep copy"). If unsuccessful (i.e. memory
// allocation for the copy fails, or ia is null), return a null pointer.
intarr_t* intarr_copy( const intarr_t* ia )
{
	if (ia == NULL)
	{
		return NULL;
	}
	intarr_t *duplicate = malloc(sizeof (intarr_t));
	if (duplicate == NULL)
	{
		return NULL;
	}
	unsigned int temp = ia->len * sizeof(int);
	duplicate->data = malloc(temp);
	duplicate->len = ia->len;
	if (duplicate->data == NULL)
	{
		return NULL;
	}
	memcpy(duplicate->data, ia->data, temp);

	return duplicate;
}
/* LAB 5 TASK 4 */

// sort ia by value smallest-to-largest, so that data[i] < data[i+1]
// for all valid i, then return INTARR_OK. Works in-place: contents of
// ia are sorted on return. If ia is null, return
// INTARR_BADARRAY.
intarr_result_t intarr_sort( intarr_t* ia )
{
	int temp, pos;
	if (ia == NULL)
	{
		return INTARR_BADARRAY;
	}
	for (int i = 1; i < (ia->len); i++)
	{
		temp = ia->data[i];
		pos = i;
		while (pos > 0 && ia->data[pos-1] > temp)
		{
			ia->data[pos] = ia->data[pos-1];
			pos--;
		}
		ia->data[pos] = temp;
	}
	return INTARR_OK;
}

/* LAB 5 TASK 5 */

// Find the first occurence of the target in the array, searching from
// index 0. If the target is found and i is non-null, set *i to the
// location index and return INTARR_OK. If target does not occur in
// the array, leave *i unmodified and return INTARR_NOTFOUND. If ia is
// null, return INTARR_BADARRAY.
intarr_result_t intarr_find( intarr_t* ia, int target, int* i )
{
	if (ia == NULL)
	{
		return INTARR_BADARRAY;
	}
	for (int j = 0; j < ia->len; j++)
	{
		if (ia->data[j] == target)
		{
			if (i != NULL)
			{
				*i = j;
				return INTARR_OK;
			}
		}
	}
	return INTARR_NOTFOUND;
}

/* LAB 5 TASK 6 */

// Append val to the end of ia (allocating space for it). If
// successful, return INTARR_OK, otherwise return
// INTARR_BADALLOC. If ia is null, return INTARR_BADARRAY.
// Take a look at TASK 7 below and see if it might be helpful
intarr_result_t intarr_push( intarr_t* ia, int val )
{
	if (ia == NULL)
	{
		return INTARR_BADARRAY;
	}

	int* tempData = ia->data;
	ia->len++;

	if ((ia->data = realloc(ia->data, ia->len * sizeof(int))) == NULL) {
		ia->data = tempData;
		ia->len--;
  		return INTARR_BADALLOC;
	}

	ia->data[ia->len-1] = val;

	return INTARR_OK;
}

// If the array is not empty, remove the value with the highest index
// from the array, and, if i is non-null, set *i to the removed value,
// then return INTARR_OK. If the array is empty, leave *i unmodified
// and return INTARR_BADINDEX. If ia is null, return INTARR_BADARRAY.
// Take a look at TASK 7 below and see if it might be helpful
intarr_result_t intarr_pop( intarr_t* ia, int* i )
{
	if (ia == NULL)
	{
		return INTARR_BADARRAY;
	}

	if (ia->data == NULL)
	{
		return INTARR_BADINDEX;
	}

	if (i != NULL)
	{
		*i = ia->data[ia->len-1];
		int *tempData = ia->data;
		ia->len--;

		if (ia->len == 0)
		{
			ia->data = NULL;
			return INTARR_OK;
		}

		if ((ia->data = realloc(ia->data, ia->len*sizeof(int))) == NULL)
		{
			ia->data = tempData;
			ia->len++;
			return INTARR_BADALLOC;
		}
	}

	return INTARR_OK;
}

/* LAB 5 TASK 7 */

// Resize ia to contain newlen values. If newlen is less than the
// original array length, the end of the array is discarded. If newlen
// is greater than the original array length, intialize all the new
// integers to zero. If the allocation is successful, return
// INTARR_OK, otherwise return INTARR_BADALLOC. If ia is null, return
// INTARR_BADARRAY.
intarr_result_t intarr_resize( intarr_t* ia, unsigned int newlen )
{
	if (ia == NULL)
	{
		return INTARR_BADARRAY;
	}

	int tempLen = ia->len;
	if (newlen < ia->len)
	{
		int *tempData = ia->data;
		ia->len = newlen;
		if ((ia->data = realloc(ia->data, ia->len*sizeof(int))) == NULL)
		{
			ia->data = tempData;
			ia->len = tempLen;
			return INTARR_BADALLOC;
		}
	}

	if (newlen > ia->len)
	{
		int *tempData = ia->data;
		ia->len = newlen;
		if ((ia->data = realloc(ia->data, ia->len*sizeof(int))) == NULL)
		{
			ia->data = tempData;
			ia->len = tempLen;
			return INTARR_BADALLOC;
		}
		for (int i = tempLen-1; i < ia->len; i++)
		{
			ia->data[i] = 0;
		}
	}

	return INTARR_OK;
}
/* LAB 5 TASK 8 */

// Get a deep copy of a portion of ia from index first to index last
// inclusive. If successful, return a pointer to a newly-allocated
// intarr_t containing a copy of the specfied section. If an error
// occurs, i.e. ia is null, first or last are out of bounds, last <
// first, or memory allocation fails, return a null pointer.
intarr_t* intarr_copy_subarray( intarr_t* ia,
                                unsigned int first,
                                unsigned int last )
{
	if (ia == NULL || last > ia->len-1 || last < first)
	{
		return NULL;
	}

	intarr_t * copy = malloc(sizeof(intarr_t));

	if (copy == NULL)
	{
		return NULL;
	}

	int subarraylen = last - first + 1;
	copy->len = subarraylen;

	unsigned int total = subarraylen * sizeof(int);
	copy->data = (int*)malloc(total);

	if (copy->data == NULL)
	{
		return NULL;
	}

	int i = 0;
	for (int j = first; j <= last; j++)
	{
		copy->data[i] = ia->data[j];
		i++;
	}

	return copy;
}
/* LAB 5 ENDS HERE */

/* -------------------------------------------------------------- */

/* LAB 6 TASK 1 */

/*
  Save the entire array ia into a file called 'filename' in a binary
  file format that can be loaded by intarr_load_binary(). Returns
  zero on success, or a non-zero error code on failure. Arrays of
  length 0 should produce an output file containing an empty array.
*/
int intarr_save_binary( intarr_t* ia, const char* filename );

/*
  Load a new array from the file called 'filename', that was
  previously saved using intarr_save_binary(). Returns a pointer to a
  newly-allocated intarr_t on success, or NULL on failure.
*/
intarr_t* intarr_load_binary( const char* filename );


/* LAB 6 TASK 2 */

/*
  Save the entire array ia into a file called 'filename' in a JSON
  text file array file format that can be loaded by
  intarr_load_json(). Returns zero on success, or a non-zero error
  code on failure. Arrays of length 0 should produce an output file
  containing an empty array.

  The JSON output should be human-readable.

  Examples:

  The following line is a valid JSON array:
  [ 100, 200, 300 ]

  The following lines are a valid JSON array:
  [
   100,
   200,
   300
  ]
*/
int intarr_save_json( intarr_t* ia, const char* filename );

/*
  Load a new array from the file called 'filename', that was
  previously saved using intarr_save_json(). The file may contain an array
  of length 0. Returns a pointer to a newly-allocated intarr_t on
  success (even if that array has length 0), or NULL on failure.
*/
intarr_t* intarr_load_json( const char* filename );


