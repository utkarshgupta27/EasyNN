#include "tensor.h"

// Constructor
tensor::tensor()
    :
    // Constructor initializer: data member (value)
    data_(1,0)
{
}
tensor::tensor(double v)
    :
    // Constructor initializer: data member (value)
    data_(1,v),
    dim_(1),
    shape_(1)
{
}
tensor::tensor(int dim, size_t shape[], double data[])
    :
    // Constructor initializer: data member (value)
    shape_(shape, shape+dim),
    dim_(dim)
{    
    int N=1;
    for (size_t i=0 ; i<dim ; i++){
    
        N=N*shape[i];    
        printf("shape[i] %zu \n",shape[i]);
   }
   data_.assign(data, data+N);
   printf("N: %d \n", N);
}

int tensor::get_dim(){

    return dim_;
}

size_t* tensor::get_shape_array(){

    return shape_.empty()? nullptr: &shape_[0];
}

double* tensor::get_data_array(){

    return &data_[0];
}
std::vector<double> tensor::get_data(){

    return data_;
}
double tensor::at(size_t i) const{

    return data_[i];
}
double tensor::at(size_t i, size_t j) const{

    return data_[i*shape_[1] + j];
}
double tensor::get_data_element(int pos){

    return data_[pos];
}
std::vector<size_t> tensor::get_shape(){

    return shape_;
}
size_t tensor::get_elements(){
    
    size_t N=1;
    for (size_t i=0 ; i<dim_ ; i++){

            N=N*shape_[i];
        } 
    return N;
}

void tensor::set_data_array(std::vector<double> data){

    data_=data;
}
void tensor::set_data_dim(int dim){

    dim_=dim;
}
void tensor::set_data_shape(std::vector<size_t> shape){

    shape_=shape;
}

void tensor::set_data_element(double data, int pos){

    data_[pos]=data;
}
