/* The C function */
#define FB_GREEN 0
#define FB_DARK_GREY 15
#include "io.h"
#include "multiboot.h"
/* The I/O ports */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5
/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15
/* The I/O ports */
/* All the I/O ports are calculated relative to the data port. This is because
* all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
* order, but they start at different values.
*/
#define SERIAL_COM1_BASE 0x3F8 /* COM1 base port */
#define SERIAL_DATA_PORT(base) (base)
#define SERIAL_FIFO_COMMAND_PORT(base) (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base) (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base) (base + 5)
/* The I/O port commands */
/* SERIAL_LINE_ENABLE_DLAB:
* Tells the serial port to expect first the highest 8 bits on the data port,
* then the lowest 8 bits will follow
*/
#define SERIAL_LINE_ENABLE_DLAB 0x80
/** serial_configure_baud_rate:
* Sets the speed of the data being sent. The default speed of a serial
* port is 115200 bits/s. The argument is a divisor of that number, hence
* the resulting speed becomes (115200 / divisor) bits/s.

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

*
* @param com The COM port to configure
* @param divisor The divisor
*/
void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
    outb(SERIAL_LINE_COMMAND_PORT(com),
    SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com),
    (divisor >> 8) & 0x00FF);
    outb(SERIAL_DATA_PORT(com),
    divisor & 0x00FF);
}

#define PIC1_PORT_A 0x20
#define PIC2_PORT_A 0xA0
/* The PIC interrupts have been remapped */
#define PIC1_START_INTERRUPT 0x20
#define PIC2_START_INTERRUPT 0x28
#define PIC2_END_INTERRUPT PIC2_START_INTERRUPT + 7
#define PIC_ACK 0x20
/** pic_acknowledge:
* Acknowledges an interrupt from either PIC 1 or PIC 2.
*
* @param num The number of the interrupt
*/
void pic_acknowledge(unsigned int interrupt)
{
    if (interrupt < PIC1_START_INTERRUPT || interrupt > PIC2_END_INTERRUPT) {
        return;
    }
    if (interrupt < PIC2_START_INTERRUPT) {
        outb(PIC1_PORT_A, PIC_ACK);
    } else {
        outb(PIC2_PORT_A, PIC_ACK);
    }
}

#define KBD_DATA_PORT 0x60
#define KBD_BUFFER_SIZE 512

/* Alphabet */
#define KBD_SC_A        0x1e
#define KBD_SC_B        0x30
#define KBD_SC_C        0x2e
#define KBD_SC_D        0x20
#define KBD_SC_E        0x12
#define KBD_SC_F        0x21
#define KBD_SC_G        0x22
#define KBD_SC_H        0x23
#define KBD_SC_I        0x17
#define KBD_SC_J        0x24
#define KBD_SC_K        0x25
#define KBD_SC_L        0x26
#define KBD_SC_M        0x32
#define KBD_SC_N        0x31
#define KBD_SC_O        0x18
#define KBD_SC_P        0x19
#define KBD_SC_Q        0x10
#define KBD_SC_R        0x13
#define KBD_SC_S        0x1f
#define KBD_SC_T        0x14
#define KBD_SC_U        0x16
#define KBD_SC_V        0x2f
#define KBD_SC_W        0x11
#define KBD_SC_X        0x2d
#define KBD_SC_Y        0x15
#define KBD_SC_Z        0x2c

/* Numeric keys */
#define KBD_SC_1        0x02
#define KBD_SC_2        0x03
#define KBD_SC_3        0x04
#define KBD_SC_4        0x05
#define KBD_SC_5        0x06
#define KBD_SC_6        0x07
#define KBD_SC_7        0x08
#define KBD_SC_8        0x09
#define KBD_SC_9        0x0a
#define KBD_SC_0        0x0b

