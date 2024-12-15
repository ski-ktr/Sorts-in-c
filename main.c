#include <stdio.h>
#include <stdlib.h>

double current_sec(void);
void init(double a[], int n, int type);
void init_m(double a[], int n);

void print_array(double a[], int n, char endwith[]); /* 配列を表示します */

void copy_array(double src[], int n, double dest[]); /* 配列srcの内容を配列destへコピーします dest配列に対して破壊的 */

int is_ordered(double a[], int n); /* 配列が昇順かどうか確認します */

int is_equal(double a1[], double a2[], int n); /* 配列a1とa2の要素が順番含め等しいか確認します */

void bubble_sort(double a[], int n); /* 配列aをバブルソートで昇順にします(破壊的) */

void select_sort(double a[], int n); /* 配列aを選択ソートで昇順にします(破壊的) */

void insert_sort(double a[], int n); /* 配列aを挿入ソートで昇順にします(破壊的) */

void quick_sort(double a[], int n); /* 配列aをクイックソートで昇順にします(破壊的) */

void heap_sort(double a[], int n); /* 配列aをヒープソートで昇順にします(破壊的) */

void merge_sort(double a[], int n); /* 配列aをマージソートで昇順にします(破壊的) */

void radix_sort(double a[], int n); /* 配列aを基数ソートで昇順にします(破壊的) [極めて重要な注意点]:負の数未対応 空間計算量と速度のバランスを取った */

void radix_sort_m(double a[], int n); /* 配列aを基数ソートで昇順にします(破壊的) [備考]:負の数未対応版(ちょっと遅いかも?) 空間計算量と速度のバランスを取った */

void radix_sort_arr(double a[], int n); /* 配列aを基数ソートで昇順にします(破壊的) [極めて重要な注意点]:負の数未対応 空間計算量を犠牲に速度を優先 */

void radix_sort_arr_m(double a[], int n); /* 配列aを基数ソートで昇順にします(破壊的) [備考]:負の数対応版(ちょっと遅いかも?) 空間計算量を犠牲に速度を優先 */

void radix_sort_lds(double a[], int n); /* 配列aを基数ソートで昇順にします(破壊的) [極めて重要な注意点]:負の数未対応 速度を犠牲に空間計算量を抑制 */

void radix_sort_lds_m(double a[], int n); /* 配列aを基数ソートで昇順にします(破壊的) [備考]:負の数対応版(ちょっと遅いかも?) 速度を犠牲に空間計算量を抑制 */

void bogo_sort(double a[], int n); /* 配列aをボゴソートで昇順にします(破壊的) [注意点]:nによっては現実的な時間で終了しません */


int main(void){
  double data[]={0.304,0.716,0.041,0.658,0.541,0.184,0.683,0.383,0.457,0.165};
  int n = 10;

  print_array(data, n, "\n");

  radix_sort(data, n);

  print_array(data, n, "\n");
  return 0;
}


/*
int main(void){
  int n = 1000000;
  double* data1 = calloc(n,sizeof(double));
  double* data2 = calloc(n,sizeof(double));
  init(data1, n, 0);
  copy_array(data1, n, data2);

  double rt1 = current_sec();
  radix_sort_arr_m(data1, n);
  double rt2 = current_sec();
  double qt1 = current_sec();
  quick_sort(data2, n);
  double qt2 = current_sec();

  double rt = rt2-rt1;
  double qt = qt2-qt1;
  printf("radix process time:\n%lf\n",rt);
  printf("quick process time:\n%lf\n",qt);
  if(is_equal(data1,data2,n)){
    puts("2 methods output same allay");
  }else{
    puts("ERROR : 2 methods output different allay");
  }
  return 0;
}
*/

