/*
    MIT License

    Copyright (c) 2024 Qexat

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#pragma GCC diagnostic ignored "-Wpedantic"

#ifndef list_h
#define list_h

#include <stdio.h>
#include <stdlib.h>

#define LIST_BASE_CAPACITY 8
/* It has been shown in the science literature that 2 is a bad choice, so here we go */
#define LIST_CAPACITY_RATIO 1.5

#define list_error(message, ...)                                                                                       \
  do                                                                                                                   \
  {                                                                                                                    \
    fprintf(stderr, "\x1b[1;31mRuntime error:\x1b[22;39m ");                                                           \
    fprintf(stderr, "line %u: ", __LINE__);                                                                            \
    fprintf(stderr, (message), ##__VA_ARGS__);                                                                         \
    fprintf(stderr, "\n");                                                                                             \
    exit(1);                                                                                                           \
  } while (0)

#define list(T)                                                                                                        \
  struct                                                                                                               \
  {                                                                                                                    \
    int _freed;                                                                                                        \
    T *ptr;                                                                                                            \
    size_t length;                                                                                                     \
    size_t capacity;                                                                                                   \
    size_t item_size;                                                                                                  \
  }

#define list_is_valid(lst) ((lst).ptr != NULL && (lst)._freed == 0)
#define list_is_empty(lst) ((lst).length == 0)
#define list_is_in_bounds(lst, index) ((index) < (lst).length)

#define _list_validity_check(lst)                                                                                      \
  ({                                                                                                                   \
    if (!list_is_valid(lst))                                                                                           \
      list_error("invalid list");                                                                                      \
  })

#define list_new(T) {0, malloc(sizeof(T) * LIST_BASE_CAPACITY), 0, LIST_BASE_CAPACITY, sizeof(T)}

#define list_delete(lst)                                                                                               \
  do                                                                                                                   \
  {                                                                                                                    \
    _list_validity_check(lst);                                                                                         \
    free((lst).ptr);                                                                                                   \
    (lst)._freed = 1;                                                                                                  \
    (lst).length = (lst).capacity = (lst).item_size = 0;                                                               \
  } while (0)

#define list_grow(lst)                                                                                                 \
  do                                                                                                                   \
  {                                                                                                                    \
    _list_validity_check(lst);                                                                                         \
    size_t new_capacity = (size_t)((lst).capacity * LIST_CAPACITY_RATIO);                                              \
    typeof((lst).ptr) new_ptr = realloc((lst).ptr, new_capacity * (lst).item_size);                                    \
    if (new_ptr == NULL)                                                                                               \
      list_error("failed to grow list");                                                                               \
    (lst).ptr = new_ptr;                                                                                               \
    (lst).capacity = new_capacity;                                                                                     \
  } while (0)

#define list_push(lst, item)                                                                                           \
  do                                                                                                                   \
  {                                                                                                                    \
    _list_validity_check(lst);                                                                                         \
    while ((lst).length >= (lst).capacity)                                                                             \
      list_grow(lst);                                                                                                  \
    (lst).ptr[(lst).length] = (item);                                                                                  \
    (lst).length++;                                                                                                    \
  } while (0)

#define list_pop(lst)                                                                                                  \
  ({                                                                                                                   \
    _list_validity_check(lst);                                                                                         \
    if (list_is_empty(lst))                                                                                            \
      list_error("no element to remove");                                                                              \
    (lst).length--;                                                                                                    \
    (lst).ptr[(lst).length];                                                                                           \
  })

#define list_get(lst, index)                                                                                           \
  ({                                                                                                                   \
    _list_validity_check(lst);                                                                                         \
    if (!list_is_in_bounds((lst), (index)))                                                                            \
    {                                                                                                                  \
      list_error("index %lu is out of bounds", (size_t)(index));                                                       \
      exit(1);                                                                                                         \
    };                                                                                                                 \
    (lst).ptr[(index)];                                                                                                \
  })

#define list_iterate(lst, identifier, block)                                                                           \
  do                                                                                                                   \
  {                                                                                                                    \
    _list_validity_check(lst);                                                                                         \
    for (size_t i = 0; i < (lst).length; i++)                                                                          \
    {                                                                                                                  \
      typeof(*(lst).ptr) identifier = list_get(lst, i);                                                                \
      block;                                                                                                           \
    }                                                                                                                  \
  } while (0)

#define list_foldleft(lst, function, initial)                                                                          \
  ({                                                                                                                   \
    _list_validity_check(lst);                                                                                         \
    typeof(initial) result = (initial);                                                                                \
    list_iterate((lst), element, { result = (function)(result, element); });                                           \
    result;                                                                                                            \
  })

int _add(int a, int b)
{
  return a + b;
}

#define list_sum(lst) (list_foldleft((lst), _add, 0))
#define list_mean(lst) (list_sum(lst) / (double)(lst).length)

#endif
