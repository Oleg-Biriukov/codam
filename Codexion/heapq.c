#include "codexion.h"

static void heapify_up(t_heapq *h, int index, t_schdl _prioriy)
{
    void *root;

    root = h->items[(index-1)/2];
    if (index && _prioriy(h->items[index], root))
        return (swap(&h->items[(index-1)/2], &h->items[index]), heapify_up(h, (index - 1) / 2, _prioriy));
}

void	enq_heapq(t_heapq *heap, void *data, t_schdl _prioriy)
{
    if (heap->size == MAX)
        return ;
    heap->items[heap->size++] = data;
    if (heap->size - 1 < 1)
        return ;
    heapify_up(heap, heap->size-1, _prioriy);
}

static void heapify_down(t_heapq *heap, int index, t_schdl _prioriy)
{
    int smallest;
    int left;
    int right;

    smallest = index;
    left = 2 * index + 1;
    right = 2 * index + 2;
    if (left < heap->size
        && _prioriy(heap->items[left], heap->items[smallest]))
        smallest = left;

    if (right < heap->size
        && _prioriy(heap->items[right], heap->items[smallest]))
        smallest = right;
    if (smallest != index)
    {
        swap(&heap->items[smallest], &heap->items[index]);
        heapify_down(heap, smallest, _prioriy);
    }
}

void	*deq_heapq(t_heapq *heap, t_schdl _prioriy)
{
    void    *data;

    if (!heap->size)
        return (NULL);
    if (heap->size == 1)
        return (heap->items[--heap->size]);
    data = heap->items[0];
    heap->items[0] = heap->items[--heap->size];
    heapify_down(heap, 0, _prioriy);
	return (data);
}

void    *peek(t_heapq *heap)
{
    if (!heap->size)
        return (NULL);
    return (heap->items[0]);
}