void print_array(double a[], int n, char endwith[]){
  int i;
  putchar('[');
  for(i=0;i<n;i++){

    printf("%lf",a[i]);
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

void bubble_sort(double a[], int n){
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

void select_sort(double a[], int n){
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

void insert_sort(double a[], int n){
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

/* 実装上の工夫:「pivotを乱択や最初の要素とするのではなく、最初真ん中最後の3値の中央値としたこと」「pivot未満/pivot以上と分けるのではなく、pivot以下/pivot以上とすることで、均一なデータなどが来た際にも大体中央で分けられる、分けた両配列が必ず1以上の長さになるので無限ループを防げるなど」 */
void quick_sort_old(double a[], int n){
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
  while(1){

    if(a[l_idx]>=pivot && a[r_idx]<=pivot){ /* 左側にpivot以上、右側にpivot以下の要素があるなら */
      if(a[l_idx]!=a[r_idx]){  /* どちらもpivotと等しい場合を除き */
        temp = a[l_idx]; /* 2つの要素を交換する */
        a[l_idx] = a[r_idx];
        a[r_idx] = temp;
      }
      l_idx++; /* l_idxを1つ右へ、r_idxを1つ左へ */
      r_idx--;
    }

    if(a[l_idx]<pivot) l_idx++; /* pivot以上の要素でなければl_idxを1つ右へ */
    if(a[r_idx]>pivot) r_idx--; /* pivot以下の要素でなければr_idxを1つ左へ */

    if(l_idx>=r_idx) break; /* l_idxとr_idxが重なるかすれ違ったらループ脱出 */
  }

  if(l_idx==r_idx){ /* l_idxとr_idxが重なっている場合 */
    if(a[l_idx]>=pivot) r_idx--; /* 重なった要素がpivot以上ならr_idxを1つ左へ */
    else l_idx++; /* そうでなければl_idxを1つ右へ */
  }
  /* この時点で[l_idxより前の全要素はpivot以下]かつ[r_idxより後の全要素はpivot以上] */

  quick_sort(&a[0], l_idx); /* 前半部分に再帰的にクイックソート */
  quick_sort(&a[l_idx], n-l_idx); /* 後半部分に再帰的にクイックソート */

  return;
}

void quick_sort(double a[], int n){
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
    // 左側からピボット以上の要素を探す
    while (a[l_idx] < pivot) l_idx++;
    // 右側からピボット以下の要素を探す
    while (a[r_idx] > pivot) r_idx--;

    if (l_idx <= r_idx) {
      // 要素を交換してインデックスを進める
      temp = a[l_idx];
      a[l_idx] = a[r_idx];
      a[r_idx] = temp;
      l_idx++;
      r_idx--;
    }
  }

    // 再帰的にクイックソートを適用
    quick_sort(a, r_idx + 1);       // 前半部分
    quick_sort(a + l_idx, n - l_idx); // 後半部分

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

void heap_sort(double a[], int n){
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

void heap_sort_old(double a[], int n){
  int heap_size;
  double temp;
  int idx;
  int p_idx; /* = (idx-1)/2 */
  int lc_idx; /* = idx*2+1 */
  int rc_idx; /* = idx*2+2 */

  for(heap_size=1;heap_size<n;heap_size++){ /* 配列全体がヒープになるまでヒープを拡大する */
    idx = heap_size; /* ヒープ化されていない領域の先頭要素に注目 */

    /* idx要素についてupheap(pushup)操作 */
    while(idx>0){ /* idxが根でない限り */
      p_idx = (idx-1)/2;
      if(a[idx]>a[p_idx]){ /* idx要素が親要素よりも大きいなら */
        temp = a[idx]; /* 親要素と交換 */
        a[idx] = a[p_idx];
        a[p_idx] = temp;
        idx = p_idx; /* 次のループでは今の親要素に注目する */
      }else{ /* そうでなければ */
        break;
      }
    }
  }

  do{ /* ヒープの大きさが1になるまで最大値を取り出しヒープを縮小する */
    temp = a[0]; /* ヒープの根(ヒープ内の最大値)とヒープ化した領域の最後の要素を交換 */
    a[0] = a[heap_size-1];
    a[heap_size-1] = temp;
    heap_size--; /* ヒープサイズを縮小 */

    idx = 0; /* ヒープの根に注目 */
    lc_idx = idx*2+1;
    rc_idx = idx*2+2;

    /* idx要素に対してdownheap(pushdown)操作 */
    while(lc_idx < heap_size){ /* idxが葉でない限り */
      if(rc_idx < heap_size){ /* 子が2つで */
        if(a[rc_idx]<=a[lc_idx] && a[lc_idx] > a[idx]){ /* 左子が右子やidx要素より大きい場合 */
          temp = a[lc_idx]; /* 左子とidx要素を交換する */
          a[lc_idx] = a[idx];
          a[idx] = temp;
          idx = lc_idx; /* 次のループでは今の左子に注目する */
          lc_idx = idx*2+1;
          rc_idx = idx*2+2;
        }
        else if(a[rc_idx]>a[lc_idx] && a[rc_idx] > a[idx]){ /* 右子が左子やidx要素より大きい場合 */
          temp = a[rc_idx]; /* 右子とidx要素を交換する */
          a[rc_idx] = a[idx];
          a[idx] = temp;
          idx = rc_idx; /* 次のループでは今の右子に注目する */
          lc_idx = idx*2+1;
          rc_idx = idx*2+2;
        }
        else{ /* 交換が無かった場合終了する */
          break;
        }
      }else{ /* 子が1つなら */
        if(a[lc_idx] > a[idx]){ /* 左子がidx要素より大きい場合 */
          temp = a[lc_idx]; /* 左子とidx要素を交換する */
          a[lc_idx] = a[idx];
          a[idx] = temp;
        }
        break; /* この左子は子を持たないので終了 */
      }
    }

  }while(heap_size>1);
}

/* 実装上の工夫:「配列が2つあるが、作業に使う配列を再帰の度に入れ替えることで、データのコピー等の手間を最初のみに抑えていること」 */
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
    if(idx1<mid_idx && (idx2>=stop_idx || src[idx1]<= src[idx2])){ /* 前半部分を使い切っていない かつ (後半部分を使い切った 又は 後半先頭より前半先頭が小さい) */
      dest[idx] = src[idx1++]; /* 前半先頭をdestへ */
    }else{
      dest[idx] = src[idx2++]; /* 後半先頭をdestへ */
    }
  }
  return;
}

void merge_sort(double a[], int n){
  int i;
  double* work_space;
  if(n<=1) return; /* 1要素以下ならソートは不要なので完了 */

  work_space = (double*)calloc((size_t)n, sizeof(double)); /* 同じ長さの作業スペースを確保 */
  if(work_space == NULL){ /* nが大きいと領域確保に失敗する(callocがNULLを返す)のでエラー処理 */
    puts("ERROR [merge_sort] failed to allocate memory for workspace.");
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

void radix_sort_arr(double a[], int n){ /* 極めて重要な注意点:負の数未対応 */

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
      puts("ERROR [radix_sort] failed to allocate memory for buckets.");
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

void radix_sort_arr_m(double a[], int n){ /*負の数対応版 分岐が増えているので遅い可能性あり */

  typedef union{ /* double型でビット演算をするために作った共有体 */
    double double_val; /* この8?byte領域と */
    unsigned char byte[__SIZEOF_DOUBLE__]; /* この8?byte領域が共有 */
  } Uni;
  Uni uni;
  int i, j;
  unsigned char bit_mask, bit_mask_nosign;
  int byte_idx, bit_idx;
  int window;

  int bucket_contains[BUCKET_COUNT]; /* それぞれのバケツに入っている正のデータの数 bucket_contains[0]は0番バケツに入っているデータの数 */
  int bucket_conteins_m[BUCKET_COUNT]; /* それぞれのバケツに入っている負のデータの数 */
  double* buckets[BUCKET_COUNT]; /* バケツをまとめている配列 バケツ自体もdoubleの配列(ポインタ)なので、ポインタの配列となる */
  int bucket_idx;

  bit_mask=(unsigned char)0; /* bit_maskの準備 いったん00000000にしてから */
  for(i=0;i<WINDOW_BIT;i++){ /* 下位側にWINDOW_BIT分だけ1が続いたデータを作る WINDOW_BITが4なら00001111 */
    bit_mask = bit_mask<<1 | (unsigned char)1;
  }
  bit_mask_nosign = bit_mask ^ (unsigned char)1; /* bit_mask_nosignは最下位だけ0にする WINDOW_BITが4なら00001110 */

  for(bucket_idx=0;bucket_idx<BUCKET_COUNT;bucket_idx++){ /* バケツの準備 バケツ1つ1つについて */
    double* bucket;
    bucket = calloc(n, sizeof(double)); /* double型が最大n個入るのでこの分だけ領域確保を試みる */

    if(bucket == NULL){ /* nが大きいと領域確保に失敗する(callocがNULLを返す)のでエラー処理 */
      puts("ERROR [radix_sort_m] failed to allocate memory for buckets.");
      for(j=0;j<bucket_idx;j++) free(buckets[j]); /* 既に確保していたバケツを解放 */
      return;
    }

    buckets[bucket_idx] = bucket; /* i番目のバケツとして確保した領域の先頭アドレスを保存 */
    bucket_contains[bucket_idx] = 0; /* i番目のバケツには今正のデータが入っていない */
    bucket_conteins_m[bucket_idx] = 0; /* もちろん負のデータも入ってない*/
  }


  for(byte_idx=0;byte_idx<__SIZEOF_DOUBLE__;byte_idx++){ /* データのbyte_idxバイト目に注目する byte_idx=0>1>2>...>SIZEOF_DOUBLE */
    for(bit_idx=0;bit_idx<BYTE_BIT;bit_idx+=WINDOW_BIT){ /* 注目した1byteの中でbit_idx番目からWINDOW_BIT個のbit列に注目する (WINDOW_BIT=2の場合)bit_idx=0,2,4,6 */
      /* つまり上2つのforループ内で(8byte=64bitデータでWINDOW_BITが2の場合) 0~1, 2~3, ..., 62~63  (最初は0bit目から1bit目までが、次は2bit目から3bit目までが...最後は62から63bit目までが注目される) */
      if(byte_idx==__SIZEOF_DOUBLE__-1 && bit_idx==BYTE_BIT-WINDOW_BIT) bit_mask = bit_mask_nosign; /* ループの最後だけ(注目するビット列に符号ビットが含まれる)bit_maskで符号ビットを見ないようにする */

      for(i=0;i<n;i++){ /* 全てのデータをバケツに投入する作業 データ1件ずつ */
        uni.double_val = a[i]; /* データをビット演算をするために共有体にdouble型として代入 */
        window = ((uni.byte[byte_idx])>>bit_idx) & bit_mask; /* 共有体のbyte_idx番目のバイトの、(下位から数えて)bit_idx番目からWINDOW_BIT長のbit列を取り出す */
        /* uni.byte[byte_idx] : uni.double_valの領域とuni.byte[0~8?]の領域は共有されているので、uni.double_valに代入したデータのi番目のバイトにはuni.byte[i]でアクセスできる */
        /* 例:[01110110]の4ビット目(0が最下位ビットなので自然に言うと5ビット目)から2ビット分を取り出すと [01'11'0110] の 11 = 0d3 が得られる */

        /* もしWINDOW_BITが2なら、データから取り出したwindowは00,01,10,11 = 10進数なら0,1,2,3のどれかになる */
        if(uni.double_val>=0){ /* 正負で処理を分ける */
          buckets[window][bucket_contains[window]++] = uni.double_val; /* window番目のバケツに正のデータを入れる もちろんbucket_containsもカウント */
        }else{
          window = BUCKET_COUNT-window-1; /* データが負の場合、windowを、元の値が大きい程小さくなるように更新 */
          buckets[window][n-1-(bucket_conteins_m[window]++)] = uni.double_val; /* バケツ領域を後ろから使う事で負のデータも入れる */
        }
      }

      i = 0;
      for(bucket_idx=0;bucket_idx<BUCKET_COUNT;bucket_idx++){ /* バケツ達の負のデータを順番通りa配列に戻す作業 0番目のバケツから順に */
        for(j=0;j<bucket_conteins_m[bucket_idx];j++){ /* バケツの負のデータ全部、入った順に取り出してa配列に順に戻す */
          a[i++]=buckets[bucket_idx][n-1-j];
        }
        bucket_conteins_m[bucket_idx] = 0; /* 負のデータを取り出しきったバケツの負のデータ数はもちろん0 */
      }
      for(bucket_idx=0;bucket_idx<BUCKET_COUNT;bucket_idx++){ /* バケツ達の正のデータを順番通りa配列に戻す作業 0番目のバケツから順に */
        for(j=0;j<bucket_contains[bucket_idx];j++){ /* バケツの正のデータ全部、入った順に取り出してa配列に順に戻す */
          a[i++]=buckets[bucket_idx][j];
        }
        bucket_contains[bucket_idx] = 0; /* 正のデータを取り出しきったバケツの正のデータ数はもちろん0 */
      }
    }
  }

  for(bucket_idx=0;bucket_idx<BUCKET_COUNT;bucket_idx++){
    free(buckets[bucket_idx]); /* バケツは動的確保で用意したので解放 */
  }
}

#define INITIAL_BUCKET_SIZE n/BUCKET_COUNT
#define GROWTH_FACTOR 32 /* バケツサイズが足りなくなった場合の増加係数 1以上でなくてはならない(1の場合、前のサイズよりも1つだけ大きいサイズが割り当てられる) */
void radix_sort(double a[], int n){ /* 極めて重要な注意点:負の数未対応 */

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
  int bucket_alloc_sizes[BUCKET_COUNT]; /* それぞれのバケツに対して確保されている長さ */
  double* buckets[BUCKET_COUNT]; /* バケツをまとめている配列 バケツ自体もdoubleの配列(ポインタ)なので、ポインタの配列となる */
  int bucket_idx;
  int initial_bucket_size;

  bit_mask=(unsigned char)0; /* bit_maskの準備 いったん00000000にしてから */
  for(i=0;i<WINDOW_BIT;i++){ /* 下位側にWINDOW_BIT分だけ1が続いたデータを作る WINDOW_BITが4なら00001111 */
    bit_mask = bit_mask<<1 | (unsigned char)1;
  }

  initial_bucket_size = (int)INITIAL_BUCKET_SIZE;
  for(bucket_idx=0;bucket_idx<BUCKET_COUNT;bucket_idx++){ /* バケツの準備 バケツ1つ1つについて */
    double* bucket = NULL;

    if(initial_bucket_size){
      bucket = calloc(initial_bucket_size, sizeof(double)); /* initial_bucket_sizeだけ領域確保を試みる */
      if(bucket == NULL){ /* nが大きいと領域確保に失敗する(callocがNULLを返す)のでエラー処理 */
        puts("ERROR [radix_sort] failed to allocate memory for buckets.");
        for(j=0;j<bucket_idx;j++) free(buckets[j]); /* 既に確保していたバケツを解放 */
        return;
      }
    }

    buckets[bucket_idx] = bucket; /* i番目のバケツとして確保した領域の先頭アドレスを保存 */
    bucket_alloc_sizes[bucket_idx] = initial_bucket_size; /* i番目のバケツにはinital_bucket_sizeが割り当てられている */
    bucket_contains[bucket_idx] = 0; /* i番目のバケツには今データが入っていない */
  }

  for(byte_idx=0;byte_idx<__SIZEOF_DOUBLE__;byte_idx++){ /* データのbyte_idxバイト目に注目する byte_idx=0>1>2>...>SIZEOF_DOUBLE */
    for(bit_idx=0;bit_idx<BYTE_BIT;bit_idx+=WINDOW_BIT){ /* 注目した1byteの中でbit_idx番目からWINDOW_BIT個のbit列に注目する (WINDOW_BIT=2の場合)bit_idx=0,2,4,6 */
      /* つまり上2つのforループ内で(8byte=64bitデータでWINDOW_BITが2の場合) 0~1, 2~3, ..., 62~63  (最初は0bit目から1bit目までが、次は2bit目から3bit目までが...最後は62から63bit目までが注目される)*/

      for(i=0;i<n;i++){ /* 全てのデータをバケツに投入する作業 データ1件ずつ */
        uni.double_val = a[i]; /* データをビット演算をするために共有体にdouble型として代入 */
        window = ((uni.byte[byte_idx])>>bit_idx) & bit_mask; /* 共有体のbyte_idx番目のバイトの、(下位から数えて)bit_idx番目からWINDOW_BIT長のbit列を取り出す */
        /* window番目のバケツにデータを入れる */
        if(bucket_alloc_sizes[window]<=bucket_contains[window]){ /* まず、window番目のバケツの容量が足り無ければ割り当てを増やす */
          int next_size;
          double* new_bucket;
          next_size = bucket_alloc_sizes[window]*GROWTH_FACTOR+1;
          if(next_size > n) next_size = n;

          new_bucket = realloc(buckets[window], next_size*sizeof(double));

          if(new_bucket==NULL){ /* 領域確保に失敗したら(reallocがNULLを返したら)エラー処理 */
            puts("ERROR [radix_sort] failed to reallocate memory for buckets.");
            for(i=0;i<BUCKET_COUNT;i++) if(buckets[i]) free(buckets[i]); /* 既に確保していたバケツを解放 */
            return;
          }

          buckets[window] = new_bucket;
          bucket_alloc_sizes[window] = next_size;
        }
        buckets[window][bucket_contains[window]++] = uni.double_val;
      }

      i = 0;
      for(bucket_idx=0;bucket_idx<BUCKET_COUNT;bucket_idx++){ /* データが入ったバケツ達を順番通りにひっくり返してa配列に戻す作業 0番目のバケツから順に */
        for(j=0;j<bucket_contains[bucket_idx];j++){ /* バケツのデータ全部、入った順に取り出してa配列に順に戻す */
          a[i++]=buckets[bucket_idx][j];
        }
        bucket_contains[bucket_idx] = 0; /* ひっくり返して空になったバケツのデータ数はもちろん0 */

        if(initial_bucket_size){ /* 使い終わったバケツは解放 */
          buckets[bucket_idx] = realloc(buckets[bucket_idx], initial_bucket_size*sizeof(double));
          bucket_alloc_sizes[bucket_idx] = initial_bucket_size;
        }else{
          if(buckets[bucket_idx]) free(buckets[bucket_idx]);
          buckets[bucket_idx] = NULL;
          bucket_alloc_sizes[bucket_idx] = 0;
        }
      }
    }
  }

  for(bucket_idx=0;bucket_idx<BUCKET_COUNT;bucket_idx++){
    if(buckets[bucket_idx]) free(buckets[bucket_idx]); /* バケツは動的確保で用意したので解放 */
  }
}

void radix_sort_m(double a[], int n){ /* 負の数対応版 分岐が増えているので遅い可能性あり */

  typedef union{ /* double型でビット演算をするために作った共有体 */
    double double_val; /* この8?byte領域と */
    unsigned char byte[__SIZEOF_DOUBLE__]; /* この8?byte領域が共有 */
  } Uni;
  Uni uni;
  int i, j;
  unsigned char bit_mask, bit_mask_nosign;
  int byte_idx, bit_idx;
  int window;

  /* 負の数に対応するため、バケツ自体を増やして対応する */
  int bucket_contains[BUCKET_COUNT*2]; /* それぞれのバケツに入っているデータの数 bucket_contains[0]は0番バケツに入っているデータの数 */
  int bucket_alloc_sizes[BUCKET_COUNT*2]; /* それぞれのバケツに対して確保されている長さ */
  double* buckets[BUCKET_COUNT*2]; /* バケツをまとめている配列 バケツ自体もdoubleの配列(ポインタ)なので、ポインタの配列となる */
  int bucket_idx;
  int initial_bucket_size;

  bit_mask=(unsigned char)0; /* bit_maskの準備 いったん00000000にしてから */
  for(i=0;i<WINDOW_BIT;i++){ /* 下位側にWINDOW_BIT分だけ1が続いたデータを作る WINDOW_BITが4なら00001111 */
    bit_mask = bit_mask<<1 | (unsigned char)1;
  }
  bit_mask_nosign = bit_mask ^ (unsigned char)1; /* bit_mask_nosignは最下位だけ0にする WINDOW_BITが4なら00001110 */

  initial_bucket_size = (int)INITIAL_BUCKET_SIZE;
  for(bucket_idx=0;bucket_idx<BUCKET_COUNT*2;bucket_idx++){ /* バケツの準備 バケツ1つ1つについて */
    double* bucket = NULL;

    if(initial_bucket_size){
      bucket = calloc(initial_bucket_size, sizeof(double)); /* initial_bucket_sizeだけ領域確保を試みる */
      if(bucket == NULL){ /* nが大きいと領域確保に失敗する(callocがNULLを返す)のでエラー処理 */
        puts("ERROR [radix_sort] failed to allocate memory for buckets.");
        for(j=0;j<bucket_idx;j++) free(buckets[j]); /* 既に確保していたバケツを解放 */
        return;
      }
    }

    buckets[bucket_idx] = bucket; /* i番目のバケツとして確保した領域の先頭アドレスを保存 */
    bucket_alloc_sizes[bucket_idx] = initial_bucket_size; /* i番目のバケツにはinital_bucket_sizeが割り当てられている */
    bucket_contains[bucket_idx] = 0; /* i番目のバケツには今データが入っていない */
  }

  for(byte_idx=0;byte_idx<__SIZEOF_DOUBLE__;byte_idx++){ /* データのbyte_idxバイト目に注目する byte_idx=0>1>2>...>SIZEOF_DOUBLE */
    for(bit_idx=0;bit_idx<BYTE_BIT;bit_idx+=WINDOW_BIT){ /* 注目した1byteの中でbit_idx番目からWINDOW_BIT個のbit列に注目する (WINDOW_BIT=2の場合)bit_idx=0,2,4,6 */
      /* つまり上2つのforループ内で(8byte=64bitデータでWINDOW_BITが2の場合) 0~1, 2~3, ..., 62~63  (最初は0bit目から1bit目までが、次は2bit目から3bit目までが...最後は62から63bit目までが注目される)*/

      if(byte_idx==__SIZEOF_DOUBLE__-1 && bit_idx==BYTE_BIT-WINDOW_BIT) bit_mask = bit_mask_nosign; /* ループの最後だけ(注目するビット列に符号ビットが含まれる)bit_maskで符号ビットを見ないようにする */

      for(i=0;i<n;i++){ /* 全てのデータをバケツに投入する作業 データ1件ずつ */
        uni.double_val = a[i]; /* データをビット演算をするために共有体にdouble型として代入 */
        window = ((uni.byte[byte_idx])>>bit_idx) & bit_mask; /* 共有体のbyte_idx番目のバイトの、(下位から数えて)bit_idx番目からWINDOW_BIT長のbit列を取り出す */

        /* window番目のバケツにデータを入れる */
        if(uni.double_val>=0){ /* 正負で処理を分ける */
          window += BUCKET_COUNT; /* データが0または正の場合、windowをBUCKET_COUNT分加算する BUCKET_COUNTが4であれば、windowは 0->4, 1->5, 2->6, 3->7 となる */
        }else{
          window = BUCKET_COUNT-window-1; /* データが負の場合、windowが大きい程小さくなるよう更新する BUCKET_COUNTが4であれば、windowは 0->3, 1->2, 2->1, 3->0 となる */
        }
        if(bucket_alloc_sizes[window]<=bucket_contains[window]){ /* まず、window番目のバケツの容量が足り無ければ割り当てを増やす */
          int next_size;
          double* new_bucket;
          next_size = bucket_alloc_sizes[window]*GROWTH_FACTOR+1;
          if(next_size > n) next_size = n;

          new_bucket = realloc(buckets[window], next_size*sizeof(double));

          if(new_bucket==NULL){ /* 領域確保に失敗したら(reallocがNULLを返したら)エラー処理 */
            puts("ERROR [radix_sort] failed to reallocate memory for buckets.");
            for(i=0;i<BUCKET_COUNT*2;i++) if(buckets[i]) free(buckets[i]); /* 既に確保していたバケツを解放 */
            return;
          }

          buckets[window] = new_bucket;
          bucket_alloc_sizes[window] = next_size;
        }
        buckets[window][bucket_contains[window]++] = uni.double_val;
      }

      i = 0;
      for(bucket_idx=0;bucket_idx<BUCKET_COUNT*2;bucket_idx++){ /* データが入ったバケツ達を順番通りにひっくり返してa配列に戻す作業 0番目のバケツから順に */
        for(j=0;j<bucket_contains[bucket_idx];j++){ /* バケツのデータ全部、入った順に取り出してa配列に順に戻す */
          a[i++]=buckets[bucket_idx][j];
        }
        bucket_contains[bucket_idx] = 0; /* ひっくり返して空になったバケツのデータ数はもちろん0 */

        if(initial_bucket_size){ /* 使い終わったバケツは解放 */
          buckets[bucket_idx] = realloc(buckets[bucket_idx], initial_bucket_size*sizeof(double));
          bucket_alloc_sizes[bucket_idx] = initial_bucket_size;
        }else{
          if(buckets[bucket_idx]) free(buckets[bucket_idx]);
          buckets[bucket_idx] = NULL;
          bucket_alloc_sizes[bucket_idx] = 0;
        }
      }
    }
  }

  for(bucket_idx=0;bucket_idx<BUCKET_COUNT;bucket_idx++){
    if(buckets[bucket_idx]) free(buckets[bucket_idx]); /* バケツは動的確保で用意したので解放 */
  }
}


/* 以下は連結データ構造(Linked Data Structure)ver メモリに優しいが、処理が増えるので遅い可能性あり */
void radix_sort_lds(double a[], int n){ /* 極めて重要な注意点:負の数未対応 */
  typedef struct Bucket_cell_st{ /* バケツの中の1データを示す構造体 */
    double double_val;
    struct Bucket_cell_st* next;
  }Bucket_cell;
  
  typedef union{ /* double型でビット演算をするために作った共有体 */
    double double_val; /* この8?byte領域と */
    unsigned char byte[__SIZEOF_DOUBLE__]; /* この8?byte領域が共有 */
  } Uni;

  Bucket_cell* cell_ptr;
  Uni uni;
  int i, j;
  unsigned char bit_mask;
  int byte_idx, bit_idx;
  int window;
  
  Bucket_cell* buckets[BUCKET_COUNT]; /* バケツをまとめる配列 バケツがBucket_cellの連結リストなのでリストの先頭cellへのポインタ(Bucket_cell*)の配列(ダブルポインタ)となる */
  Bucket_cell* bucket_last_cells[BUCKET_COUNT]; /* 各バケツの末尾セルへのポインタをまとめる配列(ダブルポインタ) */
  int bucket_idx;

  Bucket_cell** no_used_cells; /* メモリ上に確保した使用されていないBucket_cellの配列 セルへのポインタの配列(ダブルポインタ) */
  int no_used_cells_count; /* 確保済みのBucket_cellのうち、使用されていないものの数 */


  bit_mask=(unsigned char)0; /* bit_maskの準備 いったん00000000にしてから */
  for(i=0;i<WINDOW_BIT;i++){ /* 下位側にWINDOW_BIT分だけ1が続いたデータを作る WINDOW_BITが4なら00001111 */
    bit_mask = bit_mask<<1 | (unsigned char)1;
  }

  no_used_cells = calloc(n, sizeof(Bucket_cell*)); /* セルは最大n個なので長さn */
  if(no_used_cells==NULL){
    puts("ERROR [radix_sort_lds] failed to allocate memory for cell_array.");
    return;
  }
  for(no_used_cells_count=0;no_used_cells_count<n;no_used_cells_count++){ /* セルの準備 セルは最大でn個なので前もって確保しておく */
    cell_ptr = calloc(1, sizeof(Bucket_cell));
    if(cell_ptr==NULL){ /* nが大きいと領域確保に失敗する(callocがNULLを返す)のでエラー処理 */
      puts("ERROR [radix_sort_lds_m] failed to allocate memory for bucket_cell.");
      for(j=0;j<no_used_cells_count;i++) free(no_used_cells[j]); /* 確保したセルを解放 */
      free(no_used_cells); /* 動的確保した配列も解放 */
      return;
    }
    no_used_cells[no_used_cells_count] = cell_ptr; /* 確保したセルは配列へ入れておく */
  }

  for(bucket_idx=0;bucket_idx<BUCKET_COUNT;bucket_idx++){ /* バケツの準備 バケツ1つ1つについて */
    buckets[bucket_idx] = NULL;
    bucket_last_cells[bucket_idx] = NULL;
  }

  for(byte_idx=0;byte_idx<__SIZEOF_DOUBLE__;byte_idx++){ /* データのbyte_idxバイト目に注目する byte_idx=0>1>2>...>SIZEOF_DOUBLE */
    for(bit_idx=0;bit_idx<BYTE_BIT;bit_idx+=WINDOW_BIT){ /* 注目した1byteの中でbit_idx番目からWINDOW_BIT個のbit列に注目する (WINDOW_BIT=2の場合)bit_idx=0,2,4,6 */
      /* つまり上2つのforループ内で(8byte=64bitデータでWINDOW_BITが2の場合) 0~1, 2~3, ..., 62~63  (最初は0bit目から1bit目までが、次は2bit目から3bit目までが...最後は62から63bit目までが注目される)*/

      for(i=0;i<n;i++){ /* 全てのデータをバケツに投入する作業 データ1件ずつ */
        uni.double_val = a[i]; /* データをビット演算をするために共有体にdouble型として代入 */
        window = ((uni.byte[byte_idx])>>bit_idx) & bit_mask; /* 共有体のbyte_idx番目のバイトの、(下位から数えて)bit_idx番目からWINDOW_BIT長のbit列を取り出す */
        /* uni.byte[byte_idx] : uni.double_valの領域とuni.byte[0~8?]の領域は共有されているので、uni.double_valに代入したデータのi番目のバイトにはuni.byte[i]でアクセスできる */
        /* 例:[01110110]の4ビット目(0が最下位ビットなので自然に言うと5ビット目)から2ビット分を取り出すと [01'11'0110] の 11 = 0d3 が得られる */

        /* もしWINDOW_BITが2なら、データから取り出したwindowは00,01,10,11 = 10進数なら0,1,2,3のどれかになる */

        /* window番目のバケツの”末尾”にデータを入れる */
        cell_ptr = no_used_cells[--no_used_cells_count]; /* バケツの長さを伸ばすために未使用セルが集まった配列の最後のセルを取ってくる */
        cell_ptr->double_val = uni.double_val; /* セルに値をセット */
        cell_ptr->next = NULL;
        if(!buckets[window]){ /* window番目のバケツにデータがない場合(リストの先頭セルへのアドレスが入るbuckets[window]がNULLの場合) */
          buckets[window] = cell_ptr; /* 先頭セルとして用意したセルを投入 */
          bucket_last_cells[window] = cell_ptr; /* もちろん投入したセルが最後のセル */
        }else{ /* window番目のバケツにデータがある場合(リストの先頭セルへのアドレスが入るbuckets[window]がNULLでない場合) */
          bucket_last_cells[window]->next = cell_ptr; /* 末尾にデータを追加したいので、最後のセルの次のセルとして用意したセルをリンク */
          bucket_last_cells[window] = cell_ptr; /* もちろん投入したセルが最後のセル */
        }
      }


      i = 0;
      for(bucket_idx=0;bucket_idx<BUCKET_COUNT;bucket_idx++){ /* データが入ったバケツ達を順番通りにひっくり返してa配列に戻す作業 0番目のバケツから順に */
        
        cell_ptr = buckets[bucket_idx]; /* リストの先頭セルへのアドレスに注目して */
        while(cell_ptr){ /* 注目しているアドレスがNULLになるまで */
          a[i++] = cell_ptr->double_val; /* 注目したセルの数値をa配列に戻す */
          no_used_cells[no_used_cells_count++] = cell_ptr; /* 注目したセルはもう使わないので未使用セルの配列の末尾にセット */
          cell_ptr = cell_ptr->next; /* 注目するセルを今のセルからリンクされている次のセルに移す */
        }
        buckets[bucket_idx] = NULL; /* ひっくり返して空になったバケツはもちろん空 */
        bucket_last_cells[bucket_idx] = NULL;
      }

    }
  }

  for(i=0;i<n;i++) free(no_used_cells[i]); /* 確保したセルを解放 */
  free(no_used_cells); /* 使用されていないセルを置いておくための配列も動的確保なので解放 */
}

void radix_sort_lds_m(double a[], int n){ /* 負の数対応版 分岐が増えているので遅い可能性あり */
  typedef struct Bucket_cell_st{ /* バケツの中の1データを示す構造体 */
    double double_val;
    struct Bucket_cell_st* next;
  }Bucket_cell;
  
  typedef union{ /* double型でビット演算をするために作った共有体 */
    double double_val; /* この8?byte領域と */
    unsigned char byte[__SIZEOF_DOUBLE__]; /* この8?byte領域が共有 */
  } Uni;

  Bucket_cell* cell_ptr;
  Uni uni;
  int i, j;
  unsigned char bit_mask, bit_mask_nosign;
  int byte_idx, bit_idx;
  int window;
  
  /* 負の数に対応するため、バケツ自体を増やして対応する */
  Bucket_cell* buckets[BUCKET_COUNT*2]; /* バケツをまとめる配列 バケツがBucket_cellの連結リストなのでリストの先頭cellへのポインタ(Bucket_cell*)の配列(ダブルポインタ)となる */
  Bucket_cell* bucket_last_cells[BUCKET_COUNT*2]; /* 各バケツの末尾セルへのポインタをまとめる配列(ダブルポインタ) */
  int bucket_idx;

  Bucket_cell** no_used_cells; /* メモリ上に確保した使用されていないBucket_cellの配列 セルへのポインタの配列(ダブルポインタ) */
  int no_used_cells_count; /* 確保済みのBucket_cellのうち、使用されていないものの数 */


  bit_mask=(unsigned char)0; /* bit_maskの準備 いったん00000000にしてから */
  for(i=0;i<WINDOW_BIT;i++){ /* 下位側にWINDOW_BIT分だけ1が続いたデータを作る WINDOW_BITが4なら00001111 */
    bit_mask = bit_mask<<1 | (unsigned char)1;
  }
  bit_mask_nosign = bit_mask ^ (unsigned char)1; /* bit_mask_nosignは最下位だけ0にする WINDOW_BITが4なら00001110 */

  no_used_cells = calloc(n, sizeof(Bucket_cell*)); /* セルは最大n個なので長さn */
  if(no_used_cells==NULL){
    puts("ERROR [radix_sort_lds_m] failed to allocate memory for cell_array.");
    return;
  }
  for(no_used_cells_count=0;no_used_cells_count<n;no_used_cells_count++){ /* セルの準備 セルは最大でn個なので前もって確保しておく */
    cell_ptr = calloc(1, sizeof(Bucket_cell));
    if(cell_ptr==NULL){ /* nが大きいと領域確保に失敗する(callocがNULLを返す)のでエラー処理 */
      puts("ERROR [radix_sort_lds_m] failed to allocate memory for bucket_cell.");
      for(j=0;j<no_used_cells_count;i++) free(no_used_cells[j]); /* 確保したセルを解放 */
      free(no_used_cells); /* 動的確保した配列も解放 */
      return;
    }
    no_used_cells[no_used_cells_count] = cell_ptr; /* 確保したセルは配列へ入れておく */
  }

  for(bucket_idx=0;bucket_idx<BUCKET_COUNT*2;bucket_idx++){ /* バケツの準備 バケツ1つ1つについて */
    buckets[bucket_idx] = NULL;
    bucket_last_cells[bucket_idx] = NULL;
  }

  for(byte_idx=0;byte_idx<__SIZEOF_DOUBLE__;byte_idx++){ /* データのbyte_idxバイト目に注目する byte_idx=0>1>2>...>SIZEOF_DOUBLE */
    for(bit_idx=0;bit_idx<BYTE_BIT;bit_idx+=WINDOW_BIT){ /* 注目した1byteの中でbit_idx番目からWINDOW_BIT個のbit列に注目する (WINDOW_BIT=2の場合)bit_idx=0,2,4,6 */
      /* つまり上2つのforループ内で(8byte=64bitデータでWINDOW_BITが2の場合) 0~1, 2~3, ..., 62~63  (最初は0bit目から1bit目までが、次は2bit目から3bit目までが...最後は62から63bit目までが注目される)*/
      if(byte_idx==__SIZEOF_DOUBLE__-1 && bit_idx==BYTE_BIT-WINDOW_BIT) bit_mask = bit_mask_nosign; /* ループの最後だけ(注目するビット列に符号ビットが含まれる)bit_maskで符号ビットを見ないようにする */

      for(i=0;i<n;i++){ /* 全てのデータをバケツに投入する作業 データ1件ずつ */
        uni.double_val = a[i]; /* データをビット演算をするために共有体にdouble型として代入 */
        window = ((uni.byte[byte_idx])>>bit_idx) & bit_mask; /* 共有体のbyte_idx番目のバイトの、(下位から数えて)bit_idx番目からWINDOW_BIT長のbit列を取り出す */
        /* uni.byte[byte_idx] : uni.double_valの領域とuni.byte[0~8?]の領域は共有されているので、uni.double_valに代入したデータのi番目のバイトにはuni.byte[i]でアクセスできる */
        /* 例:[01110110]の4ビット目(0が最下位ビットなので自然に言うと5ビット目)から2ビット分を取り出すと [01'11'0110] の 11 = 0d3 が得られる */

        /* もしWINDOW_BITが2なら、データから取り出したwindowは00,01,10,11 = 10進数なら0,1,2,3のどれかになる */
        if(uni.double_val>=0){ /* 正負で処理を分ける */
          window += BUCKET_COUNT; /* データが0または正の場合、windowをBUCKET_COUNT分加算する BUCKET_COUNTが4であれば、windowは 0->4, 1->5, 2->6, 3->7 となる */
        }else{
          window = BUCKET_COUNT-window-1; /* データが負の場合、windowが大きい程小さくなるよう更新する BUCKET_COUNTが4であれば、windowは 0->3, 1->2, 2->1, 3->0 となる */
        }
        /* window番目のバケツの”末尾”にデータを入れる */
        cell_ptr = no_used_cells[--no_used_cells_count]; /* バケツの長さを伸ばすために未使用セルが集まった配列の最後のセルを取ってくる */
        cell_ptr->double_val = uni.double_val; /* セルに値をセット */
        cell_ptr->next = NULL;
        if(!buckets[window]){ /* window番目のバケツにデータがない場合(リストの先頭セルへのアドレスが入るbuckets[window]がNULLの場合) */
          buckets[window] = cell_ptr; /* 先頭セルとして用意したセルを投入 */
          bucket_last_cells[window] = cell_ptr; /* もちろん投入したセルが最後のセル */
        }else{ /* window番目のバケツにデータがある場合(リストの先頭セルへのアドレスが入るbuckets[window]がNULLでない場合) */
          bucket_last_cells[window]->next = cell_ptr; /* 末尾にデータを追加したいので、最後のセルの次のセルとして用意したセルをリンク */
          bucket_last_cells[window] = cell_ptr; /* もちろん投入したセルが最後のセル */
        }
      }


      i = 0;
      for(bucket_idx=0;bucket_idx<BUCKET_COUNT*2;bucket_idx++){ /* データが入ったバケツ達を順番通りにひっくり返してa配列に戻す作業 0番目のバケツから順に */
        
        cell_ptr = buckets[bucket_idx]; /* リストの先頭セルへのアドレスに注目して */
        while(cell_ptr){ /* 注目しているアドレスがNULLになるまで */
          a[i++] = cell_ptr->double_val; /* 注目したセルの数値をa配列に戻す */
          no_used_cells[no_used_cells_count++] = cell_ptr; /* 注目したセルはもう使わないので未使用セルの配列の末尾にセット */
          cell_ptr = cell_ptr->next; /* 注目するセルを今のセルからリンクされている次のセルに移す */
        }
        buckets[bucket_idx] = NULL; /* ひっくり返して空になったバケツはもちろん空 */
        bucket_last_cells[bucket_idx] = NULL;
      }

    }
  }

  for(i=0;i<n;i++) free(no_used_cells[i]); /* 確保したセルを解放 */
  free(no_used_cells); /* 使用されていないセルを置いておくための配列も動的確保なので解放 */
}

void bogo_sort(double a[], int n){ /* nによっては現実的な時間で終了しません */
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

