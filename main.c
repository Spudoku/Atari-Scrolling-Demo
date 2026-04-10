#include <stdio.h>
#include <unistd.h>


#include <atari.h>    // the real meat and potatoes
#include <joystick.h> // not really necessary right now but...

/*
    Start Display List Schenanigans
    
*/
#define HORIZ_WIDTH 32
#define VERT_HEIGHT 12

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

// TODO: add colors
void init_colors() {

}



/*
    End Display List stuff
*/


// scrolling helpers
void horiz_scroll(signed char dir);

void vert_scroll(signed char dir);

// joystick helpers
unsigned char joystickState = 0;

//extra grandular masks that atari.h doesnt have
//used for speeding up input processing
// written suspiciouslyBee
#define JOY_UPDOWN_MASK 0x03

#define JOY_UPDOWN(v) ((v) & JOY_UPDOWN_MASK)

#define JOY_LEFTRIGHT_MASK 0x0C

#define JOY_LEFTRIGHT(v) ((v) & JOY_LEFTRIGHT_MASK)


// variable declarations
    signed char fine_horiz_offset = 0;
    signed int coarse_horiz_offset = 0;

    signed char fine_vert_offset = 0;
    signed int coarse_vert_offset = 0;

// need to allocate displayList
int main() {

    unsigned int i;
    signed char horiz_dir = 1;
    signed char vert_dir = 1;

    int target_line;

    // initializing
    dl_Init();
    init_colors();
    joy_install(&atrstd_joy);  
    while (1) {
        
        // TODO: read joystick input
        joystickState = joy_read(JOY_1);
        switch(JOY_LEFTRIGHT(joystickState)){
            case JOY_LEFT_MASK:
                horiz_dir = -1;
                break;
            case JOY_RIGHT_MASK:
                horiz_dir = 1;
                break;
            default:
                horiz_dir = 0;
                break;
        }
    
        switch(JOY_UPDOWN(joystickState)){
            case JOY_UP_MASK: 
                vert_dir = 1;
                break;
            case JOY_DOWN_MASK: //down
                vert_dir = -1;
                break;
            default:  //nothing or null cancelled
                vert_dir = 0;
            break;
        }

        horiz_scroll(horiz_dir);
        vert_scroll(vert_dir);
        
        for (i = 0; i < UPDATE_DELAY; i++) {
            waitvsync();
        }
        
        ANTIC.hscrol = fine_horiz_offset;
        ANTIC.vscrol = fine_vert_offset;

        for (i = 0; i < 12; ++i) {
            target_line = i + coarse_vert_offset;
            if (target_line >= 12) target_line -= 12;
            *(unsigned int *)(DisplayList + 4 + ((target_line * 3))) = (unsigned int)(oneRow) + coarse_horiz_offset + i;
        }  
    }
}


// scrolling helpers definitions
void horiz_scroll(signed char dir) {

    if (dir > 0) {
        // scroll left
        if (++fine_horiz_offset > 7) {
            // move coarsely
            fine_horiz_offset = 0;

            if (--coarse_horiz_offset < 0) {
                coarse_horiz_offset = HORIZ_WIDTH;   
            }        
        }
    } else if (dir < 0) {
        // scroll right
        if (--fine_horiz_offset < 0) {
            // move coarsely
            fine_horiz_offset = 7;
            if (--coarse_vert_offset < 0) {
                coarse_vert_offset = VERT_HEIGHT;   
            }   
                   
        }
    }
    // else; no scrolling
}

void vert_scroll(signed char dir) {
    if (dir > 0) {
        // scroll left
        if (++fine_vert_offset > 7) {
            // move coarsely
            fine_vert_offset = 0;

            if (++coarse_horiz_offset >= HORIZ_WIDTH) {
                coarse_horiz_offset = 0;   
            }  
        }
    } else if (dir < 0) {
        // scroll right
        if (--fine_vert_offset < 0) {
            // move coarsely
            fine_vert_offset = 7;

            if (++coarse_vert_offset >= VERT_HEIGHT) {
                coarse_vert_offset = 0;   
            }        
        }
    }
}