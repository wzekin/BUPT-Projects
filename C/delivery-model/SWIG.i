%module delivery_model
 
%include "std_vector.i"

%{
/* 在包装代码中包含头文件 */
#include "model.h"
#include "order.h"
#include "point.h"
#include "rider.h"
#include "constant.h"
#include "algorithm.h"
%}


 
/* 解析头文件生成包装代码 */
%include "model.h"
%include "order.h"
%include "point.h"
%include "rider.h"
%include "constant.h"
%include "algorithm.h"

namespace std {
  %template(RiderVector) vector<Rider>;
  %template(IntVector) vector<int>;
  %template(OrderVector) vector<Order>;
  %template(PointVector) vector<Point>;
}
