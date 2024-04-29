
#define STDOUT 1
#define STDIN 0

#define SIZE_OF_BUFFER 20

#define __NR_read 3
#define __NR_write 4
#define __NR_exit_group 252


int is_num(char ch);
int is_spc(char ch);
int sys_read(char *number);
int sys_write(char *number_16);
void sys_exit_group(int result);
static void print_number(unsigned number_int);

int _start(int argc, char *argv[]) {
    char number[20];
    unsigned number_int = 0;
    int number_of_digits = 0;
    unsigned chars_to_process = 0;
    for (int i;; i++, chars_to_process--) {
        if (chars_to_process == 0) {
            int ret = sys_read(number);
            if (ret < 0) { sys_exit_group(1);}
            i = 0;
            chars_to_process = ret;
        }
        if (number_of_digits > 0 && (chars_to_process == 0 || !is_num(number[i]))) {
            print_number(number_int);
            number_of_digits = 0;
            number_int = 0;
        }
        if (chars_to_process == 0 || (!is_spc(number[i]) && !is_num(number[i]))) { sys_exit_group(0);}

        if (is_num(number[i])) {
            number_int = number_int * 10 + number[i] - '0';
            number_of_digits++;
        }
    }
}

int is_num(char ch) { return ch >= '0' && ch <= '9';}

int is_spc(char ch) { return ch == ' ' || ch == '\n';}

void sys_exit_group(int result) { asm volatile ("int $0x80" : : "a"(__NR_exit_group), "b"(result));}

int sys_write(char *number_16) {
    int result;
    asm volatile ("int $0x80" :"=a"(result) :"a"(__NR_write), "b"(STDOUT), "c"(number_16), "d"(SIZE_OF_BUFFER) :"memory");
    return result;
}

int sys_read(char *number) {
    int result;
    asm volatile ("int $0x80" :"=a"(result) :"a"(__NR_read), "b"(STDIN), "c"(number), "d"(SIZE_OF_BUFFER) :"memory");
    return result;
}

static void print_number(unsigned number_int) {
    int idx = 0;
    int quotient, remainder;
    char hex_digits[] = "0123456789abcdef";
    char temp[SIZE_OF_BUFFER];
    char number_16[SIZE_OF_BUFFER];
    if (number_int == 0) {
        number_16[0] = '0';
        number_16[1] = 'x';
        number_16[2] = '0';
        number_16[3] = '\n';
        number_16[4] = '\0';
    }
    else {
        while (number_int > 0) {
            quotient = number_int / 16;
            remainder = number_int % 16;
            temp[idx++] = hex_digits[remainder];
            number_int = quotient;
        }
        temp[idx++] = 'x';
        temp[idx++] = '0';
        for (int i = idx - 1; i >= 0; i--) {
            number_16[idx - 1 - i] = temp[i];
        }
        number_16[idx++] = '\n';
        number_16[idx] = '\0';
    }
    sys_write(number_16);
}