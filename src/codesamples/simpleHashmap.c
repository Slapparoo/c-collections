#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 20

struct DataItem
{
    int data;
    int key;
};

struct DataItem *hashArray[SIZE];
struct DataItem *dummyItem;
struct DataItem *item;

int hashCode(int key)
{
    key ^= (key << 5) * (key << 11) ^ (key << 13);
    return key % SIZE;
}


int itemIndex(int key)
{
    //get the hash
    int hashIndex = hashCode(key);

    //move in array until an empty
    while (hashArray[hashIndex] != NULL)
    {
        if (hashArray[hashIndex]->key == key)
            return hashIndex;

        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    return -1;
}



struct DataItem *search(int key)
{
    int ix = itemIndex(key);
    if (ix == -1) {
        return NULL;
    }
    return hashArray[ix];
}

void insert(int key, int data)
{
    struct DataItem *item = (struct DataItem *)malloc(sizeof(struct DataItem));
    item->data = data;
    item->key = key;

    //get the hash
    int hashIndex = hashCode(key);

    //move in array until an empty or deleted cell
    while (hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1)
    {
        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    hashArray[hashIndex] = item;
}

struct DataItem *delete (struct DataItem *item)
{
    int key = item->key;


    int ix = itemIndex(key);
    if (ix == -1) {
        return NULL;
    }
    struct DataItem *temp = hashArray[ix];

    //assign a dummy item at deleted position
    hashArray[ix] = dummyItem;
    return temp;    
}

void display()
{
    int i = 0;

    for (i = 0; i < SIZE; i++)
    {
        if (hashArray[i] != NULL)
            printf(" (%d,%d)", hashArray[i]->key, hashArray[i]->data);
        else
            printf(" ~~ ");
    }

    printf("\n");
}

int main()
{
    dummyItem = (struct DataItem *)malloc(sizeof(struct DataItem));
    dummyItem->data = -1;
    dummyItem->key = -1;

    for (int i = 0; i < 15; i++)
    {
        insert(i, i);
    }
    //    insert(1, 20);
    //    insert(2, 70);
    //    insert(42, 80);
    //    insert(4, 25);
    //    insert(12, 44);
    //    insert(14, 32);
    //    insert(17, 11);
    //    insert(13, 78);
    //    insert(37, 97);

    display();
    item = search(31);

    if (item != NULL)
    {
        printf("Element found: %d\n", item->data);
        delete (item);
    }
    else
    {
        printf("Element not found\n");
    }

    item = search(3);

    if (item != NULL)
    {
        printf("Element found: %d\n", item->data);
        delete (item);
    }
    else
    {
        printf("Element not found\n");
    }


    item = search(37);

    if (item != NULL)
    {
        printf("Element found: %d\n", item->data);
    }
    else
    {
        printf("Element not found\n");
    }
    display();
}