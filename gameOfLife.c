#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <wiringPi.h>

#include "Nokia/Nokia.h"
#include "Nokia/Images.h"

#define BUF_SIZE 500

/////////////////////////
// Game of Life functions
/////////////////////////

int** gol_alloc_cells(){
    int** cells = (int **) malloc(HEIGHT*sizeof(int*));
    for(int h=0; h<HEIGHT; h++){
        cells[h] = (int *) malloc(WIDTH*sizeof(int));
        if(cells[h] == 0){
            printf("Error :c could not allocate memory\n");
            exit(-1);
        }
    }
    return cells;
}

void gol_whipe_cells(int**cells){
    for(int h=0; h<HEIGHT; h++)
        for(int w=0; w<WIDTH; w++)
            cells[h][w] = 0;
}

void gol_init_cells_rand(int**cells){
    for(int h=0; h<HEIGHT; h++)
        for(int w=0; w<WIDTH; w++)
            cells[h][w] = rand() & 0x1;
}

int gol_count_neighbours(int**cells, int x, int y){
    int count = 0;
    if(y+1 < HEIGHT) count = (cells[y+1][x])? count+1: count; // Look up
    if(y-1 >= 0) count = (cells[y-1][x])? count+1: count; // Look down
    if(x+1 < WIDTH) count = (cells[y][x+1])? count+1: count; // Look right
    if(x-1 >= 0) count = (cells[y][x-1])? count+1: count; // Look left
    if(y+1 < HEIGHT && x+1 < WIDTH) count = (cells[y+1][x+1])? count+1: count; // Look up right
    if(y-1 >= 0 && x+1 < WIDTH) count = (cells[y-1][x+1])? count+1: count; // Look down right
    if(y+1 < HEIGHT && x-1 >= 0) count = (cells[y+1][x-1])? count+1: count; // Look up left
    if(y-1 >= 0 && x-1 >= 0) count = (cells[y-1][x-1])? count+1: count; // Look down left
    return count;
}

int gol_next_cell_state(int cell_state, int neighbours_count){
    if(neighbours_count >= 4){
        return 0;
    }
    else if(neighbours_count < 2){
        return 0;
    }
    else if(neighbours_count == 3 && cell_state == 0){
        return 1;
    }
    else{
        return cell_state;
    }
}

void gol_compute_step(int**cells, int**temp_cells){
    for(int h=0; h<HEIGHT; h++)
        for(int w=0; w<WIDTH; w++)
            temp_cells[h][w] = gol_next_cell_state(cells[h][w],gol_count_neighbours(cells, w, h));

    for(int h=0; h<HEIGHT; h++)
        for(int w=0; w<WIDTH; w++)
            cells[h][w] = temp_cells[h][w];
}

void gol_draw_screen(int**cells){
    for(int h=0; h<HEIGHT; h++)
        for(int w=0; w<WIDTH; w++)
            gr_setPixel(cells[h][w], w, h);
}

void gol_print_debug(int**cells){
    for(int h=0; h<HEIGHT; h++){
        for(int w=0; w<WIDTH; w++)
            printf("%c", (cells[h][w])? '#':' ');
        printf("\n");
    }
}

void gol_print_debug_nei(int**cells){
    for(int h=0; h<HEIGHT; h++){
        for(int w=0; w<WIDTH; w++)
            printf("%d", gol_count_neighbours(cells,w,h));
        printf("\n");
    }
}
////////////////////////

int main(int argc, char*argv[]) {
    wiringPiSetup();
    lcdInit();

    srand(time(0));
    int **gol_cells = gol_alloc_cells();
    int **temp_cells = gol_alloc_cells();
    gol_init_cells_rand(gol_cells);
    gol_print_debug_nei(gol_cells);

    gol_draw_screen(gol_cells);
    delay(4000);
    while(1){
        gol_compute_step(gol_cells, temp_cells);
        gol_draw_screen(gol_cells);
    }

    return 0;
}
