#Hashpipe Tutorial 2
In the second tutorial we will make a counter that stores its value inside a status buffer which could be viewed using hashpipe_status_monitor.rb script.The init function remains the same aside from 'Hello World!' being changed to 'Init World!'. The run function now has a while loop with the i counter incrementing indefinitely or until terminated by the user. 

```c
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

//the main body of the thread is the run function. This time we implemented a
//loop that runs continuously.

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
```
The make file remains virtually unchanged aside from file name changes(hpdemo.so to hpdemo2.so etc). 

```makefile
//This structure is the hashpipe thread descriptor that gets passed to hashpipe
//when the plugin loads.This is the runtime information about the thread e.g.
//run is a pointer to the above run function
static hashpipe_thread_desc_t hpdemo2_thread = {
    name: "hpdemo2_thread",
    skey: "HPD2STAT",
    init: init,
    run:  run,
    ibuf_desc: {NULL},
    obuf_desc: {NULL}
};
		     -I. -I$(CUDA_DIR)/include -I/usr/local/include \
		     -L. -L/usr/local/lib \
		     -lhashpipe -lrt -lm
HPDEMO_LIB_TARGET   = hpdemo2.so
HPDEMO_LIB_SOURCES  = hpdemo2.c
HPDEMO_LIB_INCLUDES =

all: $(HPDEMO_LIB_TARGET)

$(HPDEMO_LIB_TARGET): $(HPDEMO_LIB_SOURCES) $(HPDEMO_LIB_INCLUDES) 
	$(CC) -o $(HPDEMO_LIB_TARGET) $(HPDEMO_LIB_SOURCES) $(HPDEMO_LIB_CCFLAGS)

tags:
	ctags -R .

clean: 
	rm -f $(HPDEMO_LIB_TARGET) tags

prefix=/usr/local
LIBDIR=$(prefix)/lib
BINDIR=$(prefix)/bin
install-lib: $(HPDEMO_LIB_TARGET)
	mkdir -p "$(DESTDIR)$(LIBDIR)"
	install -p $^ "$(DESTDIR)$(LIBDIR)"
install: install-lib

.PHONY: all tags clean install install-lib
# vi: set ts=8 noet :
```
#Compiling
Type this into command line/terminal to create a shared object
This builds the plugin (i.e. the default target defined in the Makefile).
```
make
```
#Installing
Installs the plugin in the hashpipe plugin directory.
```
make install
```
#Running
Run this in the command line/terminal:
```
hashpipe -p hpdemo2 hpdemo2_thread
```
In a new terminal window, run this script(it should be a part of the repository you cloned from github):
```
hashpipe_status_monitor.rb
```
You should see the status monitor pop up with a counter. 

![](status_monitor.png)

