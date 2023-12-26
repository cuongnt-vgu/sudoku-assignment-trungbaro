#include "hidden_singles.h"

int find_hidden_single_values(Cell **p_cells, int* hidden_single_values) {
    int num_hidden_values = 0;
    
    for (int i = 0; i <= BOARD_SIZE; i++) {
        int counter = 0;
        
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (p_cells[j]->fixed || p_cells[j]->num_candidates == 1) {
                continue;
            }
            counter += is_candidate(p_cells[j], i);
        }
        
        if (counter == 1) {
            hidden_single_values[num_hidden_values++] = i;
        }
    }
    
    return num_hidden_values;
}

void find_hidden_single(Cell** p_cells, HiddenSingle* p_hidden_singles, int* p_counter) {
    int* single_values = (int*) malloc((BOARD_SIZE + 1) * sizeof(int));
    int number_hidden_values = find_hidden_single_values(p_cells, single_values);
    
    for (int i = 0; i < number_hidden_values; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (p_cells[j]->fixed) {
                continue;
            }

            bool check = true;
            
            for (int k = 0; k < *p_counter; k++) {
                if (p_cells[j] == p_hidden_singles[k].p_cell) {
                    check = false;
                    break;
                }
            }
            
            if (!check) {
                continue;
            }
            
            if (is_candidate(p_cells[j], single_values[i])) {
                p_hidden_singles[*p_counter].p_cell = p_cells[j];
                p_hidden_singles[*p_counter].value = single_values[i];
                (*p_counter)++;
            }
        }
    }
    
    free(single_values);
}

int hidden_singles(SudokuBoard *p_board) {
    HiddenSingle* hidden_singles = (HiddenSingle*) malloc(BOARD_SIZE * BOARD_SIZE * sizeof(HiddenSingle));
    int num_hidden_single = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        find_hidden_single(p_board->p_rows[i], hidden_singles, &num_hidden_single);
        find_hidden_single(p_board->p_cols[i], hidden_singles, &num_hidden_single);
        find_hidden_single(p_board->p_boxes[i], hidden_singles, &num_hidden_single);
    }

    for (int i = 0; i < num_hidden_single; i++) {
        for (int j = 1; j <= BOARD_SIZE; j++) {
            if (is_candidate(hidden_singles[i].p_cell, j)) {
                unset_candidate(hidden_singles[i].p_cell, j);
            }
        }
        set_candidate(hidden_singles[i].p_cell, hidden_singles[i].value);
    }

    free(hidden_singles);
    return num_hidden_single;
}