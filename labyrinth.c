void human(){
 glBegin(7);
 glColor3f(1,1,1);
 glTexCoord2f(0,0);
 glVertex2f(-16,-16);
 glTexCoord2f(1,0);
 glVertex2f(16,-16);
 glTexCoord2f(1,1);
 glVertex2f(16,16);
 glTexCoord2f(0,1);
 glVertex2f(-16,16);
 glEnd();
}

void map(){
 glBegin(1);
 glColor3f(1,0,0);
 glVertex2f(-100,100);
 glVertex2f(100,100);
 glEnd();
 human();
}

