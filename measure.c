#include <stdio.h>
#include <stdlib.h>

double current_sec(void);
void init(double a[], int n, int type);
void quick_sort(double a[], int left, int right);

void print_array(double a[], int n, char endwith[]); /* 配列を表示 */
void copy_array(double src[], int n, double dest[]); /* 配列srcの内容を配列destへコピー dest配列に対して破壊的 */
int is_ordered(double a[], int n); /* 配列が昇順かどうか確認(非破壊的) */
int is_equal(double a1[], double a2[], int n); /* 配列a1とa2の要素が順番含め等しいか確認(非破壊的) */
double measure_sort(void (*algo)(double a[], int n), double a[], int n, double a_val[]); /* aを変更することなく(非破壊的に)ソート完了までの時間を計測 a_valはソート済みの配列で、これと結果を比較もする */

double**** alloc_result_table(int use_n_count, int sort_count, int init_type_count, int sample_size); /* 測定結果を書き込む表を用意する */
void free_result_table(int use_n_count, int sort_count, int init_type_count, double**** table); /* 表を解放する */

double get_median(double a[], int n); /* 配列aの中央値を返す(破壊的) */

#define NAME_MAX_LEN 32
void print_result(double**** result_table, FILE* f, int use_n_list[], int use_n_count, char sort_names[][NAME_MAX_LEN], int sort_count, char init_type_names[][NAME_MAX_LEN], int init_type_count, int sample_size); /* 結果の表を受け取って、中央値を求めて表の形式でfに出力する */

void print_csv(double**** result_table, FILE* f, int use_n_list[], int use_n_count, char sort_names[][NAME_MAX_LEN], int sort_count, char init_type_names[][NAME_MAX_LEN], int init_type_count, int sample_size); /* 結果の表を受け取って、中央値を求めてcsv形式でfに出力する */


void bsort(double a[], int n); /* 配列aをバブルソートで昇順にする(破壊的) */
void ssort(double a[], int n); /* 配列aを選択ソートで昇順にする(破壊的) */
void isort(double a[], int n); /* 配列aを挿入ソートで昇順にする(破壊的) */
void myqsort(double a[], int n); /* 配列aをクイックソートで昇順にする(破壊的) */
void hsort(double a[], int n); /* 配列aをヒープソートで昇順にする(破壊的) */
void msort(double a[], int n); /* 配列aをマージソートで昇順にすうｒ(破壊的) */
void rsort(double a[], int n); /* 配列aを基数ソートで昇順にする(破壊的) [極めて重要な注意点]:負の数未対応 空間計算量を犠牲に速度を優先 */
void bgsort(double a[], int n); /* 配列aをボゴソートで昇順にする(破壊的) [注意点]:nによっては現実的な時間で終了しない */


