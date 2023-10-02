#ifndef TENSOR_H
#define TENSOR_H

#include <vector>
#include <string>

class tensor
{

public:
    //Constructors
    tensor(); // scalar
    explicit tensor(double v); //scalar v
    tensor(int dim, size_t shape[], double data[]);
    int get_dim();
    size_t* get_shape_array();
    double* get_data_array();
    std::vector<size_t> get_shape();
    size_t get_elements();
    double get_data_element(int pos);
    void set_data_array(std::vector<double> data);
    void set_data_dim(int dim);
    void set_data_shape(std::vector<size_t> shape);
    void set_data_element(double data, int pos);
    std::vector<double> get_data();
    double at(size_t i) const;
    double at(size_t i, size_t j) const;
    double &get_result();

private:
    int dim_;
    std::vector<size_t> shape_;
    std::vector<double> data_;
}; // class tensor

#endif // TENSOR_H
