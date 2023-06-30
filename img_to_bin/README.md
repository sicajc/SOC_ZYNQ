# IMG To BIN
1. Within the HxD, the first 8 byte is header, then RGB.
2. To use a matlab and write it out as a binary format for image processing. Like .tiff file, this must be done.
3. When searching for the correct value or estimation, having a calculator is extremely important.

```
    i = imread('lena_colr.tiff');
    outFi;e = fopen();


```
4. Matlab would store 512x512 R, 512x512G and 512x512B as Binary.
5. Matlab stores the value columnwise.
6. Thus we must convert the image into the correct format through a script conversion.
7. Note after the conversion, we are storing it row wise. But to display, we have to convert it to columnwise.