int main(void){
  int use_n_list[] = {100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600, 51200, 102400}; /* 測定するnのリスト */
  int use_n_count = 3; /* nのリストの長さ */
  int use_n_idx;

  char sort_names[][NAME_MAX_LEN] = {"Bubble sort", "Selection sort", "Insertion sort", "Quick sort", "Heap sort", "Merge sort", "Radix sort"}; /* ソートアルゴリズムの表示名リスト */
  void (*sort_algos[])(double a[], int n) = {bsort, ssort, isort, myqsort, hsort, msort, rsort}; /* 表示名リストに対応する関数のアドレスのリスト */
  int sort_count = 7; /* ソートアルゴリズムの数 */
  int sort_idx;

  char init_type_names[][NAME_MAX_LEN] = {"ramdom", "approx_asc", "approx_desc"}; /* 初期化タイプの表示名 */
  int init_type_arguments[] = {0, 1, 2}; /* 初期化タイプのリストに対応する、初期化関数への引数 */
  int init_type_count = 3; /* 初期化タイプの数 */
  int init_type_idx;

  int sample_size = 7; /* 測定の反復数 */
  int sample_idx;

  double**** results = alloc_result_table(use_n_count, sort_count, init_type_count, sample_size); /* 結果を入れる表 results[use_n_idx][sort_idx][init_type_idx][sample_idx]とアクセスできる */

  FILE* text_file = stdout;
  FILE* csv_file = stdout;

  for(use_n_idx=0;use_n_idx<use_n_count;use_n_idx++){ /* 結果を表に書き込む作業 */
    int n = use_n_list[use_n_idx];

    double* a = (double*)calloc(n, sizeof(double)); /* nが変わるとaのサイズが変わるので確保しなおす */
    double* a_val = (double*)calloc(n, sizeof(double)); /* ソート結果の検証用にもう一つ */
    if(a==NULL || a_val==NULL){ puts("ERROR [main] cant alloc memory"); exit(0); }

    for(init_type_idx=0;init_type_idx<init_type_count;init_type_idx++){
      for(sample_idx=0;sample_idx<sample_size;sample_idx++){
        init(a,n,init_type_arguments[init_type_idx]); /* sample_idxが変わる度に初期化を行う */
        copy_array(a, n, a_val); quick_sort(a_val, 0, n-1); /* 検証用配列の方を先に信頼できる方法でソートしておく */

        for(sort_idx=0;sort_idx<sort_count;sort_idx++){
          double process_sec;
          printf("measuring{N,sort,init,sample#} : { %-6d | %-14s | %-11s | #%d/%d } ",n,sort_names[sort_idx],init_type_names[init_type_idx],sample_idx+1,sample_size);
          process_sec = measure_sort(sort_algos[sort_idx], a, n, a_val); /* measure_sortは非破壊的な関数で、返り値が実行時間 */
          results[use_n_idx][sort_idx][init_type_idx][sample_idx] = process_sec; /* 表に書きこむ */
          printf("Done! process_sec: %010.7f\n",process_sec);
        }
      }
    }

    free(a);
    free(a_val);
  }
  putchar('\n');

  /*
  text_file = fopen("","w");
  csv_file = fopen("","w");
  */

  print_result(results, text_file, use_n_list, use_n_count, sort_names, sort_count, init_type_names, init_type_count, sample_size); /* 結果の出力 統計処理(中央値)はこちらでやっている */
  print_csv(results, csv_file, use_n_list, use_n_count, sort_names, sort_count, init_type_names, init_type_count, sample_size); /* 結果の出力 統計処理(中央値)はこちらでやっている */

  /*
  fclose(text_file);
  fclose(csv_file);
  */

  free_result_table(use_n_count, sort_count, init_type_count, results);

  return 0;
}

void print_array(double a[], int n, char endwith[]){
  int i;
  putchar('[');
  for(i=0;i<n;i++){

    printf("%f",a[i]);
    if(i<n-1) printf(", ");
  }
  putchar(']');
  printf("%s",endwith);
  return;
}

void copy_array(double src[], int n, double dest[]){
  int i;
  for(i=0;i<n;i++){
    dest[i] = src[i];
  }
  return;
}

int is_ordered(double a[], int n){
  int i;
  for(i=1;i<n;i++){
    if(a[i-1]>a[i]) return 0;
  }
  return 1;
}

int is_equal(double a1[], double a2[], int n){
  int i;
  for(i=0;i<n;i++){
    if(a1[i]!=a2[i]) return 0;
  }
  return 1;
}

double measure_sort(void (*algo)(double a[], int n), double a[], int n, double a_val[]){
  double begin_s, end_s;
  
  double* a_sort = (double*)calloc(n ,sizeof(double));
  if(a_sort==NULL){puts("ERROR [measure_sort] cant alloc memory for measurement."); exit(0); }

  copy_array(a, n, a_sort);

  begin_s = current_sec();
  algo(a_sort, n);
  end_s = current_sec();


  if(a_val && is_equal(a_sort, a_val, n)==0){
    puts("WARNING [measure_sort] results are different, something may be wrong...");
    exit(0);
  }

  free(a_sort);

  return end_s - begin_s;
}

