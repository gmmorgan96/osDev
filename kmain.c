/* The C function */
#define FB_GREEN 0
#define FB_DARK_GREY 15
#include "io.h"
/* The I/O ports */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5
/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{   char *fb = (char *) 0x000B8000;
    fb[i] = c;
    fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

void fb_write_cell_two(unsigned int i, char c, unsigned char fg, unsigned char bg)
{   char *fb = (char *) 0x000B8002;
    fb[i] = c;
    fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

// int sum_of_three(int arg1, int arg2, int arg3)
// {
//     return arg1 + arg2 + arg3;
// }

/** fb_move_cursor:
* Moves the cursor of the framebuffer to the given position
*
* @param pos The new position of the cursor
*/
void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos & 0x00FF);
}

void write(char *buf, unsigned int len){
    // writes to screen at point 
    char *fb = (char *) 0x000B8000; 
    for(unsigned int t = 0; t < len; t++){
        fb[0] = buf[t];
        fb[0 + 1] = ((FB_GREEN & 0x0F) << 4) | (FB_DARK_GREY & 0x0F);
        fb += 0x00000002;
    }
    fb_move_cursor(len--);
//     fb[i] = c;
//     fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
    fb = (char *) 0x000B8108;
    fb[0] = 'B';
    fb[0 + 1] = ((FB_GREEN & 0x0F) << 4) | (FB_DARK_GREY & 0x0F);
}

void cls(){
    char *fb = (char *) 0x000B8000; 
    for(unsigned int t = 0; t < 2000; t++){
        fb[0] = ' ';
        fb[0 + 1] = ((FB_GREEN & 0x0F) << 4) | (FB_DARK_GREY & 0x0F);
        fb += 0x00000002;
    }
    fb_move_cursor(0);
}

void entry(){
    // fb_move_cursor(0);
    // fb_write_cell(0, 'A', FB_GREEN, FB_DARK_GREY);
    // fb_move_cursor(1);
    // fb_write_cell_two(0, 'B', FB_GREEN, FB_DARK_GREY);
    cls();
    char test[] = "GHOST OS v0.0.0.1";
    write(test, sizeof(test));
    fb_move_cursor(189);
    while(1);
}



