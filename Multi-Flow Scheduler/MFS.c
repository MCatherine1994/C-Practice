/*

time calculation may be a nightware! 
Beware of float, int, unsigned int conversion.
you could use gettimeofday(...) to get down to microseconds!

*/

//# header files
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#define MAXFLOW 1000
struct timeval start;

typedef struct _flow
{
    float arrivalTime;
    float transTime;
    int priority;
    int id;
}flow;

flow flowList[MAXFLOW];   // parse input in an array of flow
flow *queueList[MAXFLOW];  // store waiting flows while transmission pipe is occupied.
pthread_t thrList[MAXFLOW]; // each thread executes one flow
pthread_mutex_t trans_mtx = PTHREAD_MUTEX_INITIALIZER ; 
pthread_mutex_t queue_mtx = PTHREAD_MUTEX_INITIALIZER ;
pthread_cond_t trans_cvar = PTHREAD_COND_INITIALIZER ;  
int trans_thread = 0;
int queue_size = 0;

double calTime()
{
   struct timeval time;
   gettimeofday(&time, NULL);
   
   double calTime = (time.tv_sec - start.tv_sec) * 1.0;
   calTime += (time.tv_usec - start.tv_usec)/1000000.0;
   return calTime;
}

void addToQueue(flow *newitem){
    int i = queue_size;
    flow *curr;
    queueList[i] = newitem;
    while(i > 0){
        if(queueList[i-1]->priority > queueList[i]->priority){
            curr = queueList[i-1];
            queueList[i-1] = queueList[i];
            queueList[i] = curr;
        }else if(queueList[i-1]->priority == queueList[i]->priority){
            if(queueList[i-1]->arrivalTime > queueList[i]->arrivalTime){
                curr = queueList[i-1];
                queueList[i-1] = queueList[i];
                queueList[i] = curr;
            }else if(queueList[i-1]->arrivalTime == queueList[i]->arrivalTime){
                if(queueList[i-1]->transTime > queueList[i]->transTime){
                    curr = queueList[i-1];
                    queueList[i-1] = queueList[i];
                    queueList[i] = curr;
                }else if(queueList[i-1]->transTime == queueList[i]->transTime){
                    if(queueList[i-1]->id > queueList[i]->id){
                        curr = queueList[i-1];
                        queueList[i-1] = queueList[i];
                        queueList[i] = curr;
                    }
                }
            }
        }
        i--;
    }
    queue_size++;
    return;
}

void requestPipe(flow *item) {
    //lock mutex;
    pthread_mutex_lock(&trans_mtx);
//    printf("%d",trans_thread);
    if(!trans_thread && (queue_size == 0)){
        trans_thread = item->id;
        //unlock mutex;
        pthread_mutex_unlock(&trans_mtx);
        return;
    }   
 
    //add item in queue, sort the queue according rules
    pthread_mutex_lock(&queue_mtx);
    addToQueue(item);
    pthread_mutex_unlock(&queue_mtx);
  
    printf("Flow %2d waits for the finish of flow %2d. \n", item->id, trans_thread);
    // key point here..
    // wait till pipe to be available and be at the top of the queue
    while(item->id != trans_thread){
        pthread_cond_wait(&trans_cvar, &trans_mtx);
   	pthread_mutex_lock(&queue_mtx);
        trans_thread = queueList[0]->id;
	pthread_mutex_unlock(&queue_mtx);
    }  

    // update queue
    pthread_mutex_lock(&queue_mtx);
    int i = 0;
    while(i<queue_size)
    {
       queueList[i]=queueList[i+1];
       i++;
    }
    queue_size--;
    pthread_mutex_unlock(&queue_mtx);

    return;
}

void releasePipe() {
    // I believe you genuis will figure this out!
    pthread_mutex_unlock(&trans_mtx);
    pthread_mutex_lock(&trans_mtx);
    pthread_cond_broadcast(&trans_cvar);
    pthread_mutex_unlock(&trans_mtx);
}

// entry point for each thread created
void *thrFunction(void *flowItem) {

    flow *item = (flow *)flowItem;

    // wait for arrival
    usleep(item->arrivalTime*1000000);
    printf("Flow %2d arrives: arrival time (%.2f), transmission time (%.1f), priority (%2d). \n",item->id, calTime(), item->transTime, item->priority);

    requestPipe(item) ;
    printf("Flow %2d starts its transmission at time %.2f. \n", item->id, calTime());

    // sleep for transmission time
    usleep(item->transTime*1000000);

    releasePipe(item);
    printf("Flow %2d finishes its transmission at time %.2f. \n", item->id, calTime());
    //pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	
    int i = 0;
    char buffer[256];
    gettimeofday(&start,NULL);
    // file handling
    if(argc < 2){
	fprintf(stderr, "\nThe input is not valid.\n");
	return 0;
    }
    FILE *fp = fopen(argv[1],"r"); // read file
    if(fp == NULL){
   	printf("Cannot open the file.\n");
    	return 0;
    }
    // read number of flows
    
    fgets(buffer,256,fp);
    int numFlows = atoi(buffer);
    for(i = 0; i <numFlows; i++) {
        fgets(buffer,256,fp); // read line by line
        //parse line, using strtok() & atoi(), store them in flowList[i] ;
        char* token = strtok(buffer, ":");
        int flowArray[4];
        int j = 0;
        while(token){
        	flowArray[j++] = atoi(token);
        	token = strtok(NULL, ",");
        }
        flowList[i].id = flowArray[0];
        flowList[i].arrivalTime = (float)flowArray[1]/10;
        flowList[i].transTime = (float)flowArray[2]/10;
        flowList[i].priority = flowArray[3];
    }

    fclose(fp); // release file descriptor

    for(i = 0; i < numFlows; i++){
    	// create a thread for each flow 
        pthread_create(&thrList[i], NULL, thrFunction, (void *)&flowList[i]) ;
    }
       
    // wait for all threads to terminate
    for(i = 0; i < numFlows; i++){
    	pthread_join(thrList[i],NULL);
    }
    
    // destroy mutex & condition variable
    pthread_mutex_destroy(&trans_mtx);
    pthread_mutex_destroy(&queue_mtx);
    pthread_cond_destroy(&trans_cvar);
    return 0;
}