double**** alloc_result_table(int use_n_count, int sort_count, int init_type_count, int sample_size){
  int use_n_idx, sort_idx, init_type_idx;

  double**** table = (double****)calloc(use_n_count, sizeof(double***));
  if(table==NULL){ puts("ERROR [main] cant alloc mem."); exit(0); };
  for(use_n_idx=0;use_n_idx<use_n_count;use_n_idx++){
    table[use_n_idx] = (double***)calloc(sort_count, sizeof(double**));
    if(table[use_n_idx]==NULL){ puts("ERROR [main] cant alloc mem."); exit(0); };
    for(sort_idx=0;sort_idx<sort_count;sort_idx++){
      table[use_n_idx][sort_idx] = (double**)calloc(init_type_count, sizeof(double*));
      if(table[use_n_idx][sort_idx]==NULL){ puts("ERROR [main] cant alloc mem."); exit(0); };
      for(init_type_idx=0;init_type_idx<init_type_count;init_type_idx++){
        table[use_n_idx][sort_idx][init_type_idx] = (double*)calloc(sample_size, sizeof(double));
        if(table[use_n_idx][sort_idx][init_type_idx]==NULL){ puts("ERROR [main] cant alloc mem."); exit(0); };
      }
    }
  }
  return table;
}

void free_result_table(int use_n_count, int sort_count, int init_type_count, double**** table){
  int use_n_idx, sort_idx, init_type_idx;
  for(use_n_idx=0;use_n_idx<use_n_count;use_n_idx++){
    for(sort_idx=0;sort_idx<sort_count;sort_idx++){
      for(init_type_idx=0;init_type_idx<init_type_count;init_type_idx++){
        free(table[use_n_idx][sort_idx][init_type_idx]);
      }
      free(table[use_n_idx][sort_idx]);
    }
    free(table[use_n_idx]);
  }
  free(table);
}

double get_median(double a[], int n){
  double result;

  quick_sort(a, 0, n-1);
  if(n%2==0){
    result = (a[n/2] + a[n/2-1]) / 2.0;
  }else{
    result = a[n/2];
  }
  return result;
}

void print_result(double****result_table, FILE* f, int use_n_list[], int use_n_count, char sort_names[][NAME_MAX_LEN], int sort_count, char init_type_names[][NAME_MAX_LEN], int init_type_count, int sample_size){
  int split_format_len = 2; /* 水平方向の区切りにスペースをいくつ入れるか */
  int n_label_format_len = 6; /* nを示すラベル(表の左端の列)の長さ */
  int col_format_len = 11; /* 結果1列の長さ */
  int acc_format_len = 7; /* 結果の表示の小数点以下の桁数 */
  int sort_label_format_len = col_format_len*init_type_count+split_format_len*(init_type_count-1);
  char split_line = '-'; /* 垂直方向の区切りに使う文字 */

  int use_n_idx, sort_idx, init_type_idx;
  int i;

  /* 上部の見出しを出力 */
  fprintf(f, "%*s",n_label_format_len+split_format_len,"");
  for(sort_idx=0;sort_idx<sort_count;sort_idx++) fprintf(f, "%-*s",sort_label_format_len+split_format_len,sort_names[sort_idx]);
  fputc('\n', f);
  fprintf(f, "%*s",n_label_format_len+split_format_len,"");
  for(sort_idx=0;sort_idx<sort_count;sort_idx++){ for(i=0;i<sort_label_format_len;i++){ fputc(split_line, f); } fprintf(f, "%*s",split_format_len,""); }
  fputc('\n', f);
  fprintf(f, "%-*s",n_label_format_len+split_format_len,"N");
  for(sort_idx=0;sort_idx<sort_count;sort_idx++){ for(init_type_idx=0;init_type_idx<init_type_count;init_type_idx++){ fprintf(f, "%-*s",col_format_len+split_format_len,init_type_names[init_type_idx]); } }
  fputc('\n', f);
  for(i=0;i<n_label_format_len;i++){ fputc(split_line, f); } fprintf(f, "%*s",split_format_len,"");
  for(sort_idx=0;sort_idx<sort_count;sort_idx++){ for(init_type_idx=0;init_type_idx<init_type_count;init_type_idx++){ for(i=0;i<col_format_len;i++){ fputc(split_line, f); } fprintf(f, "%*s",split_format_len,""); } }
  fputc('\n', f);


  /* 結果の部分を出力 */
  for(use_n_idx=0;use_n_idx<use_n_count;use_n_idx++){
    int n;
    n = use_n_list[use_n_idx];
    fprintf(f, "%-*d",n_label_format_len+split_format_len,n); /* 行見出し(nのラベル)を出力 */

    for(sort_idx=0;sort_idx<sort_count;sort_idx++){
      for(init_type_idx=0;init_type_idx<init_type_count;init_type_idx++){
        double median = get_median(result_table[use_n_idx][sort_idx][init_type_idx], sample_size); /* 各ソート、各初期化タイプの、sample_size個の記録の中央値を計算 */
        fprintf(f, "%0*.*f",col_format_len,acc_format_len,median);
        fprintf(f, "%*s",split_format_len,"");
      }
    }
    fputc('\n', f);
  }
}

