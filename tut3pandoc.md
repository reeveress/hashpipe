#Hashpipe Tutorial 3
Now that you've familiarized yourself with the basic hashpipe functions, it is time to receive some UDP packets.
Make sure to alter Makefile so the c source code and share object file names are hpdemo3.c and hpdemo3.so, respectively. 
```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
//You must include the hashpipe header file to access hashpipe definitions
#include "hashpipe.h"
#define PKTSIZE 10000
//defining a struct of type hashpipe_udp_params as defined in hashpipe_udp.h
static struct hashpipe_udp_params params;
static int init(hashpipe_thread_args_t * args)
{
        hashpipe_status_t st = args->st;
        strcpy(params.bindhost,"127.0.0.1");
        //selecting a port to listen to
	params.port = 1500;
        params.packet_size = 0;
	hashpipe_udp_init(&params);
        hashpipe_status_lock_safe(&st);
        hputi4(st.buf, "NPACKETS", 0);
        hputi4(st.buf, "NBYTES", 0);
        hashpipe_status_unlock_safe(&st);
        return 0;

}

//The run function now uses a while loop to continuously receive packets from
//localhost(the .bindhost "127.0.0.1" address in the init function
static void *run(hashpipe_thread_args_t * args)
{
	hashpipe_status_t st = args->st;
        int npackets = 0;
        int nbytes = 0;
        char data[PKTSIZE];
        int n;
        while (1){

                n = recvfrom(params.sock,data,PKTSIZE,0,0,0);
                //recvfrom return -1 when no data is received, otherwise it
                //receives the byte size of the packet
                if (n == -1){
                        sleep(1);
                }else{

                        npackets++;
                        nbytes += n;
                        hashpipe_status_lock_safe(&st);
                        //adding fields into the status buffer
			hputi4(st.buf, "NPACKETS", npackets);
                        hputi4(st.buf, "NBYTES", nbytes);
                        hashpipe_status_unlock_safe(&st);
                }
        }
}
```
#Sending Packets
There are various ways that you can send packets in localhost. One way(and probably the simplest) is to download netcat if you don't already have it installed and run this line in terminal:
```
echo foo | nc -u -w 1 127.0.0.1 1500
```
-u refers to the UDP nature of packets
-w makes sure that netcat doesn't wait for response before returning to the prompt

Another way is to send it through ipython socket:
```
In [1]: import socket

In [2]: sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

In [3]: addr = ("127.0.0.1",1500)

In [5]: sock.sendto("hello", addr)
```
Now if you run hashpipe_status_monitor.rb script in another terminal you should get the amount of packets that were sent over the socket.


