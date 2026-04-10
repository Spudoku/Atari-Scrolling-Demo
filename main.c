#include <stdio.h>
#include <unistd.h>


#include <atari.h>    // the real meat and potatoes
#include <joystick.h> // not really necessary right now but...

/*
    Start Display List Schenanigans
    
*/

unsigned char oneRow[20] = 
{
0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
};
//  20, 21, 22, 23

// Display list written by suspiciouslyBee
#pragma data-name (push, "DISPLAYLIST")
char DisplayList[] = {
    // 24 blank lines
    DL_BLK8,
    DL_BLK8,
    DL_BLK8,
    
    
    //eventually work up to DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2)))
    //for the fine scrolling
    //LMS will be needed for each dl entry
    //and a mem address for the stream
    
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    //second group of 2

    
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,
    DL_LMS(DL_HSCROL(DL_VSCROL(DL_GRAPHICS2))),
    0x00,0x00,


    //loop the display list
    DL_JVB,
    0x00,0x00 //dl loc
};
#pragma data-name (pop)

void dl_Init(){
    *(unsigned int *)&DisplayList[sizeof(DisplayList) - 2] 
          = (unsigned int)DisplayList;


    //hscroll does mean that we cant get to the furthest left part of the screen
    ANTIC.hscrol = 0; 
    ANTIC.vscrol = 0;


    OS.sdlst = &DisplayList;

    *(unsigned int *)(DisplayList + 4) = (unsigned int)(oneRow);
}




/*
    End Display List stuff
*/


// need to allocate displayList
int main() {
    int test = 0;
    printf("hello world!!!!");
    dl_Init();
    while (1) {
        waitvsync();
        test = (test++) % 16;
        ANTIC.hscrol = test;
    }
    return 0;
}