void print_csv(double****result_table, FILE* f, int use_n_list[], int use_n_count, char sort_names[][NAME_MAX_LEN], int sort_count, char init_type_names[][NAME_MAX_LEN], int init_type_count, int sample_size){
  int acc_format_len = 7; /* 結果の表示の小数点以下の桁数 */
  char split_char = ','; /* 区切りに使う文字 */

  int use_n_idx, sort_idx, init_type_idx;

  /* 上部の見出しを出力 */
  fputc(' ', f);
  for(sort_idx=0;sort_idx<sort_count;sort_idx++) for(init_type_idx=0;init_type_idx<init_type_count;init_type_idx++) fprintf(f, "%c%s",split_char,sort_names[sort_idx]);
  fputc('\n', f);
  fputc('N', f);
  for(sort_idx=0;sort_idx<sort_count;sort_idx++) for(init_type_idx=0;init_type_idx<init_type_count;init_type_idx++) fprintf(f, "%c%s",split_char,init_type_names[init_type_idx]);
  fputc('\n', f);


  /* 結果の部分を出力 */
  for(use_n_idx=0;use_n_idx<use_n_count;use_n_idx++){
    int n;
    n = use_n_list[use_n_idx];
    fprintf(f, "%d",n); /* 行見出し(nのラベル)を出力 */

    for(sort_idx=0;sort_idx<sort_count;sort_idx++){
      for(init_type_idx=0;init_type_idx<init_type_count;init_type_idx++){
        double median = get_median(result_table[use_n_idx][sort_idx][init_type_idx], sample_size); /* 各ソート、各初期化タイプの、sample_size個の記録の中央値を計算 */
        fprintf(f, "%c%.*f",split_char,acc_format_len,median);
      }
    }
    fputc('\n', f);
  }
}


void bsort(double a[], int n){
  int i;
  int sorted;
  int swap_flag;
  double temp;

  for(sorted=0;sorted<n-1;sorted++){ /* 未ソート要素が1になるまで(残った1つは最大値になる)ループ */

    swap_flag = 0;
    for(i=n-1;i>sorted;i--){ /* 配列の最後から未ソート領域をの先頭まで、2要素ずつ比較 */
      if(a[i-1]>a[i]){ /* 必要があれば交換、フラグを立てる */
        swap_flag = 1;
        temp = a[i-1];
        a[i-1] = a[i];
        a[i] = temp;
      }
    }

    if(!swap_flag) return; /* 一度も交換が無ければソート済み */
  }
  return;
}

void ssort(double a[], int n){
  int i;
  int sorted;
  int min_idx;
  double temp;

  for(sorted=0;sorted<n-1;sorted++){ /* 未ソート要素が1になるまで(残った1つは最大値になる)ループ */

    min_idx = sorted; /* 未ソート要素の先頭から最後までで最小値のインデックスを探す */
    for(i=sorted+1;i<n;i++){
      if(a[i] < a[min_idx]) min_idx = i;
    }
    
    if(min_idx!=sorted){ /* 交換の必要があれば */
      temp = a[sorted]; /* 未ソート要素の先頭と最小要素を交換 */
      a[sorted] = a[min_idx];
      a[min_idx] = temp;
    }

  }
  return;
}

