#include <stdio.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <sys/time.h>

#include <math.h>
#include <malloc.h>

//#include <string.h>

/*
TODO: fps counter
*/

float width=800,height=800,x=0,y=0,lipos[4]={2,2,1,0},matrix[16];

unsigned int selbuf[512];
float afps;
struct timeval tv,tvo,check;


int mode=0;
#include "utils.c"

#include "labyrinth.c"
//GUI
void button_(float la){
	glBegin(7);
	glColor3f(0.6,0.6,0);
	glVertex2f(-100,la-50);
	glVertex2f(100,la-50);
	glColor3f(1,1,0.6);
	glVertex2f(100,la+50);
	glVertex2f(-100,la+50);
	glEnd();
}

void draw(){

	switch(mode){
	case 0:
		button_(0); //start
		button_(-110); //options
	break;
	case 2:
		map();
	break;
	}
}


int main()
{
	Display *dpy;
	Window root;
	int att[]={GLX_RGBA,GLX_DEPTH_SIZE,24,GLX_DOUBLEBUFFER,None};
	XVisualInfo *vi;
	Colormap cmap;
	XSetWindowAttributes swa;
	Window win;
	GLXContext glc;
	XWindowAttributes gwa;
	XEvent xev;
	char *buf;

	buf=malloc(1500);
	struct texture tex;
	int viewport[4];

	dpy = XOpenDisplay(0);

	if(!dpy){
		printf("XOpenDisplay failed\n");
		return -1;
	}
	root=DefaultRootWindow(dpy);
	vi=glXChooseVisual(dpy,0,att);
	if(!vi){
		printf("glXChooseVisual failed\n");
	}

	cmap=XCreateColormap(dpy,root,vi->visual,AllocNone);

	swa.colormap=cmap;
	swa.event_mask=ExposureMask|KeyPressMask|ButtonPressMask|ButtonReleaseMask|PointerMotionMask;
	win=XCreateWindow(dpy,root,0,0,width,height,0,vi->depth,InputOutput,vi->visual,CWColormap|CWEventMask,&swa);
	XMapWindow(dpy,win);
	XGetWindowAttributes(dpy,win,&gwa);
	glViewport(0,0,gwa.width,gwa.height);

	glc=glXCreateContext(dpy,vi,NULL,GL_TRUE);
	glXMakeCurrent(dpy,win,glc);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-gwa.width/2,gwa.width/2,-gwa.height/2,gwa.height/2,0,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	sprintf(buf,"%s",glGetString(GL_VERSION));
	XStoreName(dpy,win,buf);

	glGetIntegerv(GL_VIEWPORT,viewport);
	glColor3ub(255,255,255);
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//	glLightfv(GL_LIGHT0,GL_POSITION,lipos);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexEnvf(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	tex=read_texture("img/human.png");
printf("%i %i %i\n",tex.width,tex.height,tex.data[0]);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,tex.width,tex.height,0,GL_RGB,GL_UNSIGNED_BYTE,tex.data);
	glEnable(GL_TEXTURE_2D);
//	glEnable(GL_DEPTH_TEST);
	xev.type=Expose;
	XSendEvent(dpy,win,0,ExposureMask,&xev);
	gettimeofday(&tv,0);
	while(1){
		while (XPending(dpy) > 0){
			XNextEvent(dpy,&xev);
			if(xev.type == KeyPress){
				if(xev.xkey.type==KeyPress){
					switch(xev.xkey.keycode){
					case 9:
						glXMakeCurrent(dpy,None,0);
						glXDestroyContext(dpy,glc);
						XDestroyWindow(dpy,win);
						XCloseDisplay(dpy);
						free(buf);
						return 0;
					}
				}
			}else if(xev.type == MotionNotify){
			}else if(xev.type == ButtonPress){
				if(xev.xbutton.button == Button1){
					glPushMatrix();
					glSelectBuffer(512,selbuf);
					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
					gluPickMatrix(xev.xbutton.x,viewport[3]-xev.xbutton.y,1,1,viewport);
					glOrtho(-gwa.width/2,gwa.width/2,-gwa.height/2,gwa.height/2,0,1);
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					glRenderMode(GL_SELECT);
					glInitNames();
					glPushName(1);
					button_(0);
					glPopName();
					glPushName(2);
					button_(-110);
					glPopName();
					
					if(glRenderMode(GL_RENDER)>0){
						sprintf(buf,"%i",selbuf[3]);
						if(selbuf[3]==1)
							mode=2; //start game
						if(selbuf[3]==2)
							mode=2; //options menu
					}else{
						sprintf(buf,"-");
					}
					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
					glOrtho(-gwa.width/2,gwa.width/2,-gwa.height/2,gwa.height/2,0,1);
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					XStoreName(dpy,win,buf);
					glPopMatrix();
				}else if(xev.xbutton.button == Button3){ //RMB
				}else if(xev.xbutton.button == Button4){ //scroll
				}else if(xev.xbutton.button == Button5){ //scroll
				}
			}else if(xev.type == ButtonRelease){
				if(xev.xbutton.button == Button3){
				}
			}
		}
		gettimeofday(&tv,0);
		afps=(tv.tv_usec-tvo.tv_usec)/1000000.0+(float)(tv.tv_sec-tvo.tv_sec);
		tvo.tv_sec=tv.tv_sec;
		tvo.tv_usec=tv.tv_usec;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw();
		glFinish();
		glXSwapBuffers(dpy, win);
	}
	return 0;
}

