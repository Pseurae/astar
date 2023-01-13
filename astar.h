#ifndef GUARD_PATHFINDING_H
#define GUARD_PATHFINDING_H

#define HEAP_NODES 200
#define MAP_NODES (HEAP_NODES * 4)

struct Node
{
    int16_t x, y;
    uint16_t g, f;
    struct Node *parent;
};

struct NodeMap
{
    uint16_t count;
    uint32_t *keys;
    struct Node *nodes;
};

struct NodeHeap
{
    uint16_t count;
    struct Node **nodes;
};

#endif // GUARD_PATHFINDING_H