void isort(double a[], int n){
  int i;
  int sorted = 0;
  double temp;

  i = 0;
  for(sorted=0;sorted<n;sorted++){ /* 未ソート要素が無くなるまでループ */

    temp = a[sorted]; /* 未ソート要素の先頭を”掴む” */

    for(i=sorted;i>0 && a[i-1]>temp;i--){ /* 挿入位置を探しながら要素を後ろへ移動 */
        a[i] = a[i-1];
    }
    a[i] = temp; /* ループ脱出時点での位置に掴んだ要素を置く */
  }
  return;
}

void myqsort(double a[], int n){
  double pivot;
  int l_idx, r_idx;
  double temp;

  if(n<=1) return; /* 1要素以下ならソートが不必要なので完了 */

  /* 最初,真ん中,最後の3要素のうちの中央値をpivotとする */
  pivot = (a[0] < a[n/2]) ?
          (a[n/2] < a[n-1] ? a[n/2] : (a[0] < a[n-1] ? a[n-1] : a[0]))
        : (a[0] < a[n-1] ? a[0] : (a[n/2] < a[n-1] ? a[n-1] : a[n/2]));


  l_idx = 0; /* l_idxを左端(先頭)へ */
  r_idx = n-1; /* r_idxを右端(末尾)へ */
  while (l_idx <= r_idx) {
    /* 左側からピボット以上の要素を探す */
    while (a[l_idx] < pivot) l_idx++;
    /* 右側からピボット以下の要素を探す */
    while (a[r_idx] > pivot) r_idx--;

    if (l_idx <= r_idx) {
      /* 要素を交換してインデックスを進める */
      temp = a[l_idx];
      a[l_idx] = a[r_idx];
      a[r_idx] = temp;
      l_idx++;
      r_idx--;
    }
  }

    /* 再帰的にクイックソートを適用 */
    myqsort(a, r_idx + 1);       /* 前半部分 */
    myqsort(a + l_idx, n - l_idx); /* 後半部分 */

  return;
}

void _downheap(double a[], int heap_size, int idx){
  int lc_idx, rc_idx, max_idx;
  double temp;

  while((lc_idx=idx*2+1) < heap_size){ /* idxが葉でない限り */
    rc_idx = lc_idx+1;
    max_idx = idx;

    if(a[lc_idx]>a[max_idx]) max_idx = lc_idx; /* 左子が最大かも */
    if(rc_idx<heap_size && a[rc_idx]>a[max_idx]) max_idx = rc_idx; /* 右子が最大かも */
    if(max_idx==idx) break; /* 自分が最大ならdownheap終了 */
    
    temp = a[max_idx]; /* 最大の子と交換 */
    a[max_idx] = a[idx];
    a[idx] = temp;

    idx = max_idx;
  }
  return;
}

void hsort(double a[], int n){
  double temp;
  int i, heap_size;

  for(i=(n-2)/2;i>=0;i--){ /* ヒープの構成 */
    _downheap(a, n, i);
  }

  for(heap_size=n;heap_size>1;heap_size--){ /* ヒープを縮小しながらソート */
    temp = a[0]; /* ヒープ先頭(最大値)を取り出し、最後と交換 */
    a[0] = a[heap_size-1];
    a[heap_size-1] = temp;

    _downheap(a, heap_size-1, 0); /* ヒープサイズを縮小し再ヒープ化 */
  }
  return;
}

