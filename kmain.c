/* The C function */
#define FB_GREEN 2
#define FB_DARK_GREY 8

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{   char *fb = (char *) 0x000B8000;
    fb[i] = c;
    fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

int sum_of_three(int arg1, int arg2, int arg3)
{
    fb_write_cell(0, 'A', FB_GREEN, FB_DARK_GREY);
    return arg1 + arg2 + arg3;
}

