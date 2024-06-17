#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_func1(void *param);
void *thread_func2(void *param);

int main(){
    pthread_t thread;
    pthread_t thread1;
    int ret = 0;

    // スレッドの作成
    // int pthread_create(pthread_t * thread, pthread_attr_t * attr,
    //                    void * (*start_routine)(void *), void * arg)
    // 第4引数に渡した値が、スレッド関数の引数に渡される
    ret = pthread_create(&thread, NULL, thread_func1, NULL);
    if (ret != 0) {
        // pthread_createが失敗した場合は0以外の値を返す
        exit(1);
    }
    ret = pthread_create(&thread1, NULL, thread_func2, NULL);
    if (ret != 0) {
        // pthread_createが失敗した場合は0以外の値を返す
        exit(1);
    }

    // スレッドの終了を待機
    // int pthread_join(pthread_t th, void **thread_return)
    ret = pthread_join(thread, NULL);
    if (ret != 0) {
        // pthread_joinが失敗した場合は0以外の値を返す
        exit(1);
    }
    pthread_exit(&ret); /* status を示して終了 */

    /*
    // スレッドの終了を待たない
    // int pthread_detach(pthread_t th)
    ret = pthread_detach(thread);
    if (ret != 0) {
        // pthread_detachが失敗した場合は0以外の値を返す
        exit(1);
    }
    */

    return EXIT_SUCCESS;
}

void *thread_func1(void *param)
{
    printf("1");
    // 非同期に実行する処理
}

void *thread_func2(void *param)
{
    printf("2");
    // 非同期に実行する処理
}