void _split_merge(double src[], double dest[], int start_idx, int stop_idx){
  int n;
  int idx, idx1, idx2, mid_idx;

  n = stop_idx - start_idx;

  if(n<=1) return; /* 1要素以下なら分割もマージも不可能 */

  /* 第一引数(仮引数名src)の部分にdestを、第二引数(仮引数名dest)の部分にsrcを入れている。これにより、毎回コピーする手間を省ける */
  mid_idx = start_idx + n/2; /* 分割 */
  _split_merge(dest, src, start_idx, mid_idx); /* 前半部分に再帰的に処理 */
  _split_merge(dest, src, mid_idx, start_idx+n); /* 後半部分に再帰的に処理 */

  idx1 = start_idx; /* マージ */
  idx2 = mid_idx;
  for(idx=start_idx;idx<stop_idx;idx++){
    if(idx1<mid_idx && (idx2>=stop_idx || src[idx1] <= src[idx2])){ /* 前半部分を使い切っていない かつ (後半部分を使い切った 又は 後半先頭より前半先頭が小さい) */
      dest[idx] = src[idx1++]; /* 前半先頭をdestへ */
    }else{
      dest[idx] = src[idx2++]; /* 後半先頭をdestへ */
    }
  }
  return;
}

void msort(double a[], int n){
  int i;
  double* work_space;
  if(n<=1) return; /* 1要素以下ならソートは不要なので完了 */

  work_space = (double*)calloc((size_t)n, sizeof(double)); /* 同じ長さの作業スペースを確保 */
  if(work_space == NULL){ /* nが大きいと領域確保に失敗する(callocがNULLを返す)のでエラー処理 */
    puts("ERROR [msort] failed to allocate memory for workspace.");
    return;
  }
  for(i=0;i<n;i++){ /* 作業スペースに元配列をコピーしておく */
    work_space[i] = a[i];
  }

  _split_merge(work_space, a, 0, n);

  free(work_space); /* 解放 */
  return;
}

#define BYTE_BIT 8 /* 1byteが何bitか */

#ifndef __SIZEOF_DOUBLE__ /* 処理系によってはdouble型(ソート対象)のバイト数__SIZEOF_DOUBLE__が定義されていないのでその場合 */
#define __SIZEOF_DOUBLE__ 8 /* 手動で定義 多分8バイト 多くの処理系では8だからそう たぶんそう きっとそう */
#endif

#define WINDOW_BIT 8 /* データを何bitずつ見るか 処理の都合で1byte以下かつ1byteを割り切れる必要がある */
#define BUCKET_COUNT 256 /* 中で行うバケツ(=バケット)ソートで用意するバケツの数 2のWINDOW_BIT乗個 */

