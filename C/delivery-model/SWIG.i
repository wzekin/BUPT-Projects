%module delivery_model
 
%include "std_vector.i"

%{
/* �ڰ�װ�����а���ͷ�ļ� */
#include "model.h"
#include "order.h"
#include "point.h"
#include "rider.h"
#include "constant.h"
#include "algorithm.h"
%}


 
/* ����ͷ�ļ����ɰ�װ���� */
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
