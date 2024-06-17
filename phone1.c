#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
typedef struct receive_param {
    int s;
    int N;
    pthread_t thread1;
} receive_param;
typedef struct send_param {
    int s;
    int N;
    pthread_t thread1;
} send_param;
void *thread(void *param);
void *myreceive(void *param);
void *mysend(void *param);

int main(int args, char**argv){
    if((args != 2)&&(args != 3)){
        perror("usage: ./phone <server><port> or ./phone <port>");
        exit(1);
    }
    pthread_t thread1;
    int ret;
    int N = 1000;
    ////////SERVER/////////////
    if(args == 2){
        //step0:入力読み捨てるスレッド
        ret = pthread_create(&thread1, NULL, thread, NULL);
        if (ret != 0) {
            // pthread_createが失敗した場合は0以外の値を返す
            exit(1);
        }
        int n, m, n1, m1;
        //step 1:ソケットを作る
        int ss = socket(PF_INET, SOCK_STREAM, 0);
        //step 2:どのポートで待ち受けるか決める
        struct sockaddr_in addr;
        addr.sin_family = AF_INET; /*これはIPv4のアドレスです*/
        addr.sin_port = htons(atoi(argv[1])); /*ポートは...です*/
        addr.sin_addr.s_addr = INADDR_ANY;/*どのIPアドレスでも待ち受けできる*/
        bind(ss, (struct sockaddr *)&addr, sizeof(addr));

        //step 3:待ち受け宣言 ->これは無限に続く
        listen(ss, 10);

        //step 4:クライアントがconnectするまで待つ
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(struct sockaddr_in);
        int s = accept(ss, (struct sockaddr *)&client_addr, &len);
        if(s == -1){
            perror("accept");
            exit(1);
        }
        pthread_cancel(thread1); /* status を示して終了 */

        //step 5:データの送信、受信を待つ
        pthread_t thread_rec;
        pthread_t thread_send;
        //step 5:データの送信、受信を待つ
        //receive
        receive_param receive_param0;
        receive_param0.s = s;
        receive_param0.N = N;
        receive_param0.thread1 = thread_rec;
        //send
        send_param send_param0;
        send_param0.s = s;
        send_param0.N = N;
        send_param0.thread1 = thread_rec;
        //char* data = (char*)calloc(N, sizeof(char));
        //char* data1 = (char*)calloc(N, sizeof(char));
        //step 6:スレッドで
        ret = pthread_create(&thread_rec, NULL, myreceive, (void*)&receive_param0);
        ret = pthread_create(&thread_send, NULL, mysend, (void*)&send_param0);
        if (ret != 0) {
            // pthread_createが失敗した場合は0以外の値を返す
            perror("chat");
            exit(1);
        }
        while(1){
            sleep(5);
        }
        return EXIT_SUCCESS;
    }
    //////////////CLIENT//////////////////////////////////
    if(args == 3){
        //step0　入力読み捨てるスレッド
        ret = pthread_create(&thread1, NULL, thread, NULL);
        if (ret != 0) {
            // pthread_createが失敗した場合は0以外の値を返す
            exit(1);
        }
        //1.socket
        int s = socket(PF_INET, SOCK_STREAM, 0);
        if(s == -1){
            perror("socket");
            exit(1);
        }
        //step 2.connect
        struct sockaddr_in addr;
        addr.sin_family = AF_INET; /*これはIPv4のアドレスです*/
        if(inet_aton(argv[1], &addr.sin_addr) == 0){
            perror("inet_aton"); exit(1);
        }; 
        /*IPアドレスは...です*/
        addr.sin_port = htons(atoi(argv[2])); /*ポートは...です*/
        /*遂にconnect*/
        if(connect(s, (struct sockaddr *)&addr, sizeof(addr)) == -1){
            perror("connect");
            exit(1);
        }
        pthread_cancel(thread1); /* status を示して終了 */
        //step 3.4 recordを呼び出す
        //int N = 20000;
        int n, m, n1;
        //step 5:データの送信、受信を待つ
        pthread_t thread_rec;
        pthread_t thread_send;
        //step 5:データの送信、受信を待つ
        //send
        receive_param receive_param0;
        receive_param0.s = s;
        receive_param0.N = N;
        receive_param0.thread1 = thread_rec;
        //receive
        send_param send_param0;
        send_param0.s = s;
        send_param0.N = N;
        send_param0.thread1 = thread_rec;
        //char* data = (char*)calloc(N, sizeof(char));
        //char* data1 = (char*)calloc(N, sizeof(char));
        //step 6:スレッドで
        ret = pthread_create(&thread_rec, NULL, myreceive, (void*)&receive_param0);
        ret = pthread_create(&thread_send, NULL, mysend, (void*)&send_param0);
        if (ret != 0) {
            // pthread_createが失敗した場合は0以外の値を返す
            perror("chat");
            exit(1);
        }
        while(1){
            sleep(5);
        }
        return EXIT_SUCCESS;
    }
}

void *thread(void *param){
    char data_bye[100];
    while(1){
        int n = read(0, data_bye, sizeof(data_bye));
        //printf("%s", data_bye);
    }
}

void *myreceive(void *param){
    receive_param* receive_param0 = (receive_param *)param;
    int N = receive_param0->N;
    int s = receive_param0->s;
    char* data1 = (char*)calloc(N, sizeof(char));
    FILE* fp = fopen("phoneN.txt", "a");
    int n1;
    while(1){
        n1 = recv(s, data1, N, 0);
        fprintf(fp, "N = %d\n", n1);
        if(n1 == -1){perror("recv"); exit(1);}
        if(n1 == 0) break; /*EOFに来た*/
        //if(data[0] == 'a'){break;}
        write(1, data1, n1);
    }
    close(s);
    pthread_cancel(receive_param0->thread1);
    return EXIT_SUCCESS;
}

void *mysend(void *param){
    send_param* send_param0 = (send_param *)param;
    int N = send_param0->N;
    int s = send_param0->s;
    char* data = (char*)calloc(N, sizeof(char));
    int n, m;
    while(1){
        n = read(0, data, sizeof(data));
        if(n == -1){perror("read"); exit(1);}
        m = send(s, data, n, 0);
        if(m == -1){perror("send");exit(1);}
        if(n == 0){
            break;}
    }
    close(s);
    pthread_cancel(send_param0->thread1);
    return EXIT_SUCCESS;
}