void rsort(double a[], int n){ /* 極めて重要な注意点:負の数未対応 */

  typedef union{ /* double型でビット演算をするために作った共有体 */
    double double_val; /* この8?byte領域と */
    unsigned char byte[__SIZEOF_DOUBLE__]; /* この8?byte領域が共有 */
  } Uni;
  Uni uni;
  int i, j;
  unsigned char bit_mask;
  int byte_idx, bit_idx;
  int window;

  int bucket_contains[BUCKET_COUNT]; /* それぞれのバケツに入っているデータの数 bucket_contains[0]は0番バケツに入っているデータの数 */
  double* buckets[BUCKET_COUNT]; /* バケツをまとめている配列 バケツ自体もdoubleの配列(ポインタ)なので、ポインタの配列となる */
  int bucket_idx;

  bit_mask=(unsigned char)0; /* bit_maskの準備 いったん00000000にしてから */
  for(i=0;i<WINDOW_BIT;i++){ /* 下位側にWINDOW_BIT分だけ1が続いたデータを作る WINDOW_BITが4なら00001111 */
    bit_mask = bit_mask<<1 | (unsigned char)1;
  }

  for(bucket_idx=0;bucket_idx<BUCKET_COUNT;bucket_idx++){ /* バケツの準備 バケツ1つ1つについて */
    double* bucket;
    bucket = calloc(n, sizeof(double)); /* double型が最大n個入るのでこの分だけ領域確保を試みる */

    if(bucket == NULL){ /* nが大きいと領域確保に失敗する(callocがNULLを返す)のでエラー処理 */
      puts("ERROR [rsort] failed to allocate memory for buckets.");
      for(j=0;j<bucket_idx;j++) free(buckets[j]); /* 既に確保していたバケツを解放 */
      return;
    }

    buckets[bucket_idx] = bucket; /* i番目のバケツとして確保した領域の先頭アドレスを保存 */
    bucket_contains[bucket_idx] = 0; /* i番目のバケツには今データが入っていない */
  }

  for(byte_idx=0;byte_idx<__SIZEOF_DOUBLE__;byte_idx++){ /* データのbyte_idxバイト目に注目する byte_idx=0>1>2>...>SIZEOF_DOUBLE */
    for(bit_idx=0;bit_idx<BYTE_BIT;bit_idx+=WINDOW_BIT){ /* 注目した1byteの中でbit_idx番目からWINDOW_BIT個のbit列に注目する (WINDOW_BIT=2の場合)bit_idx=0,2,4,6 */
      /* つまり上2つのforループ内で(8byte=64bitデータでWINDOW_BITが2の場合) 0~1, 2~3, ..., 62~63  (最初は0bit目から1bit目までが、次は2bit目から3bit目までが...最後は62から63bit目までが注目される)*/

      for(i=0;i<n;i++){ /* 全てのデータをバケツに投入する作業 データ1件ずつ */
        uni.double_val = a[i]; /* データをビット演算をするために共有体にdouble型として代入 */
        window = ((uni.byte[byte_idx])>>bit_idx) & bit_mask; /* 共有体のbyte_idx番目のバイトの、(下位から数えて)bit_idx番目からWINDOW_BIT長のbit列を取り出す */
        /* uni.byte[byte_idx] : uni.double_valの領域とuni.byte[0~8?]の領域は共有されているので、uni.double_valに代入したデータのi番目のバイトにはuni.byte[i]でアクセスできる */
        /* 例:[01110110]の4ビット目(最下位ビットを0ビット目と呼ぶので自然に言うと5ビット目)から2ビット分を取り出すと [01'11'0110] の 11 = 0d3 が得られる */

        /* もしWINDOW_BITが2なら、データから取り出したwindowは00,01,10,11 = 10進数なら0,1,2,3のどれかになる */
        buckets[window][bucket_contains[window]++] = uni.double_val; /* window番目のバケツにデータを入れる もちろんbucket_containsもカウント */
      }

      i = 0;
      for(bucket_idx=0;bucket_idx<BUCKET_COUNT;bucket_idx++){ /* データが入ったバケツ達を順番通りにひっくり返してa配列に戻す作業 0番目のバケツから順に */
        for(j=0;j<bucket_contains[bucket_idx];j++){ /* バケツのデータ全部、入った順に取り出してa配列に順に戻す */
          a[i++]=buckets[bucket_idx][j];
        }
        bucket_contains[bucket_idx] = 0; /* ひっくり返して空になったバケツのデータ数はもちろん0 */
      }
    }
  }

  for(bucket_idx=0;bucket_idx<BUCKET_COUNT;bucket_idx++){
    free(buckets[bucket_idx]); /* バケツは動的確保で用意したので解放 */
  }
}

void bgsort(double a[], int n){ /* nによっては現実的な時間で終了しません */
  int i, j;
  double temp;
  int is_ordered;

  is_ordered = 1; /* 昇順かどうかチェック */
  for(i=1;i<n;i++){
    if(a[i-1]>a[i]){
      is_ordered = 0;
      break;
    }
  }

  while(!is_ordered){ /* 昇順じゃない間 */
    for(i=n-1;i>0;i--){ /* Fisher–Yates shuffle でシャッフルする */
      j = (int)(rand() * (i+1.0) / (1.0+RAND_MAX));
      temp = a[i];
      a[i] = a[j];
      a[j] = temp;
    }

    is_ordered = 1; /* 昇順かどうかチェック */
    for(i=1;i<n;i++){
      if(a[i-1]>a[i]){
        is_ordered = 0;
        break;
      }
    }
  }
  return;
}

