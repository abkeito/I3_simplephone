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

void *thread(void *param);

int main(int args, char**argv){
    if((args != 2)&&(args != 3)){
        perror("usage: ./phone <server><port> or ./phone <port>");
        exit(1);
    }
    pthread_t thread1;
    int ret;
    int N = 50000;
    char data[N];
    ////////SERVER/////////////
    if(args == 2){
        //step0　入力読み捨てるスレッド
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

        //step 3:待ち受け宣言
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
        //send
        char* data = (char*)calloc(N, sizeof(char));
        char* data1 = (char*)calloc(N, sizeof(char));
        while(1){
            //音を送る
            n = read(0, data, sizeof(data));
            if(n == -1){perror("read"); exit(1);}
            m = send(s, data, n, 0);
            if(m == -1){perror("send");exit(1);}
            if(n == 0){
                break;}
            //音を受け取る
            n1 = recv(s, data1, n, 0);
            if(n1 == -1){perror("recv"); exit(1);}
            if(n1 == 0) break; /*EOFに来た*/
            //if(data[0] == 'a'){break;}
            write(1, data1, sizeof(data1));
        }
        //step 6:通信終了後、ソケットを待つ
        close(ss);
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
        //send
        char* data = (char*)calloc(N, sizeof(char));
        char* data1 = (char*)calloc(N, sizeof(char));
        while(1){
            //音を送る
            n = read(0, data, sizeof(data));
            if(n == -1){perror("read"); exit(1);}
            m = send(s, data, n, 0);
            if(m == -1){perror("send");exit(1);}
            if(n == 0){
                break;}
            //音を受け取る
            n1 = recv(s, data1, n, 0);
            if(n1 == -1){perror("recv"); exit(1);}
            if(n1 == 0) break; /*EOFに来た*/
            //if(data[0] == 'a'){break;}
            write(1, data1, sizeof(data1));
        }
        close(s);

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