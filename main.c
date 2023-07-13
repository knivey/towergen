#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

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
    int starting_rows = rows;
    int fg = 0;
    int bg = 88;
    int fg_backup = fg;
    int bg_backup = fg;
    const int left_side_cols = 30;
    const int right_side_cols = 30;
    const int tower_width = 10;
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
    while (rows > 0) {
        bzero(buffer, 1024);
        while(infile && !feof(infile) && strcmp(buffer, "") == 0) {
            fgets(buffer, 1024, infile);
        }
        // left side
        char *test_str = buffer;
        int test_str_i = 0;
        int current_color = 98;
        // generate left side 'sky' and code or whatever file you want to read in
        // one idea is that we should have magical words or something
        fg = 0;
        // this allows us to have a gradient of colors
        // every mod rows we shift color by 1
        bg = ((bg+1) * (rows % 8==0 && rows < starting_rows)) + 
              (bg *    (rows % 8 || rows >= starting_rows));
        // to make sure we dont go past 98, this will go back to 88 when we hit it
        bg = (bg * (bg <= 98)) + (88 * (bg > 98)); 
        printf("\x03%02d,%02d", fg, bg);
        for (int i = 0; i < left_side_cols; i++) {
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
        for(int i=0; i<10; i++) {
        //while (current_color > 88) {
            // generate a window
            // branch-less technique
            int exp = i==4 && rows < starting_rows && rows % 4 == 0;
            fg = (88 * exp) + (current_color * !exp);
            bg = (88 * exp) + (current_color * !exp);
            printf("\x03%02d,%02d", fg, bg);
            putchar(' ');
            current_color--;
            test_str_i++; // for incrementing through our file buffer
        }
        // right side
        fg = fg_backup;
        bg = bg_backup;
        printf("\x03%02d,%02d", fg, bg);
        for (int i = 0; i < right_side_cols; i++) {
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

