#include <time.h>
#include <stdlib.h>

double current_sec(void){
  static time_t base_sec = 0;
  time_t current_sec;
  long extra_nsec;
  struct timespec ts;
  double result;

  timespec_get(&ts, TIME_UTC);
  current_sec = ts.tv_sec;
  extra_nsec = ts.tv_nsec;

  if(base_sec==0) base_sec = current_sec; /* 初回実行時のみ実行される */

  result = (double)difftime(current_sec, base_sec) + ((double)extra_nsec) / 1000000000.0;

  return result;
}

int _get_rand_int(int min, int max){
  return min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}

double _get_rand_double(double min, double max){
  return (double)rand()/RAND_MAX * (max-min) + min;
}

void _init_randam(double a[], int n){
  int i;
  for(i=0;i<n;i++){
    a[i] = rand() / (RAND_MAX + 1.0);
  }
  return;
}

void _init_roughly_ramp(double a[], int n, double roughness, int is_desc){
  int i;
  int swap_count;
  int pivot;
  int idx1, idx2;
  double temp;

  if(is_desc){
    for(i=0;i<n;i++) a[n-i-1] = (double)i / (double)n;
  }else{
    for(i=0;i<n;i++) a[i] = (double)i / (double)n;
  }

  swap_count = (int)((n/2 + 1/roughness - 1) * roughness);
  for(i=0;i<swap_count;i++){
    pivot = _get_rand_int(0, n-2);
    idx1 = _get_rand_int(0, pivot);
    idx2 = _get_rand_int(pivot+1,n-1);
    temp = a[idx1];
    a[idx1] = a[idx2];
    a[idx2] = temp;
  }
  return;
}

void init(double a[], int n, int type){
  static first_flag = 1;
  if(first_flag){ /* 初回実行時のみ実行される */
    srand((unsigned int)time(NULL));
    first_flag = 0;
    rand(); /* srand後1度目の呼び出しはランダム性に欠けるのでここで呼び出し */
  }

  switch(type){
    case 0:
      _init_randam(a, n);
      break;
    case 1:
      _init_roughly_ramp(a, n, 0.1, 0);
      break;
    case 2:
      _init_roughly_ramp(a, n, 0.1, 1);
      break;
    default:
      exit(-1);
  }
  return;
}

void init_m(double a[], int n){
  static first_flag = 1;
  int i;
  if(first_flag){ /* 初回実行時のみ実行される */
    srand((unsigned int)time(NULL));
    first_flag = 0;
    rand(); /* srand後1度目の呼び出しはランダム性に欠けるのでここで呼び出し */
  }

  for(i=0;i<n;i++){
    a[i] = _get_rand_double(-1,1);
  }
  return;
}

