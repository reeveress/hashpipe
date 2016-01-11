/*
 * hpdemo2.c
 * hashpipe 'hello world' tutorial
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
//You must include the hashpipe header file to access hashpipe definitions
#include "hashpipe.h"
static int init(hashpipe_thread_args_t * args)
{
	printf("Init World!\n");
	return 0;
}

//the main body of the thread is the run function Normally this fuction will
//have a loop to have it run continuously but for this demo it's just a simple
//print statement
static void *run(hashpipe_thread_args_t * args)
{
	hashpipe_status_t st = args->st;
	int i=0;
	while (1){
		hashpipe_status_lock_safe(&st);
		// Store counter value in status buffer
		hputi4(st.buf, "COUNTER", i);
		hashpipe_status_unlock_safe(&st);
		i++;
		sleep(1);
	}
}




//This structure is the hashpipe thread descriptor that gets passed to hashpipe
//when the pluggin loads This is the runtime information about the thread e.g.
//run is a pointer to the above run function
static hashpipe_thread_desc_t hpdemo2_thread = {
    name: "hpdemo2_thread",
    skey: "HPD2STAT",
    init: init,
    run:  run,
    ibuf_desc: {NULL},
    obuf_desc: {NULL}
};
//This is how the hashpipe learns about the existence of the thread descriptor
//The constructor attribute means that this function will be called
//automatically when hashpipe loads the pluggin
static __attribute__((constructor)) void ctor()
{
//register_hashpipe_thread function is part of hashpipe and needs a pointer to
//hashpipe thread descriptor(hpdemo2_thread).  It has to be called once per
//hashpipe thread 
  register_hashpipe_thread(&hpdemo2_thread);
}
