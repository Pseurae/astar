#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "astar.h"

#define MAP_WIDTH 20
#define MAP_HEIGHT 20

// Heap functions

static int heap_parent(int i)
{
    return (i - 1) / 2;
}

static int heap_left_child(int i)
{
    return 2 * i + 1;
}

static int heap_right_child(int i)
{
    return 2 * i + 2;
}

static void node_swap(struct Node **node1, struct Node **node2)
{
    struct Node *temp = *node1;
    *node1 = *node2;
    *node2 = temp;
}

static void heap_push(struct NodeHeap *heap, struct Node *node)
{
    int32_t i;
    heap->nodes[heap->count] = node;
    heap->count += 1;

    i = heap->count - 1;
    while (i != 0 && heap->nodes[heap_parent(i)]->f > heap->nodes[i]->f)
    {
        node_swap(&heap->nodes[heap_parent(i)], &heap->nodes[i]);
       i = heap_parent(i);
    }
}

void heap_heapify(struct NodeHeap *heap, int i)
{
    int left = heap_left_child(i);
    int right = heap_right_child(i);

    int largest = i;

    if (left <= heap->count && heap->nodes[left]->f < heap->nodes[largest]->f)
        largest = left;

    if (right <= heap->count && heap->nodes[right]->f < heap->nodes[largest]->f)
        largest = right;

    if (largest != i)
    {
        struct Node *temp = heap->nodes[i];
        heap->nodes[i] = heap->nodes[largest];
        heap->nodes[largest] = temp;
        heap_heapify(heap, largest);
    }
}

static struct Node *heap_pop(struct NodeHeap *heap)
{
    struct Node *max_item = heap->nodes[0];

    heap->nodes[0] = heap->nodes[heap->count - 1];
    heap->count -= 1;

    heap_heapify(heap, 0);
    return max_item;
}

// End of Heap functions

// Map functions

static struct Node *map_insert(struct NodeMap *map, uint32_t key, struct Node node)
{
    map->keys[map->count] = key;
    map->nodes[map->count] = node;
    return &map->nodes[map->count++];
}

static bool map_exists(struct NodeMap *map, uint32_t key)
{
    uint16_t i;

    for (i = 0; i < map->count; i++)
    {
        if (map->keys[i] == key)
            return true;
    }

    return false;
}

static struct Node *map_get(struct NodeMap *map, uint32_t key)
{
    uint16_t i;

    for (i = 0; i < map->count; i++)
    {
        if (map->keys[i] == key)
            return &map->nodes[i];
    }

    return NULL;
}

// End of Map functions

static const int8_t child_offsets[4][2] =
{
    {0, 1},
    {0, -1},
    {-1, 0},
    {1, 0},
};

static struct Node node_create_new(uint16_t x, uint16_t y, uint16_t g, uint16_t f)
{
    struct Node node = {x, y, g, f, NULL};
    return node;
}

static struct Node node_create_child(struct Node *parent, struct Node *end, uint8_t i)
{
    int32_t dx, dy;
    struct Node node;

    node.x = parent->x + child_offsets[i][0];
    node.y = parent->y + child_offsets[i][1];
    node.g = parent->g + 1;

    dx = node.x - end->x;
    dy = node.y - end->y;
    node.f = node.g + 2 * (abs(dx) + abs(dy));

    return node;
}

static bool find_path(uint16_t startx, uint16_t starty, uint16_t endx, uint16_t endy)
{
    uint32_t index;
    int i, steps;
    struct Node *node, child, *inserted, startnode, endnode;
    struct NodeMap map = { 0 };
    struct NodeHeap heap = { 0 };
    int32_t width, height;

    map.keys = calloc(sizeof(uint32_t), MAP_NODES);
    map.nodes = calloc(sizeof(struct Node), MAP_NODES);
    heap.nodes = calloc(sizeof(struct Node *), HEAP_NODES);

    width = MAP_WIDTH;
    height = MAP_HEIGHT;

    startnode = node_create_new(startx, starty, 0, 0);
    endnode = node_create_new(endx, endy, 0, 0);

    printf("Starts: %d %d\n", startnode.x, startnode.y);
    printf("Ends: %d %d\n", endnode.x, endnode.y);

    heap_push(&heap, &startnode);

    while (heap.count)
    {
        node = heap_pop(&heap);

        printf("Popped %d, %d\n", node->x, node->y);

        if (node->x == endx && node->y == endy)
        {
            printf("\nFinal path:\n");
            while (node && i >= 0)
            {
                printf("%d %d\n", node->x, node->y);
                node = node->parent;
            }

            free(map.nodes);
            free(heap.nodes);
            return true;
        }

        if (heap.count < HEAP_NODES && map.count < MAP_NODES)
        {
            for (i = 0; i < 4; i++)
            {
                child = node_create_child(node, &endnode, i);

                // if (CollisionCheck(x, y))
                    // continue;

                index = child.x + child.y * width;
                child.parent = node;

                printf("Checking neighbour: %d %d (%u %u)\n", child.x, child.y, index, child.f);

                if (!map_exists(&map, index) || child.f < node->f)
                {
                    if (map.count < MAP_NODES)
                    {
                        inserted = map_insert(&map, index, child);
                        if (heap.count < HEAP_NODES)
                        {
                            heap_push(&heap, inserted);
                            printf("Inserted %d %d\n", inserted->x, inserted->y);
                        }
                    }
                }
            }
        }
    }

    printf("No path.\n");
    free(map.nodes);
    free(heap.nodes);
    return false;
}

int main(int argc, char *argv[])
{
    find_path(0, 0, 19, 19);
    return 0;
}