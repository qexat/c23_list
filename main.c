#include <stdlib.h>
#include "list.h"

/* <main::start> */

int add(int a, int b)
{
    return a + b;
}

int main()
{

    list(int) lst = list_new(int);

    list_push(lst, 0);
    list_push(lst, 1);

    for (size_t i = 2; i < 20; i++)
    {
        list_push(lst, list_get(lst, i - 2) + list_get(lst, i - 1));
    }

    int lastElement = list_pop(lst);

    list_iterate(lst, element, {
        printf("index %lu: %d\n", i, element);
    });

    printf("last element: %d\n", lastElement);

    size_t myIndex = 50;

    if (list_is_in_bounds(lst, myIndex))
        printf("%d\n", list_get(lst, myIndex));
    else
        printf("sorry, index %lu is out of bounds :(\n", myIndex);

    int sumList = list_foldleft(lst, add, 0);

    printf("sum of the list: %d\n", sumList);

    list_delete(lst);

    return EXIT_SUCCESS;
}

/* <main::end> */
