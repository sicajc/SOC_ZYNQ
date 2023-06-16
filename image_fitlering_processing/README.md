# Filtering IP
1. We can actually import source code from SDK.
2. Note the Heap size must be allocated and adjust according to the data size you want to send.
3. Sometimes the malloc would also fail, check if the value is less than 0, to see if failing, the error handling code.
4. When using array representation for passing in a pointer, i.e. image[0] must add &image[0] for passing in the pointer you want, &image[header] this mean only start transfer from the header address.