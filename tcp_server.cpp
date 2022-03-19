#include<iostream>

#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <sys/socket.h>


#define PORT 8113

void on_read_cb(struct bufferevent * bev, void * ctx){

 // read callback 
 struct evbuffer * input = NULL;
 struct evbuffer * output = NULL; 
 input = bufferevent_get_input(bev);
 output = bufferevent_get_output(bev);
 evbuffer_add_buffer(output,input);
 

}


// define the call back function 
void  on_accept_callback(struct evconnlistener * listener ,
	       	        	evutil_socket_t fd , 
				struct sockaddr * addr,
			       	int socklen,
				void * ctx)
{
	/*We got a new connectio, set up the buffer event for it */
	struct bufferevent * bev = NULL;
	struct event_base * base = NULL;
	base  = evconnlistener_get_base(listener);
	bev = bufferevent_socket_new(base,fd,0);
	
	// enable the the event type for the bufferevent
	bufferevent_enable(bev,EV_READ|EV_WRITE);

	// define the call back function
	bufferevent_setcb(bev,on_read_cb,NULL,NULL,NULL);


	
	

}


int main(int argc, char * argv[]){
    
    struct evconnlistener * listener = nullptr; 
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = INADDR_ANY;
  



    
    struct event_base * base =  event_base_new();
    listener = evconnlistener_new_bind( base,
		    	    on_accept_callback,
			    NULL,
			    LEV_OPT_REUSEABLE,
			    10,
			    (struct sockaddr*)&serveraddr,
			    sizeof(serveraddr)		 
		    );
    event_base_dispatch(base);
    


    


    return 0 ; 
}	
