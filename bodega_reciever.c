#include<stdio.h>
#include<stdlib.h>
#include <X11/Xlib.h> // Every Xlib program must include this
#include <assert.h>   // I include this to test return values the
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#define NIL (0)       // A name for the void pointer
#define STEP 8



//PARA COMPILAR
// gcc my-program.c $(pkg-config --cflags --libs x11)
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
  int offset=0;
   int sockfd, newsockfd, portno; 
   socklen_t clilen; 
   char buffer[256]; 
   struct sockaddr_in serv_addr, cli_addr; 
   int n; 
   Display *dpy = XOpenDisplay(NIL);
   assert(dpy);
   int blackColor = BlackPixel(dpy, DefaultScreen(dpy));
   int whiteColor = WhitePixel(dpy, DefaultScreen(dpy));
   if (argc < 2) {
     fprintf(stderr,"ERROR, no port provided\n");
     exit(1);
   }
   Colormap screen_colormap;     /* color map to use for allocating colors.   */

   XColor red, brown, blue, yellow, green;
   /* used for allocation of the given color    */
   /* map entries.                              */
   Status rc;
   // Create the window
   Window w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0 , 0,
				  100*STEP,30*STEP , 0, blackColor, whiteColor);
   // We want to get MapNotify events
   XSelectInput(dpy, w, StructureNotifyMask);
  // "Map" the window (that is, make it appear on the screen)
  XMapWindow(dpy, w);
  // Create a "Graphics Context"
  GC gc = XCreateGC(dpy, w, 0, NIL);
  // Tell the GC we draw using the white color
  XSetForeground(dpy, gc, blackColor);
  // Wait for the MapNotify event
  for(;;) {
    XEvent e;
    XNextEvent(dpy, &e);
    if (e.type == MapNotify)
      break;
  }
  XDrawRectangle(dpy, w, gc, STEP-1, STEP , 98*STEP, 26*STEP);
  XSetForeground(dpy, gc, blackColor);

  
  screen_colormap = DefaultColormap(dpy, DefaultScreen(dpy));


  rc = XAllocNamedColor(dpy, screen_colormap, "red", &red, &red);
  if (rc == 0) {
    fprintf(stderr, "XAllocNamedColor - failed to allocated 'red' color.\n");
    exit(1);
  }
  rc = XAllocNamedColor(dpy, screen_colormap, "brown", &brown, &brown);
  if (rc == 0) {
    fprintf(stderr, "XAllocNamedColor - failed to allocated 'brown' color.\n");
    exit(1);
  }
  rc = XAllocNamedColor(dpy, screen_colormap, "blue", &blue, &blue);
  if (rc == 0) {
    fprintf(stderr, "XAllocNamedColor - failed to allocated 'blue' color.\n");
    exit(1);
  }
  rc = XAllocNamedColor(dpy, screen_colormap, "yellow", &yellow, &yellow);
  if (rc == 0) {
    fprintf(stderr, "XAllocNamedColor - failed to allocated 'yellow' color.\n");
    exit(1);
  }
  rc = XAllocNamedColor(dpy, screen_colormap, "green", &green, &green);
  if (rc == 0) {
    fprintf(stderr, "XAllocNamedColor - failed to allocated 'green' color.\n");
    exit(1);
  }


  
  /*SERVER*/
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");
  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *) &serv_addr,
  	   sizeof(serv_addr)) < 0)
    error("ERROR on binding");
  
  listen(sockfd,5);
  XDrawString(dpy, w, gc, 20, 180,"Humidity",8);
  XDrawString(dpy, w, gc, 20, 160,"Temperature",11);
  XDrawString(dpy, w, gc, 20, 140,"Heat Index",10);
  XDrawString(dpy, w, gc, 20, 120,"Moisture",8);
  XSetForeground(dpy, gc, red.pixel);
  XDrawPoint(dpy, w, gc, 10,180);
  XDrawPoint(dpy, w, gc, 11,180);
  XDrawPoint(dpy, w, gc, 12,180);
  XDrawPoint(dpy, w, gc, 13,180);
  XDrawPoint(dpy, w, gc, 14,180);
  XSetForeground(dpy, gc,blackColor);
  XDrawPoint(dpy, w, gc, 10,160);
  XDrawPoint(dpy, w, gc, 11,160);
  XDrawPoint(dpy, w, gc, 12,160);
  XDrawPoint(dpy, w, gc, 13,160);
  XDrawPoint(dpy, w, gc, 14,160);
  XSetForeground(dpy, gc, brown.pixel);
  XDrawPoint(dpy, w, gc, 10,140);
  XDrawPoint(dpy, w, gc, 11,140);
  XDrawPoint(dpy, w, gc, 12,140);
  XDrawPoint(dpy, w, gc, 13,140);
  XDrawPoint(dpy, w, gc, 14,140);
  XSetForeground(dpy, gc, blue.pixel);
  XDrawPoint(dpy, w, gc, 10,120);
  XDrawPoint(dpy, w, gc, 11,120);
  XDrawPoint(dpy, w, gc, 12,120);
  XDrawPoint(dpy, w, gc, 13,120);
  XDrawPoint(dpy, w, gc, 14,120);
  XFlush(dpy);
  while(1){
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
  		       (struct sockaddr *) &cli_addr,
  		       &clilen);
    if (newsockfd < 0)
      error("ERROR on accept");
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");

    float ti,op;
    char *pch;
    printf("> %s\n",buffer);
    FILE *fp;
    fp=fopen("datosbodega.txt", "a+");
    fprintf(fp, "%s\n",buffer);
    fclose(fp);
    pch=strtok(buffer,";");
    printf("> %s\n",pch);
    ti=atoi(pch);
    printf("int> %d\n",ti);
    while ((ti-offset)>350)
      {
	offset=offset+350;
      }


    pch=strtok(NULL,";");
    printf("> %s\n",pch);
    op=atof(pch);
    printf("float> %f\n",op);
    XSetForeground(dpy, gc, red.pixel);
    XDrawPoint(dpy, w, gc, (ti-offset)*2,op);
    XDrawPoint(dpy, w, gc, (ti-offset)*2,op+1);
    XDrawPoint(dpy, w, gc, (ti-offset)*2,op-1);
    XDrawPoint(dpy, w, gc, (ti-offset)*2+1,op);
    XDrawPoint(dpy, w, gc, (ti-offset)*2-1,op);
    

   pch=strtok(NULL,";");
    printf("> %s\n",pch);
    op=atof(pch);
    printf("float> %f\n",op);
    XSetForeground(dpy, gc, blackColor);
    XDrawPoint(dpy, w, gc, (ti-offset)*2,op);
    XDrawPoint(dpy, w, gc, (ti-offset)*2,op+1);
    XDrawPoint(dpy, w, gc, (ti-offset)*2,op-1);
    XDrawPoint(dpy, w, gc, (ti-offset)*2+1,op);
    XDrawPoint(dpy, w, gc, (ti-offset)*2-1,op);



    pch=strtok(NULL,";");
    printf("> %s\n",pch);
    op=atof(pch);
    printf("float> %f\n",op);
    XSetForeground(dpy, gc, brown.pixel);
    XDrawPoint(dpy, w, gc, (ti-offset)*2,(op/2));
    XDrawPoint(dpy, w, gc, (ti-offset)*2,(op/2)+1);
    XDrawPoint(dpy, w, gc, (ti-offset)*2,(op/2)-1);
    XDrawPoint(dpy, w, gc, (ti-offset)*2+1,(op/2));
    XDrawPoint(dpy, w, gc, (ti-offset)*2-1,(op/2));

    
 

    
    pch=strtok(NULL,";");
    printf("> %s\n",pch);
    op=atof(pch);
    printf("float> %f\n",op);
    XSetForeground(dpy, gc, blue.pixel);
    XDrawPoint(dpy, w, gc, (ti-offset)*2,op/6);
    XDrawPoint(dpy, w, gc, (ti-offset)*2,(op/6)+1);
    XDrawPoint(dpy, w, gc, (ti-offset)*2,(op/6)-1);
    XDrawPoint(dpy, w, gc, (ti-offset)*2+1,(op/6));
    XDrawPoint(dpy, w, gc, (ti-offset)*2+-1,(op/6));
    XFlush(dpy);

    close(newsockfd);

        printf("\n\n___________________\n\n");
  }
  close(sockfd);

  /*  SERVER OFF*/

  /*COSENO*/


    /* while(i<99*STEP) */
  /*   { */
  /*     XDrawPoint(dpy, w, gc, i, (int)(80*cos(50*STEP*i)+40)); */
  /*     printf("> %d %d\n",i, (int)(80*cos(50*STEP*i)+40)); */
  /*     i=i+1; */
  /*     XFlush(dpy); */
  /*     usleep(200000); */

  /*   } */
    XFlush(dpy);
  sleep(99);
}
