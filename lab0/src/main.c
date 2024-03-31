#include <stdio.h>

enum { max_input_len = 13, 
       max_output_len = 52, /* (f -> 1111) if 13 "f" => 4 * 13 = 52("1") */
       num_char_after_dot = 12 }; 

typedef struct part_num {
    char part[max_input_len];
    short len;
    char is_fract;
    short num_sys;
} PART_NUM;

char correct_ns(const PART_NUM* num) {
    const char alf[] = "0123456789ABCDEF";
    for (short i = 0; i < num->len; ++i) {
        if ( (('0' <= num->part[i]) && (num->part[i] <= '9')) || (('A' <= num->part[i] ) && (num->part[i] <= 'F')) ) {
            if (num->part[i] > alf[num->num_sys - 1]) return 0;
        }
        else return 0;
    }
    return 1;
}

void small_let_to_capital(PART_NUM* num) {
    short ASCII_dist_to_capital_let = 'a' - 'A';
    for (short i = 0; i < num->len; ++i)
        if (num->part[i] >= 'a') num->part[i] -= ASCII_dist_to_capital_let;
}

int char_to_int(char c) {
    return (c <= '9') ? (c - '0') : (c - 'A' + 10);
}

long double str_decoding(const PART_NUM* num) {
    long double res = 0;
    if (num->is_fract) {
        for (short i = num->len - 1; i >= 0; --i)
            res = res / num->num_sys + char_to_int(num->part[i]);
        res /= num->num_sys;
    }
    else
        for (short i = 0; i < num->len; ++i)
            res = res * num->num_sys + char_to_int(num->part[i]);
    return res;
}

void fill_array_end_char(char* ar, short length) { 
    for (short i = 0; i < length; ++i) ar[i] = '\0'; 
}

void print_num_in_new_ns(long double num, char is_fract_part, short new_ns) {
    const char alf[] = "0123456789ABCDEF";
    char ar[max_output_len];
    fill_array_end_char(ar, max_output_len);

    if (is_fract_part) {
        for (short i = 0; i < num_char_after_dot; i++) {
            putchar(alf[(int)(num *= new_ns)]);
            num -= (int)num;
        }
    }
    else {
        if (num == 0) putchar('0');
        else {
            long long num_int = num;
            short i = 0;
            while(num_int != 0) {
                ar[i] = alf[num_int % new_ns];
                num_int /= new_ns;
                ++i;
            }
            for(i = 0; i < max_output_len; ++i)
                if(ar[max_output_len - 1 - i] != '\0') putchar(ar[max_output_len - 1 - i]);
        }
    }
}

int main() {
    int orig_ns, new_ns;
    char bad_input = 0, dot_is_print = 0;
    if (scanf("%d %d", &orig_ns, &new_ns) != 2) bad_input = 1;

    PART_NUM int_part = {.is_fract = 0, orig_ns};
    PART_NUM fract_part = {.is_fract = 1, orig_ns};
    fill_array_end_char(int_part.part, int_part.len);
    fill_array_end_char(fract_part.part, int_part.len);

    char x = getchar(); /* we read the garbage "\n" so that it does not interfere in the next cycle */
    short i = 0;
    while (1) {
        x = getchar();
        if(i == 0 && (x == '.' || x == '\n')) bad_input = 1;
        if(x == '.') {
            dot_is_print = 1;
            break;
        }
        if (x != '\n') {
            int_part.part[i] = x;
            ++i;
        }
        else break;
    }
    int_part.len = i;
    i = 0;
    while (dot_is_print && (x = getchar()) != '\n') {
        fract_part.part[i] = x;
        ++i;
    }
    fract_part.len = i;

    small_let_to_capital(&int_part);
    small_let_to_capital(&fract_part);

    if (bad_input || (fract_part.part[0] == '\0' && dot_is_print) ||\
    !(((2 <= orig_ns) && (orig_ns <= 16)) && ((2 <= new_ns) && (new_ns <= 16))) ||\
    !(correct_ns(&int_part) && correct_ns(&fract_part))) {
        printf("bad input");
        return 0;
    }

    print_num_in_new_ns(str_decoding(&int_part), int_part.is_fract, new_ns);
    if (dot_is_print) {
        putchar('.');
        print_num_in_new_ns(str_decoding(&fract_part), fract_part.is_fract, new_ns);
    }

    return 0;
}