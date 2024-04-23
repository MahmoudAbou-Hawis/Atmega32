## Overview:
this is the implementation of a queue data structure in C.  A queue operates on the First In First Out (FIFO) principle, meaning the elements are added to the back (push) and removed from the front (pop).



## Features:

- **Fixed-size array :** predefined size eliminates memory allocation overhead.

- **Efficient operations :** Operations like push and pop are generally faster compared to dynamic queues due to less memory management.

- **Basic operations** :

  - `QUEUE_enuInit` Used to initialize the queue. 
  - `QUEUE_enuPush :` Adds an element to the back of the queue if not full.
  - `QUEUE_enuPop :` Removes and return the element at the front of the queue if not empty.
  - `QUEUE_enuGetRear` return the element at the end of the queue.

  

## Example Usage:

`````c
#include "queue.h"

int main(void)
{
    queue q;

    uint8_t val;

    QUEUE_enuInit(&q);

    QUEUE_enuPush(&q, 5);

    QUEUE_enuGetRear(&q, &val);

    QUEUE_enuPop(&q, &val);
    
    return 0;
}

`````



## Additional Notes:

- This implementation assumes the data type being stored in the queue is `int` .
- You should change the queue size from the `queue_CFG.h`  configuration file.



**Contact**

-  <a href="mailto:mahmoud.abohawis10@gmail.com" style="text-decoration: none;">Gmail</a>

-  <a href="https://www.linkedin.com/in/abou-hawis/" style="text-decoration: none;">LinkedIn Profile</a>



**Author**

**Mahmoud Abou-Hawis**

