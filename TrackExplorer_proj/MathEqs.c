// MathEqs.c
// Runs on TM4C123
// By Kevin Martinez
/*
Matheqs files are dedicated approximation methods. Matheqs aims to solve the issue of approximating IR sensor measurements
without a constant equation, as sharp IR sensors datasheet does not include a constant equation. The function of interest 
is CalcDist. CalcDist uses ADC output values and turns them into readable cm values. In order to accomplish this 
approximation with <= 16 Mhz clock speed we need optimal time complexity otherwise approximations may be computationally 
to intensive and the car will crash before the car can adjust. To achieve optimal time complexity we need to use a combination 
of binary search and linear vector approximation. 
If TM4C123 is clocked at a higher speed (>= 32 Mhz) Lagrange polynomail method can be used. Lagrange polynomail is more accurate, 
however more computationally more intensive and you may need to take into account a few more edge cases.
*/

#include "MathEqs.h"
// IR sensor measurements
uint16_t x_ir[6] = {IR09CM, 	IR15CM, IR20CM, IR30CM, IR40CM, IR80CM}; 			// x points that represent the IR voltage values
uint16_t y_dist[6] = {9, 		    15,     20,     30,     40,     80}; 		// y points that represent distance
char n_pts = sizeof(x_ir)/sizeof(x_ir[0]);  							// size of x and y arr
/*
// T: O(n^2) M: O(1)    Complexity, where n is len of x & y arr
double LagrangePoly(uint16_t x[], uint16_t y[], double fx, char n){
    double poly = 0;
    for (char i = 0; i < n; i++){   // loop over all points
        double L = 1;
        for (char j = 0; j < n; j++){   //loop over all points except i
            if (j != i){    // avoid division by zero
                L *= (fx - x[j]) / (x[i] - x[j]);   //multiply by Lagrange basis polynomial
            }
        }
        poly += y[i] * L; //add weighted term to P
    }
    return poly;
}
*/

// T: O(log n) M: O(1)  Complexity, where n is len of arr
//char* BinSearch_range(uint16_t arr[], uint16_t target, char len_arr, _Bool reverse){
char BinSearch_range(uint16_t arr[], uint16_t target, char len_arr, _Bool reverse){
    char l_idx = 0, r_idx = len_arr - 1;
    while (l_idx + 1 < r_idx){
        // midpoint = left_idx + ((total dist) // 2) = left_idx + half dist
        char mid = l_idx + ((r_idx - l_idx) / 2);
        //char mid = (l_idx + r_idx) / 2;   // can lead to overflow if l & r are to big
        if (arr[mid] < target && reverse == 0) l_idx = mid;
        else if (arr[mid] > target && reverse == 1) l_idx = mid;
        else r_idx = mid;
    }
    //char *clsts_nbors = malloc(2 * sizeof(char));
    //clsts_nbors[0] = l_idx, clsts_nbors[1] = r_idx;
    return l_idx;
}
// T: O(log n) M: O(n) Complexity, where n is len of arr
uint16_t CalcDist(uint16_t adc_out){
    // if IR adc output out of range return dist constants
    if (adc_out <= IR80CM * 1.02) return 80;
    else if(adc_out >= IR09CM * .98) return 9; 
    // Search for 2 closest points of adc_out using Binary Search
    //char *idxs_range = BinSearch_range(x_ir, adc_out, n_pts, 1);
    char l_idx = BinSearch_range(x_ir, adc_out, n_pts, 1);
    char r_idx = l_idx + 1;
    uint16_t dist_range[2] = {y_dist[l_idx], y_dist[r_idx]};
    uint16_t ir_range[2] = {x_ir[l_idx], x_ir[r_idx]};
    //free(idxs_range); 	// deallocate dynamically allocated memory as no longer needed
    // Approximate linear distance between both points
    double range_perc = 1 - (double) (adc_out - ir_range[1]) / (ir_range[0] - ir_range[1]);
    uint16_t dist = dist_range[0] + ((dist_range[1] - dist_range[0]) * range_perc);
    return dist;
}
/*
#include <stdio.h>
void test_CalcDist(void){
    for (int x = IR80CM - 1; x <= IR09CM + 1; x++){
        uint16_t dist = CalcDist(x);
        printf("adc_out: %d, dist: %u\n\n", x, dist);
    }
}
*/
