#include "codexion.h"

static void	swap(void **a, void **b)
{
	void	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

static void heapify_up(t_heapq *h, int index, int (_prioriy)(void *, void *))
{
    void *root;

    root = h->items[(index-1)/2];
    if (index > 0 && _prioriy(h->items[index], root))
        return (swap(&h->items[(index-1)/2], &h->items[index]), heapify_up(h, (index - 1) / 2, _prioriy));
}

void	enq_heapq(t_heapq *heap, void *data, int (_prioriy)(void *, void *))
{
    if (heap->size == MAX)
        return ;
    heap->items[heap->size++] = data;
    heapify_up(heap, heap->size-1, _prioriy);
}

static void heapify_down(t_heapq *heap, int index, int (_prioriy)(void *, void *))
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

void	*deq_heapq(t_heapq *heap, int (_prioriy)(void *, void *))
{
    void    *data;

    if (!heap->size)
        return (NULL);
    data = heap->items[0];
    heap->items[0] = heap->items[--heap->size];
    heapify_down(heap, 0, _prioriy);
	return (data);
}
