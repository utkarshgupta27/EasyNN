#ifndef EVALUATION_H
#define EVALUATION_H

#include "expression.h"
#include "tensor.h"

class evaluation
{
public:      
    // Constructor 
    evaluation(const std::vector<expression> &exprs);

    void add_kwargs_double(
        const char *key,
        double value);

    void add_kwargs_ndarray(
        const char *key,
        int dim,
        size_t shape[],
        double data[]);

    // return 0 for success
    int execute();

    void set_result(tensor valor);

    tensor &get_result();

private:
    tensor result_;
    const std::vector<expression> exprs_;
    std::map<std::string, double> kwargs_;

    std::map<std::string, tensor> variables_;
}; // class evaluation

#endif // EVALUATION_H
