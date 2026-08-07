#include "codexion.h"

t_array	*create_heapq(int size)
{
	t_array	*heap;

	heap = NULL;
	if (size <= 0)
		return (NULL);
	while (size--)
		heap = la_append(heap, NULL);
	return (la_start(heap));
}
void	enq_heapq(t_array *heap, void *data, int (_prioriy)(t_array *, t_array))
{
	while (heap && heap->data != NULL)
		heap = heap->next;
	if (!heap)
		return ;
	heap->data = data;
	la_sort(heap, _prioriy);
}

void	*denq_heapq(t_array *heap)
{
	
}
