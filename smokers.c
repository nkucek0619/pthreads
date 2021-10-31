/*
    Operating Systems Lab 2: POSIX Threads and Semaphores

    Author: Nicholas Kucek
    
    Date Completed: 10/31/21
    
    SUMMARY:
    -3 agent threads
    -3 pushers
    -6 smokers: 2 tobacco, 2 papers, 2 matches
    -Each smoker finishes 3 cigarettes before exiting
    -Rather than loop forever, each agent loops 6 times, and each pusher 12 times.
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t m =	PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t smoker = PTHREAD_MUTEX_INITIALIZER;

//actors conditionals
pthread_cond_t agent_c = PTHREAD_COND_INITIALIZER;
pthread_cond_t smoker_tobacco_c = PTHREAD_COND_INITIALIZER;
pthread_cond_t smoker_match_c =	PTHREAD_COND_INITIALIZER;
pthread_cond_t smoker_paper_c =	PTHREAD_COND_INITIALIZER;

//resource conditionals
pthread_cond_t tobacco = PTHREAD_COND_INITIALIZER;
pthread_cond_t paper = PTHREAD_COND_INITIALIZER;
pthread_cond_t match = PTHREAD_COND_INITIALIZER;

int has_tobacco = 0;
int has_paper = 0;
int has_match = 0;

int agent_job = 1;
int smoker_tobacco_job = 0;
int smoker_match_job = 0;
int smoker_paper_job = 0;

int getRand(int range){
    
    int n = rand() % range;
    
    return n;
    
}

// function for calls to the agent processes
void * agent(void * arg){
    
    for(;;) {
        
        sleep(5);
        
        pthread_mutex_lock(&m);
        
        //The agent stays waiting if agent_job is equal 0
        while(agent_job == 0)
            pthread_cond_wait(&agent_c, &m);
        
        int n = getRand(3);
        
        printf("NEW AGENT\n");
        
        //agent has paper and match
        if (n == 0) {
           
            agent_job = 0;
            has_match = 1;
            has_paper = 1;
            printf("The agent generates paper and match.\n");
            pthread_cond_signal(&paper);
            pthread_cond_signal(&match);

        }
        //agent has tobacco and match
        else if (n == 1) {
            agent_job = 0;
            has_match = 1;
            has_tobacco = 1;
            printf("The agent generates tobacco and match.\n");
            pthread_cond_signal(&paper);
            pthread_cond_signal(&match);
        }
        //agent has tobacco and paper
        else if (n == 2) {
            agent_job = 0;
            has_tobacco = 1;
            has_paper = 1;
            printf("The agent generates paper and tobacco.\n");
            pthread_cond_signal(&paper);
            pthread_cond_signal(&tobacco);
        }
        pthread_mutex_unlock(&m);
    }
    return 0;
}

// pusher thread for the paper smoker
void * pusher_paper(void * arg) {
    
    for(;;) {
        pthread_mutex_lock(&m);
        while(has_paper == 0)
            pthread_cond_wait(&paper, &m);
    
        if (has_match == 1) {
            has_match = 0;
            agent_job = 0;
            smoker_tobacco_job = 1;
            printf("Call tobacco smoker.\n");
            pthread_cond_signal(&smoker_tobacco_c);
        }
        if (has_tobacco == 1) {
            has_tobacco = 0;
            agent_job = 0;
            smoker_match_job = 1;
            printf("Call match smoker.\n");
            pthread_cond_signal(&smoker_match_c);
        }
        pthread_mutex_unlock(&m);
    }
    return 0;
}

// pusher thread for the match smoker
void * pusher_match(void * arg){
    
    for(;;) {
        pthread_mutex_lock(&m);
        while(has_match == 0)
            pthread_cond_wait(&match, &m);
    
        if(has_paper == 1) {
            has_paper = 0;
            agent_job = 0;
            smoker_tobacco_job = 1;
            printf("Call tobacco smoker.\n");
            pthread_cond_signal(&smoker_tobacco_c);
        }
        if(has_tobacco == 1) {
            has_tobacco = 0;
            agent_job = 0;
            smoker_paper_job = 1;
            printf("Call the paper smoker.\n");
            pthread_cond_signal(&smoker_paper_c);
        }
        pthread_mutex_unlock(&m);
    }
    
    return 0;
}

// pusher thread for the tobacco smoker
void * pusher_tobacco(void * arg) {
    
    for(;;) {
        pthread_mutex_lock(&m);
        while(has_tobacco == 0)
            pthread_cond_wait(&tobacco, &m);
    
        if(has_match == 1) {
            has_match = 0;
            agent_job = 0;
            smoker_paper_job = 1;
            printf("Call paper smoker.\n");
            pthread_cond_signal(&smoker_paper_c);
        }
        if(has_paper == 1) {
            has_tobacco = 0;
            agent_job = 0;
            smoker_match_job = 1;
            printf("Call match smoker.\n");
            pthread_cond_signal(&smoker_match_c);
        }
        pthread_mutex_unlock(&m);
    }
    return 0;
}

// function to signal tobacco smoker
void * smoker_tobacco(void * arg){
    
    for(;;){
        
        pthread_mutex_lock(&smoker);
        while(smoker_tobacco_job == 0)
            pthread_cond_wait(&smoker_tobacco_c, &smoker);
        has_paper = 0;
        has_match = 0;
        smoker_tobacco_job = 0;
        agent_job = 1;
        printf("Tobacco smoker is making a cigarette...\n");
        pthread_mutex_unlock(&smoker);
        
        printf("Tobacco smoker is smoking...\n");
    }
    return 0;
}

// function to signal paper smoker
void * smoker_paper(void * arg){
    
    for(;;){
        
        pthread_mutex_lock(&smoker);
        while(smoker_paper_job == 0)
            pthread_cond_wait(&smoker_paper_c, &smoker);
        has_tobacco = 0;
        has_match = 0;
        smoker_paper_job = 0;
        agent_job = 1;
        printf("Paper smoker is making a cigarette...\n");
        pthread_mutex_unlock(&smoker);
        
        printf("Paper smoker is smoking...\n");
    }
    return 0;
}

// function to signal match smoker
void * smoker_match(void * arg){
    
    for(;;){
        
        pthread_mutex_lock(&smoker);
        while(smoker_match_job == 0)
            pthread_cond_wait(&smoker_match_c, &smoker);
        has_paper = 0;
        has_tobacco = 0;
        smoker_match_job = 0;
        agent_job = 1;
        printf("Match smoker is making a cigarette...\n");
        pthread_mutex_unlock(&smoker);
        
        printf("Match smoker is smoking...\n");
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // p_threads for running the agent and smoker processes with semaphores
    pthread_t agent_t_one, agent_t_two, agent_t_three,
    smoker_tobacco_t_one, smoker_paper_t_one, smoker_match_t_one, smoker_tobacco_t_two, smoker_paper_t_two, smoker_match_t_two,
    pusher_tobacco_t, pusher_paper_t, pusher_match_t;
    
    // random seed
    time_t t;
    srand((unsigned) time(&t));
    
    if (pthread_create(&agent_t_one, NULL, agent, NULL) != 0) {
        fprintf (stderr, "Unable to create thread\n");
        exit(1);
    }
    
    if (pthread_create(&agent_t_two, NULL, agent, NULL) != 0) {
        fprintf (stderr, "Unable to create thread\n");
        exit(1);
    }
    
    if (pthread_create(&agent_t_three, NULL, agent, NULL) != 0) {
        fprintf (stderr, "Unable to create thread\n");
        exit(1);
    }

    if (pthread_create(&pusher_tobacco_t, NULL, pusher_tobacco, NULL) != 0) {
        fprintf (stderr, "Unable to create thread\n");
        exit(1);
    }
    
    if (pthread_create(&pusher_paper_t, NULL, pusher_paper, NULL) != 0) {
        fprintf (stderr, "Unable to create thread\n");
        exit(1);
    }
    
    if (pthread_create(&pusher_match_t, NULL, pusher_match, NULL) != 0) {
        fprintf (stderr, "Unable to create thread\n");
        exit(1);
    }
    
    if (pthread_create(&smoker_tobacco_t_one, NULL, smoker_tobacco, NULL) != 0) {
        fprintf (stderr, "Unable to create thread\n");
        exit(1);
    }
    
    if (pthread_create(&smoker_paper_t_one, NULL, smoker_paper, NULL) != 0) {
        fprintf (stderr, "Unable to create thread\n");
        exit(1);
    }
    
    if (pthread_create(&smoker_match_t_one, NULL, smoker_match, NULL) != 0) {
        fprintf (stderr, "Unable to create thread\n");
        exit(1);
    }
    
    if (pthread_create(&smoker_tobacco_t_two, NULL, smoker_tobacco, NULL) != 0) {
        fprintf (stderr, "Unable to create thread\n");
        exit(1);
    }
    
    if (pthread_create(&smoker_paper_t_two, NULL, smoker_paper, NULL) != 0) {
        fprintf (stderr, "Unable to create thread\n");
        exit(1);
    }
    
    if (pthread_create(&smoker_match_t_two, NULL, smoker_match, NULL) != 0) {
        fprintf (stderr, "Unable to create thread\n");
        exit(1);
    }
    
    pthread_join(agent_t_one, NULL);
    pthread_join(agent_t_two, NULL);
    pthread_join(agent_t_three, NULL);
    pthread_join(pusher_tobacco_t, NULL);
    pthread_join(pusher_paper_t, NULL);
    pthread_join(pusher_match_t, NULL);
    pthread_join(smoker_tobacco_t_one, NULL);
    pthread_join(smoker_paper_t_one, NULL);
    pthread_join(smoker_match_t_one, NULL);
    pthread_join(smoker_tobacco_t_two, NULL);
    pthread_join(smoker_paper_t_two, NULL);
    pthread_join(smoker_match_t_two, NULL);

}
