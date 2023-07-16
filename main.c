#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

void generate_top_rows_empty(int fg, int bg, int total_width);
void generate_top_rows_title(int fg, int bg, int left_side_pad, int right_side_pad, char *tower_title);

int main(int argc, char *argv[]) {
    srand(time(NULL));
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s <rows> [filepath]\n", argv[0]);
        return EXIT_FAILURE;
    }
    // if no filepath, no problem
    char *tower_title = "The Tower Of The Darkmage";
    int rows = atoi(argv[1]);
    double total_rows = rows;
    float starting_rows = rows;
    int fg = 0;
    int bg = 88;
    int fg_backup = fg;
    int bg_backup = fg;
    const int left_side_cols = 30;
    const int right_side_cols = 30;
    const int tower_width = 30;
    const int total_width = left_side_cols + tower_width + right_side_cols;
    size_t tower_title_len = strlen(tower_title);
    const int left_side_pad = (total_width - tower_title_len) / 2;
    const int right_side_pad = total_width - tower_title_len - left_side_pad;
    char *filename = argv[2];
    char buffer[1024] = {0};
    FILE *infile = NULL;
    infile = fopen(filename, "r");
    if (!infile) {
        //fprintf(stderr, "Error: could not open file '%s'\n", filename);
        infile = NULL;
        //return EXIT_FAILURE;
    }
    // generate the top rows
    generate_top_rows_empty(fg, bg, total_width);
    generate_top_rows_title(fg, bg, left_side_pad, right_side_pad, tower_title);
    generate_top_rows_empty(fg, bg, total_width);
    // tower loop
    fg = 0;
    bg = 88;
    double cnt = -1;
    while (rows > 0) {
        cnt++;
        bzero(buffer, 1024);
        while(infile && !feof(infile) && strcmp(buffer, "") == 0) {
            fgets(buffer, 1024, infile);
        }
        // left side
        char *test_str = buffer;
        int test_str_i = 0;
        // generate left side 'sky' and code or whatever file you want to read in
        // one idea is that we should have magical words or something
        fg = 0;
        // figure out the background gradient
        //int base_bg = 88 + (int)floor(cnt/(total_rows/11.0)) % 11;
        double dither = (cnt / (total_rows/11.0)) - 0.5;
        double dither_i = 11;
        dither = modf(dither, &dither_i);
        int base_bg = 88 + (int)dither_i;
        int bg = base_bg;

        printf("\x03%02d,%02d", fg, bg);
        for (int i = 0; i < left_side_cols - (int)round(sin(rows / 9.0) * 5.0); i++) {
            if (base_bg != 98 && rand() < RAND_MAX * dither) {
                if(bg == base_bg) {
                    bg = 88 + (base_bg - 88 + 1) % 11;
                    printf("\x03%02d,%02d", fg, bg);
                }
            } else if (bg != base_bg) {
                bg = base_bg;
                printf("\x03%02d,%02d", fg, bg);
            }
            char c = test_str[test_str_i++];
            // branch-less technique
            int exp = c!='\n' && c != '\0' && c != ' ';
            c = (c * exp) + (' ' * !exp);
            putchar(c);
        }
        // generate the actual tower
        // as a fix, if reading in text to display behind the tower,
        // increment the test_str_i
        // this is the gray-scale tower, colors 98-88
        fg_backup = fg;
        bg_backup = bg;
        for(double i=0; i<tower_width; i++) {
            int current_color = 88 + round(sin(i/12.0)*10.0);
        //while (current_color > 88) {
            // generate a window
            // branch-less technique
            int exp = (i==9 || i == 10) && rows < starting_rows && (rows % 8 == 0 || rows % 8 == 1);
            fg = (88 * exp) + (current_color * !exp);
            bg = (88 * exp) + (current_color * !exp);
            printf("\x03%02d,%02d", fg, bg);
            putchar(' ');
            test_str_i++; // for incrementing through our file buffer
        }
        // right side
        fg = fg_backup;
        bg = bg_backup;
        printf("\x03%02d,%02d", fg, bg);
        for (int i = 0; i < right_side_cols + (int)round(sin(rows / 9.0) * 5.0); i++) {
            if (base_bg != 98 && rand() < RAND_MAX * dither) {
                if(bg == base_bg) {
                    bg = 88 + (base_bg - 88 + 1) % 11;
                    printf("\x03%02d,%02d", fg, bg);
                }
            } else if (bg != base_bg) {
                bg = base_bg;
                printf("\x03%02d,%02d", fg, bg);
            }
            char c = test_str[test_str_i++];
            // branch-less technique
            int exp = c!='\n' && c != '\0' && c != ' ';
            c = (c * exp) + (' ' * !exp);
            putchar(c);
        }
        // old stars, might come back
        /*
        for (int i = 0; i < 10; i++) {
            int r = rand() % 2;
            if (r == 0) {
                putchar(' ');
            } else {
                putchar('*');
            }
        }
        */
        putchar('\n');
        rows--;
    }
    if (infile) {
        fclose(infile);
    }
    return EXIT_SUCCESS;
}


void generate_top_rows_empty(int fg, int bg, int total_width) {
    for (int j=0; j<2; j++) {
        printf("\x03%02d,%02d", fg, bg);
        for (int i=0; i<total_width; i++) {
            putchar(' ');
        }
        putchar('\n');
    }
}


void generate_top_rows_title(int fg, int bg, int left_side_pad, int right_side_pad, char *tower_title) {
    printf("\x03%02d,%02d", fg, bg);
    for (int i=0; i<left_side_pad; i++) {
        putchar(' ');
    }
    printf("%s", tower_title);
    for (int i=0; i<right_side_pad; i++) {
        putchar(' ');
    }
    putchar('\n');
}

