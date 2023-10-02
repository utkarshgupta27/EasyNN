#include "expression.h"

// constructor
expression::expression(
    int expr_id,
    const char *op_name,
    const char *op_type,
    int *inputs,
    int num_inputs) 
    :
    // constructor initializer: data member (value)
    expr_id_(expr_id), 
    op_name_(op_name),
    op_type_(op_type),
    inputs_(inputs, inputs+num_inputs)
    
{
}

void expression::add_op_param_double(
    const char *key,
    double value)
{
    op_param_[key]=tensor(value);
}

void expression::add_op_param_ndarray(
    const char *key,
    int dim,
    size_t shape[],
    double data[])
{
    op_param_[key]=tensor(dim,shape,data);
    
}

