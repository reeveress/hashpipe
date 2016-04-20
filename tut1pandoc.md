#Hashpipe Tutorial 1
HASHPIPE is a GUPPI derivative that extends the functionality of the original GUPPI architecture. Hashpipe allows to pass thread options in command line, construct the pipeline at runtime, specify processor affinities and get real time information from the status buffer that has direct access to thread memory buffers. Status Monitor offers a great way to check the status of running threads.


- Network Thread - receives and collates 10GbE packets from F engines
- Fluffing Thread - reorders and 'fluffs' 4 bit data to 8 bit data(since GPU accepts 8 bit data)
- GPU Thread - passes data to/from xGPU library
- Output Thread - reorders and sends visibility data to catcher

#Software prerequisites
Make sure you have HASHPIPE installed from [github](https://github.com/david-macmahon/hashpipe).

#Writing your first plugin
To start your first HASHPIPE plugin, you must first have a Makefile in your directory. Makefile tells make how to compile and link a program. The plugin Makefile will build a shared object file from the plugin script.c. For more information refer to gnu make [documentation](https://www.gnu.org/software/make/manual/html_node/Introduction.html#Introduction). 
This is what our Makefile will look like:

```makefile
CC          = gcc
HPDEMO_LIB_CCFLAGS     = -g -O3 -fPIC -shared -lstdc++ -mavx -msse4 \
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
The plugin itself is a c script:

```c


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
To compile the file and create a share object use the command 
```bash
hashpipe -p hpdemo hpdemo_thread
```
hpdemo after the -p option specifies the plugin name followed by the thread name as specified in the thread descriptor's 'name' field. The output will look something like this: 
```
initing  thread 'hpdemo_thread' with databufs 0 and 1
starting thread 'hpdemo_thread' with databufs 0 and 1
Hello World!
Joined thread 'hpdemo_thread'
```


