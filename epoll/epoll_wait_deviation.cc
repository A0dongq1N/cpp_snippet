/**
验证epoll_wait的等待时间
tlinux系统上：
[root@VM-73-4-tencentos build]# sysctl -a | grep min_epoll_wait_time
min_epoll_wait_time = 1
[root@VM-73-4-tencentos build]# sysctl min_epoll_wait_time
min_epoll_wait_time = 1
[root@VM-73-4-tencentos build]# sudo sysctl -w min_epoll_wait_time=2
min_epoll_wait_time = 2
 * */
 #include <stdio.h>
 #include <assert.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <sys/time.h>
 #include <sys/epoll.h>
  
 int64_t get_current_time(){
     timeval now;
     int ret = gettimeofday(&now, NULL);
     assert(ret != -1);
  
     return now.tv_sec * 1000 + now.tv_usec / 1000;
 }
  
 int main(int argc, char** argv){
     int ep = epoll_create(1024);
     if(ep == -1){
         printf("create epoll error!\n"); exit(-1);
     }
     
     for(int i = 0; i < 2 * 60 * 60 * 1000; i += 1){
        timeval start;
        gettimeofday(&start, NULL);

         epoll_event events[1];
         // 此时的等待时间为max(min_epoll_wait_time, 1),内核参数优先
         if(epoll_wait(ep, events, 1, 1) == -1){
             printf("wait epoll error!\n"); exit(-1);
         }
         
        timeval end;
        gettimeofday(&end, NULL);
         printf("begin=%llu, end=%llu, epoll wait =%llu (us)\n", start.tv_usec, end.tv_usec, end.tv_usec - start.tv_usec);
     }
     
     close(ep);
     
     return 0;
 }