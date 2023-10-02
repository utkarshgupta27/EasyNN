#include <assert.h>
#include "evaluation.h"
#include "tensor.h"
#include <iostream>

// Constructor 
evaluation::evaluation(const std::vector<expression> &exprs)
    :
    // Constructor initializer: data member (value) 
    result_(0),
    exprs_(exprs)  
{
}

void evaluation::add_kwargs_double(
    const char *key,
    double value)
{
    int dim=1;
    size_t shape[]={1};
    tensor *ten = new tensor(value);
    variables_[key]=*ten;
}

void evaluation::add_kwargs_ndarray(
    const char *key,
    int dim,
    size_t shape[],
    double data[])
{
    tensor *ten = new tensor(dim,shape,data);
    variables_[key]=*ten;
}

int evaluation::execute(){
    std::map<int, tensor> values;
    values.clear();
    for (auto iter = exprs_.begin() ; iter != exprs_.end() ; ++iter){

        std::cout << iter->op_type_ <<"\n";

        if(iter->op_type_ == "Input"){
            auto it = variables_.find(iter->op_name_);
            values[iter->expr_id_]=it->second;

        }else if (iter->op_type_ == "Const"){            
            auto it2 = iter->op_param_.find("value");
            values[iter->expr_id_]=it2->second;
            
        } else if (iter->op_type_ == "Add"){
            tensor sum1;
            tensor sum2;
            for (auto iter1 = exprs_.begin() ; iter1 != exprs_.end() ; ++iter1){
                if ( iter1->expr_id_ == iter->inputs_[0] ){
                    sum1=values[iter1->expr_id_];
                }
                if( iter1->expr_id_ == iter->inputs_[1] ){
                    sum2=values[iter1->expr_id_];
                }               
            }
            std::vector<double> suma;
            suma.clear();
            size_t N=1;
            N=sum1.get_elements();            
            for (size_t i=0 ; i<N ; i++){
                suma.push_back(sum1.at(i) + sum2.at(i));
            }                       
            sum2.set_data_array(suma);
            values[iter->expr_id_]=sum2 ;

        } else if (iter->op_type_ == "Mul"){
            tensor mul1;
            tensor mul2;
            for (auto iter1 = exprs_.begin() ; iter1 != exprs_.end() ; ++iter1){
                if ( iter1->expr_id_ == iter->inputs_[0] ){
                    mul1=values[iter1->expr_id_];
                }
                if( iter1->expr_id_ == iter->inputs_[1] ){
                    mul2=values[iter1->expr_id_];
                }
            }
            std::vector<double> mul;
            mul.clear();
            size_t N=1;
            double res=0;
            if(mul1.get_dim()>1 & mul2.get_dim()>1){
                size_t fila1=mul1.get_shape()[0];
                size_t col1=mul1.get_shape()[1];
                size_t fila2=mul2.get_shape()[0];
                size_t col2=mul2.get_shape()[1];

                for (size_t i=0 ; i<fila1 ; i++){ 
                    for (size_t j=0 ; j<col2 ; j++){ 
                        for (size_t k=0 ; k<col1 ; k++){
                            res= res +(mul1.at(i,k) * mul2.at(k,j));
                        }                        
                    mul.push_back(res);  
                    res=0;  
                    }                
                } 
                std::vector<size_t> shape={fila1,col2};
                mul2.set_data_array(mul);
                mul2.set_data_shape(shape);
                values[iter->expr_id_]=mul2;

            }else if (mul1.get_dim()<=1){
                for (size_t i=0 ; i<mul2.get_shape()[0] ; i++){ 

                    for (size_t j=0 ; j<mul2.get_shape()[1] ; j++){ 

                        mul.push_back(mul1.at(0) * mul2.at(i,j));
                        
                    }
                }
            mul2.set_data_array(mul);
            values[iter->expr_id_]=mul2;
            }else if (mul2.get_dim()<=1){
                for (size_t i=0 ; i<mul1.get_shape()[0] ; i++){ 

                    for (size_t j=0 ; j<mul1.get_shape()[1] ; j++){ 

                        mul.push_back(mul1.at(i,j) * mul2.at(0));
                        
                    }
                }
            mul1.set_data_array(mul);
            values[iter->expr_id_]=mul1;
            }
        }else if (iter->op_type_ == "Sub"){

            tensor sub1;
            tensor sub2;

            for (auto iter1 = exprs_.begin() ; iter1 != exprs_.end() ; ++iter1){
                if ( iter1->expr_id_ == iter->inputs_[0] ){
                    sub1=values[iter1->expr_id_];
                }
                if( iter1->expr_id_ == iter->inputs_[1] ){
                    sub2=values[iter1->expr_id_];                
                }
            }
            std::vector<double> sub;
            sub.clear();
            size_t N=1;
            N=sub1.get_elements();            
            for (size_t i=0 ; i<N ; i++){

                sub.push_back(sub1.at(i) - sub2.at(i));
            }                       
            sub2.set_data_array(sub);
            values[iter->expr_id_]=sub2 ;
        }else if (iter->op_type_ == "ReLU"){

            tensor relu1;

            for (auto iter1 = exprs_.begin() ; iter1 != exprs_.end() ; ++iter1){
                if ( iter1->expr_id_ == iter->inputs_[0] ){
                    relu1=values[iter1->expr_id_];
                    for (size_t i=0 ; i<relu1.get_elements() ; i++){
                        if(relu1.at(i) < 0){
                            relu1.set_data_element(0,i);
                        }
                    }
                    values[iter->expr_id_]= relu1;
                }
            }            
        }else if (iter->op_type_ == "Flatten"){
            tensor flatten1;
    
            for (auto iter1 = exprs_.begin() ; iter1 != exprs_.end() ; ++iter1){
                if ( iter1->expr_id_ == iter->inputs_[0] ){
                    flatten1=values[iter1->expr_id_];                    
                    std::vector<size_t> sec=flatten1.get_shape();
                    size_t temp=1;
                    for (size_t i=1 ; i<flatten1.get_dim() ; i++){                        
                        temp = temp*sec[i];
                    }
                    std::vector<size_t> shape={flatten1.get_shape()[0], temp};
                    flatten1.set_data_dim(2);
                    flatten1.set_data_shape(shape);                   
                    values[iter->expr_id_]= flatten1;                   
                }
            }            
        }  else if (iter->op_type_ == "Input2d"){
            tensor input2d1;
            input2d1 = variables_.find(iter->op_name_)->second;
            size_t n = input2d1.get_shape()[0];
            size_t h = input2d1.get_shape()[1];
            size_t w = input2d1.get_shape()[2];
            size_t c = input2d1.get_shape()[3];
            input2d1.set_data_shape({n,c,h,w});
            std::vector<double> data=input2d1.get_data();
            for (size_t v=0 ; v<n ; v++){
                for (size_t i=0 ; i<h ; i++){                            
                    for (size_t j=0 ; j<w ; j++){                        
                        for (size_t k=0 ; k<c ; k++){
                            input2d1.set_data_element(data[v*c*h*w + i*w*c + j*c + k],(v*c*h*w + k*h*w + i*w + j) );
                        }
                    }
                }
            }    
            values[iter->expr_id_]= input2d1;
        } else if (iter->op_type_ == "Linear"){
            tensor input;
            tensor weight;
            tensor bias;
            
            for (auto iter1 = exprs_.begin() ; iter1 != exprs_.end() ; ++iter1){
                if ( iter1->expr_id_ == iter->inputs_[0] ){
                    input = values[iter1->expr_id_];
                }
                weight = iter->op_param_.find("weight")->second;
                bias = iter->op_param_.find("bias")->second;
            }
            std::vector<double> out;
            for (size_t i =0 ; i<input.get_shape()[0] ; i++){
               
                std::vector<double> inter;
                inter.clear();
                size_t o = weight.get_shape()[0];
                size_t in = weight.get_shape()[1];
                for (size_t j=0 ; j<o ; j++){
                    double res=0;
                    for (size_t k=0 ; k<in ; k++){
                        res = res + (weight.at(j,k) * input.at(i,k));
                    }
                    inter.push_back(res);
                }            
                for (size_t v=0 ; v<bias.get_shape()[0] ; v++){
                    out.push_back(inter.at(v) + bias.at(v));
                }             
            }
            input.set_data_array(out); 
            std::vector<size_t> shape={input.get_shape()[0],weight.get_shape()[0]};
            input.set_data_shape(shape);          
            values[iter->expr_id_]= input;            
        } else if (iter->op_type_ == "MaxPool2d"){
            tensor x;
            tensor kernel_size;
            tensor stride;
            for (auto iter1 = exprs_.begin() ; iter1 != exprs_.end() ; ++iter1){
                x=values.rbegin()->second;
                kernel_size = iter->op_param_.find("kernel_size")->second;
            }
            double patch = kernel_size.at(0) * kernel_size.at(0);            
            size_t n = x.get_shape()[0];
            size_t c = x.get_shape()[1];
            size_t h = x.get_shape()[2];
            size_t w = x.get_shape()[3];
            std::vector<double> out;
            double k=0;
            for (size_t u=0 ; u<n ; u++){
                for (size_t v=0 ; v<c ; v++){
                    for (size_t i=0 ; i<(h/kernel_size.at(0)) ; i++){
                        for (size_t j=0 ; j<(w/kernel_size.at(0)) ; j++){
                            double max=0;
                            for (size_t k=0 ; k<kernel_size.at(0) ; k++){                                    
                                for (size_t m=0 ; m<kernel_size.at(0) ; m++){
                                    double b= x.get_data_element(u*c*w*h + v*w*h + i*kernel_size.at(0)*w + j*kernel_size.at(0) + w*k + m);
                                    if (max<b){
                                        max = x.get_data_element(u*1*c*w*h + v*w*h + i*kernel_size.at(0)*w + j*kernel_size.at(0) + w*k + m);
                                    }
                                }
                            }
                            out.push_back(max);
                        }
                    }
                }
            }
            x.set_data_array(out);
            std::vector<size_t> shape = {n , c, h/kernel_size.at(0), w/kernel_size.at(0)};
            x.set_data_shape(shape);
            values[iter->expr_id_]= x;            
        } else if (iter->op_type_ == "Conv2d"){
            tensor x;
            tensor kernel_size;
            tensor weight;
            tensor bias;
            tensor output;
            for (auto iter1 = exprs_.begin() ; iter1 != exprs_.end() ; ++iter1){
                if ( iter1->expr_id_ == iter->inputs_[0] ){                    
                    x = values[iter1->expr_id_];
                }
                kernel_size = iter->op_param_.find("kernel_size")->second;
                weight = iter->op_param_.find("weight")->second;
                bias = iter->op_param_.find("bias")->second; 
                output = iter->op_param_.find("out_channels")->second; 
            }
            size_t n = x.get_shape()[0];
            size_t c = x.get_shape()[1];
            size_t h = x.get_shape()[2];
            size_t w = x.get_shape()[3];
            size_t out_channels = weight.get_shape()[0];
            std::vector<double> out;

            for (size_t u=0 ; u<n ; u++){
                for (size_t z=0 ; z<out_channels ; z++){
                    for (size_t i=0 ; i<(h-kernel_size.at(0)+1) ; i++){
                        for (size_t j=0 ; j<(w-kernel_size.at(0)+1) ; j++){
                            double res=0;
                            double cont =0;
                            for (size_t v=0 ; v<c ; v++){                            
                                for (size_t k=0 ; k<kernel_size.at(0) ; k++){                                    
                                    for (size_t m=0 ; m<kernel_size.at(0) ; m++){
                                        double a= x.get_data_element(u*c*w*h + v*w*h + i*w + j + w*k + m);
                                        double wg= weight.get_data_element(kernel_size.at(0)*kernel_size.at(0)*c*z + cont);
                                        res = res + a*wg;
                                        cont++;
                                    }
                                }                            
                            }
                            res = res + bias.get_data_element(z);
                            out.push_back(res);
                        }
                    }
                }
            }
            x.set_data_array(out);
            std::vector<size_t> shape = {n , output.get_data_element(0), h-kernel_size.at(0)+1, w-kernel_size.at(0)+1};
            x.set_data_shape(shape);
            values[iter->expr_id_]= x;
        }        
    }
    set_result(values.rbegin()->second);

    return 0;
}

void evaluation::set_result(tensor valor){
    result_= valor;
}

tensor &evaluation::get_result(){
    return result_;
}

