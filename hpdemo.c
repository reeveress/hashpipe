/*
 * hpdemo.c
 * hashpipe 'hello world' tutorial
 */

#include <stdio.h>
//You must include the hashpipe header file to access hashpipe definitions
#include "hashpipe.h"
//the main body of the thread is the run function Normally this fuction will
//have a loop to have it run continuously but for this demo it's just a simple
//print statement
static void *run(hashpipe_thread_args_t * args)
{
	printf("Hello World!\n");
}
//This structure is the hashpipe thread descriptor that gets passed to hashpipe
//when the pluggin loads This is the runtime information about the thread e.g.
//run is a pointer to the above run function
static hashpipe_thread_desc_t hpdemo_thread = {
    name: "hpdemo_thread",
    skey: "HPDSTAT",
    init: NULL,
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
//hashpipe thread descriptor(hpdemo_thread).  It has to be called once per
//hashpipe thread 
  register_hashpipe_thread(&hpdemo_thread);
}
