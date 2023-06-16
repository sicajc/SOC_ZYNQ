# Point processing
1. Input image, do some operation on image then get the output image.
2. Output image is only depends on the function and the input image.

# Neighborhood processing
1. The pixel value depends on the neighboring pixels too.
2. Take in an image, perform kernal operation onto the image using a mask depending on what you are trying to accomplish. MAC

# Data transfer problem
1. Pure streaming based approach is not possible for this image processing core! We must buffer the data within the IP.
2. Not practical to buffer the entire image within the IP. BRAMS on quite limited.
3. Practically, we would only buffered a part of image within the IP.

# Line Buffers
1. Small RAMs for storing 1 line of image pixels.
2. Size of Line buffers is determined by the width of image.
3. In convolution, same line buffer is used for multiple times.
4. We can improve performance by adding 4th line buffer, while convolution is in progress, we can store the image pixel into the 4th line buffer, originally 3.

# Interrupt
1. Once one line processing is over, the convolutioned result would be sent through DMA using interrupt to the DDR.


# DMA problem in block design1
1.