/* Special keys */
#define KBD_SC_ENTER    0x1c
#define KBD_SC_SPACE    0x39
#define KBD_SC_BS       0x0e
#define KBD_SC_LSHIFT   0x2a
#define KBD_SC_RSHIFT   0x36
#define KBD_SC_DASH     0x0c
#define KBD_SC_EQUALS   0x0d
#define KBD_SC_LBRACKET 0x1a
#define KBD_SC_RBRACKET 0x1b
#define KBD_SC_BSLASH   0x2b
#define KBD_SC_SCOLON   0x27
#define KBD_SC_QUOTE    0x28
#define KBD_SC_COMMA    0x33
#define KBD_SC_DOT      0x34
#define KBD_SC_FSLASH   0x35
#define KBD_SC_TILDE    0x29
#define KBD_SC_CAPSLOCK 0x3a
#define KBD_SC_TAB      0x0f
/** read_scan_code:
* Reads a scan code from the keyboard
*
* @return The scan code (NOT an ASCII character!)
*/
unsigned char read_scan_code(void)
{
    return inb(KBD_DATA_PORT);
}

/** serial_configure_line:
* Configures the line of the given serial port. The port is set to have a
* data length of 8 bits, no parity bits, one stop bit and break control
* disabled.
*
* @param com The serial port to configure
*/
void serial_configure_line(unsigned short com)
{
    /* Bit: | 7 | 6 | 5 4 3 | 2 | 1 0 |
    * Content: | d | b | prty | s | dl |
    * Value: | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
    */
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

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

/** serial_is_transmit_fifo_empty:
* Checks whether the transmit FIFO queue is empty or not for the given COM
* port.
*
* @param com The COM port
* @return 0 if the transmit FIFO queue is not empty
* 1 if the transmit FIFO queue is empty
*/
int serial_is_transmit_fifo_empty(unsigned int com)
{
    /* 0x20 = 0010 0000 */
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

void write_serial(char a){
    while (serial_is_transmit_fifo_empty(SERIAL_COM1_BASE) == 0);
    outb(SERIAL_COM1_BASE, a);
}

void print_f(char *buf, unsigned int len){
    for(unsigned int t = 0; t < len; t++){
        write_serial(buf[t]);
    }
}

struct cpu_state {
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;

    unsigned int esp;
} __attribute__((packed));
struct stack_state {
    unsigned int error_code;
    unsigned int eip;
    unsigned int cs;
    unsigned int eflags;
} __attribute__((packed));
// void interrupt_handler(struct cpu_state cpu, struct stack_state stack, unsigned int interrupt){

// }
void interrupt_handler(){
    char test[] = "GHOST OS Test";
    write(test, sizeof(test));
}
char *fb = (char *) 0x000B8000;
void print_code(unsigned char code){
    fb[0] = code;
    fb[0 + 1] = ((FB_GREEN & 0x0F) << 4) | (FB_DARK_GREY & 0x0F);
}


// GET INPUT AND PRINT TO SCREEN
void input(unsigned char code){
    // blank char array... to keep command in place
    // check for new incoming code... if code print to screen at buffer location
    
    // if (code == 0x1E){
    //     code = 'a';
    //     print_code(code);
    //     fb += 0x00000002;
    // }  
    // if (code == 0x30){
    //     code = 'b';
    //     print_code(code);
    //     fb += 0x00000002;
    // }
    if (code & 0x80) {
        code = 'a';
        print_code(code);
        fb += 0x00000002;
    }
    
    pic_acknowledge(0x20);
    

}

typedef void (*call_module_t)(void);


void entry(unsigned int ebx){
    // fb_move_cursor(0);
    // fb_write_cell(0, 'A', FB_GREEN, FB_DARK_GREY);
    // fb_move_cursor(1);
    // fb_write_cell_two(0, 'B', FB_GREEN, FB_DARK_GREY);
    // serial_configure_baud_rate(SERIAL_COM1_BASE, 2);
    // serial_configure_line(SERIAL_COM1_BASE);
    // serial_is_transmit_fifo_empty(SERIAL_COM1_BASE);
    cls();
    char test[] = "GHOST OS v0.0.0.1";
    write(test, sizeof(test));
    multiboot_info_t *mbinfo = (multiboot_info_t *) ebx;
    unsigned int address_of_module = mbinfo->mods_addr;
    cls();
    write((char*) ebx, sizeof(ebx));
    //write((char*) address_of_module, sizeof(address_of_module));
    call_module_t start_program = (call_module_t) address_of_module;
    start_program();
    // print_f(test, sizeof(test));
    fb_move_cursor(80);
    // unsigned char code;
    // while(1){
    //     code = read_scan_code();
    //     if (code){
    //         input(code);
    //         print_f((char*) &code, sizeof(code));
    //     }
        
    // }
    while(1);

}