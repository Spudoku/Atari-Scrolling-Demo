#include <stdio.h>
#include <unistd.h>


#include <atari.h>    // the real meat and potatoes
#include <joystick.h> // not really necessary right now but...

/*
    Start Display List Schenanigans
    
*/
#define HORIZ_WIDTH 32

#define UPDATE_DELAY 1
unsigned char oneRow[HORIZ_WIDTH * 2] = 
{
0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,20,21,22,23,24,25,26,27,28,29,30,31,
0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,20,21,22,23,24,25,26,27,28,29,30,31,
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
    unsigned int i;
    *(unsigned int *)&DisplayList[sizeof(DisplayList) - 2] 
          = (unsigned int)DisplayList;


    //hscroll does mean that we cant get to the furthest left part of the screen
    ANTIC.hscrol = 0; 
    ANTIC.vscrol = 0;


    OS.sdlst = &DisplayList;

    // note: this is pretty expensive!
    for (i = 0; i < 12; ++i) {
        *(unsigned int *)(DisplayList + 4 + (i * 3)) = (unsigned int)(oneRow);
    }  
}




/*
    End Display List stuff
*/


// need to allocate displayList
int main() {
    signed char fine_horiz_offset = 0;
    unsigned int coarse_horiz_offset = 0;
    unsigned int i;

    dl_Init();
    while (1) {
        
        

        if (--fine_horiz_offset < 0) {
            // move coarsely
            fine_horiz_offset = 7;

            if (++coarse_horiz_offset >= HORIZ_WIDTH) {
                coarse_horiz_offset = 0;   
            }        
        }
        
        for (i = 0; i < UPDATE_DELAY; i++) {
            waitvsync();
        }
        
        ANTIC.hscrol = fine_horiz_offset;

        for (i = 0; i < 12; ++i) {
            *(unsigned int *)(DisplayList + 4 + (i * 3)) = (unsigned int)(oneRow) + coarse_horiz_offset + i;
        }  
        
        
    }
}