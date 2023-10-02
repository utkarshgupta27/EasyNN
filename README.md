# EasyNN
The EasyNN library provides scalar, matrix and tensor operations in C++. Using the ctypes library, we will create a bridge between the code in Python and C++. The goal is to take advantage of the high computational efficiency of the C++ language to speed up the Neural Networks operations.  These are the operations implemented in EasyNN:

**ReLU()**: apply the ReLU operation element-wise to the elements in the input tensor.

**Flatten()**: after receiving a tensor as an input, it will flatten each example into a vector using row-major order. For example, if the input tensor is in the NCHW format, the output tensor will have two dimensions where shape[0]=N will remain unchanged and shape[1]=CHW. 

**Input2d(name, height, width, in_channels)**: after receiving the input tensor in NHWC format, it will convert it to NCHW format. 

**Linear(weight, bias)**: it will perform the following operation: (weight * x^T + bias)^T , where x is the input tensor in a form of a matrix of N rows and I columns, weight is a matrix with O rows and I columns and bias is a vector of O elements. The output tensor is a matrix of N rows and O columns. 

**MaxPool2d(kernel_size, stride)**: after receiving an input tensor as a 4D tensor with the shape (N, C, H, W), it will perform a two-dimensional MaxPool operation (for each kernel_size × kernel_size patch from one slice per example per channel from the input tensor, their maximum is put into the output tensor). For simplicity, it is assumed that kernel_size = stride. The output will be a 4D tensor with the shape (N, C, H/kernel_size, W/kernel_size). 

**Conv2d(in_channels, out_channels, kernel_size, weight, bias)**: after receiving an input tensor as a 4D tensor with the shape (N, in_channels, H, W), weight as a 4D tensor with the shape (out_channels, in_channels, kernel_size, kernel_size) and bias as a vector with out_channels elements, this function will perform a two-dimensional convolution operation (for each in_channels × kernel_size × kernel_size tensor from one slice per example from the input tensor, it is multiplied element-wise with one slice per out_channels from weight, and then the result elements, plus the corresponding element from bias for out_channels, are added together and put into the output tensor). The output will be a 4D tensor with the shape (N, out_channels, H-kernel_size+1, W-kernel_size+1).
