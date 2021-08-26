#include "pthread_divetimpera.h"
#include "../common/data_generation.h"
#include "../common/merge_implementations.h"

using std::cout;
using std::endl;

typedef struct {
  uint64_t mitt;
  uint64_t span;
  uint64_t left;
  uint64_t right;
} mydata_t;

mydata_t **tasks;

int *array;
int *tmp_array;
pthread_t *threads;
pthread_cond_t *cond;
pthread_mutex_t *mutex;

void *worker(void *args){
  int myidx = *((int *) args);
  mydata_t *elem;


  pthread_mutex_lock(&mutex[myidx]);
  while(tasks[myidx] == NULL){
    pthread_cond_wait(&cond[myidx], &mutex[myidx]);
  }
  elem = tasks[myidx];
  tasks[myidx] = NULL;
  pthread_mutex_unlock(&mutex[myidx]);

  mydata_t *half;
  uint64_t left = elem->left;
  uint64_t middle;
  uint64_t right = elem->right;

  int dest = myidx + elem->span;
  uint64_t span = elem->span /2;

  while(dest > myidx){
    middle = (left + right) /2;

    half = (mydata_t *) malloc(sizeof(mydata_t));
    half->mitt = myidx;
    half->span = span;
    half->left = middle;
    half->right = right;

    right = middle;

    pthread_mutex_lock(&mutex[dest]);
    tasks[dest] = half;
    pthread_cond_signal(&cond[dest]);
    pthread_mutex_unlock(&mutex[dest]);

    dest = myidx + span;
    span = span /2;
  }

  merge_sort(array, tmp_array, left, right - 1);

  if(span == 0) {
    span = 1;
    dest = myidx + span;
  }

  while(span <= elem->span){

    pthread_mutex_lock(&mutex[myidx]);
    while(tasks[myidx] == NULL){
      pthread_cond_wait(&cond[myidx], &mutex[myidx]);
    }
    half = tasks[myidx];
    tasks[myidx] = NULL;
    pthread_mutex_unlock(&mutex[myidx]);


    // combines the two ordered subarrays
    right = half->right;
    merge(array, tmp_array, left, (left + right - 1) /2, right - 1);

    // calculation of the id of the previous process
    span = span * 2;
    dest = span + myidx;
    free(half);
    half = NULL;
  }


  if(myidx != 0) {
    pthread_mutex_lock(&mutex[elem->mitt]);
    tasks[elem->mitt] = elem;
    pthread_cond_signal(&cond[elem->mitt]);
    pthread_mutex_unlock(&mutex[elem->mitt]);
  }

  return NULL;
}


int main(int argc, char *argv[]) {
    uint64_t len;
    int num_threads;

    array = common_begin(argc, argv, &len, &num_threads);
    tmp_array = (int *) malloc(len * sizeof(int));

    clock_t start = clock();

    threads = (pthread_t *) malloc(num_threads * sizeof(pthread_t));
    cond = (pthread_cond_t *) malloc(num_threads * sizeof(pthread_cond_t));
    mutex = (pthread_mutex_t *) malloc(num_threads * sizeof(pthread_mutex_t));
    tasks = (mydata_t **) malloc(num_threads * sizeof(mydata_t *));

    // create threads
    int *tmp;
    for(int j = 0; j < num_threads; j++){
      pthread_mutex_init(&mutex[j], NULL);
      pthread_cond_init(&cond[j], NULL);
      tmp = (int *) malloc(sizeof(int));
      *tmp = j;
      pthread_create(&threads[j], nullptr, worker, (void *) tmp);

    }

    sleep(1);


    mydata_t *main_task = (mydata_t *) malloc(sizeof(mydata_t));
    main_task->mitt = -1;
    main_task->left = 0;
    main_task->right = len;
    main_task->span = num_threads / 2;

    pthread_mutex_lock(&mutex[0]);
    tasks[0] = main_task;
    pthread_cond_signal(&cond[0]);
    pthread_mutex_unlock(&mutex[0]);

    // waiting threads
    for(int j = 0; j < num_threads; j++){
        pthread_join(threads[j], nullptr);
    }

    clock_t end = clock();
    common_end(start, end, array, len);
    free(array);

    return 0;
}
