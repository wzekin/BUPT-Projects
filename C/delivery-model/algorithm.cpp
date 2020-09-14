#include "algorithm.h"

const static int INTMAX = INT_MAX / 2;
std::tuple<std::queue<Point>, int> Algorithm::cal_solution(
    const Rider &r, const Order &new_order, const int time) {

  int sending_num = r.sending_orders().size();					//�������͵Ķ���
  int received_num = r.received_orders().size() + 1;    //�ѽӵ�������û���͵Ķ��� (������order���㣩
  int n = sending_num + received_num * 2;             //����Ҫ�����ĵ������
  Point *P = new Point[n];                            //���еĵ�
  int *last_time = new int[n];                        //���еĵ��ڲ���ʱ���������µ����ʱ��
  int **is_outdate = new int *[n];                    //״̬�Ƿ��Ѿ��ж�����ʱ
  int **G = new int *[n];                             //��������� n*n��ά����
  int **from = new int *[n];                          //��ǰ״̬�����һ������ĵ� n*(2^n)��ά����
  int **dp = new int *[n];                            //��¼��ǰ״̬���ѵ�ʱ��
  int index_from = 2;                                 // ���

  //��ʼ������
  for (int i = 0; i < n; i++) {
    last_time[i] = INTMAX;
    G[i] = new int[n];
    is_outdate[i] = new int[(long long)1 << n];
    from[i] = new int[(long long)1 << n];
    dp[i] = new int[(long long)1 << n];
    for (int j = 0; j < n; j++) {
      G[i][j] = INTMAX;
    }
    for (int j = 0; j < ((long long)1 << n); j++) {
      is_outdate[i][j] = 0;
      from[i][j] = -1;
      dp[i][j] = INTMAX;
    }
  }

  //��ʼ��P
  P[0] = new_order.restaurant;  //�������ӵĶ���������ֵ
  P[1] = new_order.dinner;
  last_time[1] =
      OUTDATE_TIME * 2 - 2;  // -2 �Ժ��������پ�����������������ͬ��
  for (auto it_from = r.received_orders().begin();
       it_from != r.received_orders().end(); it_from++) {
    P[index_from] = (*it_from).restaurant;
    P[index_from + 1] = (*it_from).dinner;
    last_time[index_from + 1] =
        (OUTDATE_TIME - (time - (*it_from).time)) * 2 - 2;  //��2�Է����cost ��2��ƽ�������������
    index_from += 2;
  }
  for (auto it_from = r.sending_orders().begin();
       it_from != r.sending_orders().end(); it_from++) {
    P[index_from] = (*it_from).dinner;
    last_time[index_from] = (OUTDATE_TIME - (time - (*it_from).time)) * 2 - 2; // ��2��ƽ�������������
    index_from += 1;
  }

  //��G��dp��ֵ
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      G[i][j] = G[j][i] = Point::GetDistant(P[i], P[j]);
    }
    dp[i][1 << i] = Point::GetDistant(r.position(), P[i]);
  }

  //��dp����;
  for (int S = 0; S < (1 << n); S++)
    for (int i = 0; i < n; ++i)
      if (S & (1 << i) && Check(S, i, received_num))  //�жϴ�״̬�Ƿ�Ϸ�
        for (int j = 0; j < n; ++j)
          if (!(S & (1 << j)) &&
              G[i][j] != INTMAX) {  // �ж��Ƿ񵽹�j ��i j �Ƿ�ɴ�
            if (dp[j][S | (1 << j)] > dp[i][S] + G[i][j]) {  //�ж��Ƿ���Ҫ����
              if (dp[i][S] + G[i][j] > last_time[j] ||
                  is_outdate[i][S]) {  //�жϴ�״̬�Ƿ�ʱ
                if (dp[j][S | (1 << j)] == INTMAX ||
                    is_outdate[j][S | (1 << j)] ==
                        1) {  // �ж��ѳ�ʱ״̬�Ƿ���Ը���
                  dp[j][S | (1 << j)] = dp[i][S] + G[i][j];
                  from[j][S | (1 << j)] = i;
                  is_outdate[j][S | (1 << j)] = 1;
                }
              } else {
                dp[j][S | (1 << j)] = dp[i][S] + G[i][j];
                from[j][S | (1 << j)] = i;
                is_outdate[j][S | (1 << j)] = 0;
              }
            }
          }

  //�ҳ�cost��С�Ҳ��ᳬʱ��·��
  int index = 0, all_cost = INTMAX, is_out = 1;
  for (int i = 0; i < n; i++) {
    if (dp[i][(1 << n) - 1] < all_cost &&
        !(is_outdate[i][(1 << n) - 1] == 1 && is_out == 0)) {
      index = i;
      is_out = is_outdate[i][(1 << n) - 1];
      all_cost = dp[i][(1 << n) - 1];
    } else if (is_outdate[i][(1 << n) - 1] == 0 && is_out == 1) {
      index = i;
      is_out = is_outdate[i][(1 << n) - 1];
      all_cost = dp[i][(1 << n) - 1];
    }
  }

  //�õ�·��
  std::queue<Point> path;
  GetPath(index, (1 << n) - 1, from, P, path);

  //�ͷ���Դ
  for (int i = 0; i < n; i++) {
    delete[] is_outdate[i];
    delete[] G[i];
    delete[] from[i];
    delete[] dp[i];
  }
  delete[] last_time;
  delete[] is_outdate;
  delete[] G;
  delete[] from;
  delete[] dp;
  return {path, all_cost};
}

bool Algorithm::Check(int S, int i, int received_num) {
  if (i < 2 * received_num && i % 2 == 1 && !(S & (1 << (i - 1)))) return false;
  return true;
}

void Algorithm::GetPath(int end, int S, int **from, Point P[],
                        std::queue<Point> &path) {
  if (end != -1) {
    GetPath(from[end][S], S ^ 1 << end, from, P, path);
    path.push(P[end]);
  }
}
