/* Program to perform 1D cellular automaton (CA) computations and to use 1D CA
   to solve the density classification problem.

  Skeleton program written by Artem Polyvyanyy, http://polyvyanyy.com/,
  September 2024, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.
  All included code is (c) Copyright University of Melbourne, 2024.

  Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: [XIAONING TIAN 1441462]
  Dated:     [2024.10.6]
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

/* #DEFINE'S -----------------------------------------------------------------*/
#define SDELIM "==STAGE %d============================\n"   // stage delimiter
#define MDELIM "-------------------------------------\n"    // delimiter of -'s
#define THEEND "==THE END============================\n"    // end message

#define CRTRNC '\r'     // carriage return character
#define NBRHDS 8        // number of possible neighborhoods
#define MAX_LENGTH 100

/* TYPE DEFINITIONS ----------------------------------------------------------*/
typedef char cells_t;            // base type to store states of cells
typedef struct state state_t;           // a cellular automaton state
typedef unsigned char rule_t[NBRHDS];   // an elementary CA update rule function

struct state {                   // a state in a CA is defined by
    cells_t*        clls;        // ... an array of cells and
    state_t*        next;        // ... a link to the next state
};

typedef struct {                 // a run of a CA consists of
    state_t*        init;        // ... the initial state and
    state_t*        curr;        // ... the current state,
} run_t;                         // implemented as a linked list of states


typedef struct {                 // an elementary CA is defined by
    unsigned char   code;        // ... a code of the update rule,
    unsigned int    size;        // ... a number of cells,
    unsigned int    elts;        // ... number of elements to store cells,
    unsigned int    time;        // ... the current time step,
    rule_t          rule;        // ... an update rule function, and
    run_t*          run;         // ... a run of state steps
} CA_t;

/* USEFUL FUNCTIONS ----------------------------------------------------------*/
int  mygetchar(void);
                                        // getchar() that skips
                                        //    carriage returns
run_t* make_empty_list(void);
run_t* insert_at_foot(run_t* list, cells_t* clls);
int convert_code(char ch);
void stage1_processing(CA_t* c, char* neighbor[], int steps);
void printing_char(CA_t *c, cells_t *cells, int k);
void on_off(CA_t *c, int pos, int start);
void stage0_processing(CA_t *c, char* neighbors[]);
void density_classfication(CA_t *c, int pos, int start, int time_step);
void stage2_processing_part1(CA_t* c);
void free_run(run_t* run);
void stage2_processing_part2(CA_t *c, int steps);

/* WHERE IT ALL HAPPENS ------------------------------------------------------*/
int main(int argc, char *argv[]) {


//stage 0
    //the initial neighbors 
    char* neighbors[] = {"000", "001", "010", "011", 
                "100", "101", "110", "111"};
    CA_t* c = malloc(sizeof(CA_t));
    assert(c != NULL);
    stage0_processing(c, neighbors);

    //stage 0 finished

    //stage 1


    //find how many steps i need to do

    int steps;
    scanf("%d", &steps);
    getchar();
    
    stage1_processing(c, neighbors, steps);

    stage2_processing_part1(c);
    stage2_processing_part2(c, steps);


    printf(THEEND);

    free_run(c->run);
    free(c);
    
    return 0;        // algorithms are fun!!!
}

/* USEFUL FUNCTIONS ----------------------------------------------------------*/

// An improved version of getchar(); skips carriage return characters.
// NB: Adapted version of the mygetchar() function by Alistair Moffat
int mygetchar(void) {
    int c;
    while ((c=getchar())==CRTRNC);          // skip carriage return characters
    return c;
}


/* THE END -------------------------------------------------------------------*/

//reference from lectures!!
//https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c
run_t* 
make_empty_list(void){
    run_t* r=malloc(sizeof(run_t));
    assert(r !=NULL);
    r->init = r->curr =NULL;
    return r;
}

//reference from lectures!!
//https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c
run_t* 
insert_at_foot(run_t* list, cells_t* clls){
    state_t* new;
    new=malloc(sizeof(state_t));
    assert(list!=NULL && new !=NULL);
    new->clls=clls;
    new->next=NULL;
    if(list-> init == NULL){
        list->init = list ->curr =new;
    }else{
        list->curr->next =new;
        list->curr = new;
    }
    return list;
}

//init=head  curr=foot

//converting code '.' and '*' to 1 and 0
int
convert_code(char ch){
    if (ch == '.') {
        return 0; 
    } else if (ch == '*') {
        return 1; 
    }
return -1;
}


// stage1
void stage1_processing(CA_t* c, char* neighbor[], int steps) {
    printf(SDELIM, 1); // Print the stage header

    // Print the initial state
    printf("%4d: ", 0);
    for (int j = 0; j < c->size; j++) {
        printf("%c", c->run->init->clls[j]);
    }
    printf("\n");

    
 

    for (int k = 1; k <= steps; k++) {
        cells_t* curr_cells = malloc(sizeof(cells_t) * c->size);
        cells_t* old_cells = c->run->curr->clls;
        //finding the index in left, center and right hand
        for (int index = 0; index < c->size; index++) {
            int l_index = (index - 1 + c->size) % c->size;
            int c_index = index;
            int r_index = (index + 1) % c->size;

            //using convert_code function 
            char neighborhood[4] = {old_cells[l_index], old_cells[c_index], 
                            old_cells[r_index], '\0'};
            int rule_index = (convert_code(neighborhood[0]) * 4) + 
                        (convert_code(neighborhood[1]) * 2) 
                    + convert_code(neighborhood[2]);

            //currrent cells became '*' and'.'
            if (c->rule[rule_index]) {
                curr_cells[index] = '*'; // Turn cell on
            } else {
                curr_cells[index] = '.'; // Keep cell off
            }
        }


    insert_at_foot(c->run, curr_cells);

    //print all of them by printing_char function 
    printing_char(c, curr_cells, k);
    }

    //searching the pos anf start to count on and off
    int pos, start;
    if (scanf("%d, %d", &pos, &start) != 2) {
        printf("Error reading position and start step.\n");
        return;
    }
    while (getchar() != '\n');
    printf(MDELIM);
    on_off(c, pos, start);

    //using time to update current steps
    c->time = steps;

}


//printing all cells line by a 'for loop'
void printing_char(CA_t *c, cells_t *cells, int k) {
    printf("%4d: ", k);
    for (int i = 0; i < c->size; i++) {
        printf("%c", cells[i]);
    }
    printf("\n");
}


// a function can count the number of on and off
void on_off(CA_t *c, int pos, int start) {
    state_t* head = c->run->init;
    int on = 0, off = 0;


    for (int step = 0; step < start; step++) {
        if (!head) break; 
        head = head->next;
    }


    while (head) {
        if (head->clls[pos] == '*') {
            on++;
        } else {
            off++;
        }
        head = head->next;
    }
    
    printf("#ON=%d #OFF=%d CELL#%d START@%d\n", on, off, pos, start);
}

//stage2 function when rule=184

void stage2_processing_part1(CA_t *c) {
    printf(SDELIM,2);
    // calculating m and n
    int n = (c->size - 2) / 2;
    

    //using the code in stage1 

    // Using rule 184
    printf("RULE: 184; STEPS: %d.\n", n);
    printf(MDELIM);
    
    // Set rule 184
    int rule_184 = 184;
    for (int i = 0; i < NBRHDS; i++) {
        unsigned char bit = rule_184 & 1;
        c->rule[i] = bit;
        rule_184 = rule_184 >> 1;
    }
    
    printf("%4d: ", c->time);
    for (int i = 0; i < c->size; i++) {
        printf("%c", c->run->curr->clls[i]);
    }
    printf("\n");
    
    // For n steps
    for (int i = 0; i < n; i++) {
        cells_t* curr_cells = malloc(sizeof(cells_t) * c->size);
        cells_t* old_cells = c->run->curr->clls;
     
        for (int index = 0; index < c->size; index++) {
            int l_index = (index - 1 + c->size) % c->size;
            int c_index = index;
            int r_index = (index + 1) % c->size;

           
            char neighborhood[4] = {old_cells[l_index], old_cells[c_index], 
                                old_cells[r_index], '\0'};
            int rule_index = (convert_code(neighborhood[0]) * 4) +
                             (convert_code(neighborhood[1]) * 2) +
                             convert_code(neighborhood[2]);

            if (c->rule[rule_index]) {
                curr_cells[index] = '*';
            } else {
                curr_cells[index] = '.';
            }
        }

        // Insert the new state into the run list
        insert_at_foot(c->run, curr_cells);

        c->time++;
        printing_char(c, curr_cells, c->time);
    }

}

//stage2 function when rule=232

void stage2_processing_part2(CA_t *c, int steps){
    int m = (c->size - 1) / 2; 

    //  using rule 232
    printf(MDELIM);
    printf("RULE: 232; STEPS: %d.\n", m);
    printf(MDELIM);
    
    // Set rule 232
    int rule_232 = 232;
    for (int i = 0; i < NBRHDS; i++) {
        unsigned char bit = rule_232 & 1;
        c->rule[i] = bit;
        rule_232 = rule_232 >> 1;
    }


    printf("%4d: ", c->time);
    for (int i = 0; i < c->size; i++) {
        printf("%c", c->run->curr->clls[i]);
    }
    printf("\n");
    
    // For m steps
    for (int i = 0; i < m; i++) {
        cells_t* curr_cells = malloc(sizeof(cells_t) * c->size);
        cells_t* old_cells = c->run->curr->clls;
        for (int index = 0; index < c->size; index++) {
            int l_index = (index - 1 + c->size) % c->size;
            int c_index = index;
            int r_index = (index + 1) % c->size;

            char neighborhood[4] = {old_cells[l_index], old_cells[c_index], 
                                    old_cells[r_index], '\0'};
            int rule_index = (convert_code(neighborhood[0]) * 4) +
                             (convert_code(neighborhood[1]) * 2) +
                             convert_code(neighborhood[2]);

            if (c->rule[rule_index]) {
                curr_cells[index] = '*';
            } else {
                curr_cells[index] = '.';
            }
        }

    // Insert the new state into the run list
    insert_at_foot(c->run, curr_cells);
    c->time++;
    printing_char(c, curr_cells, c->time);
    }

    // Counting the on_num and off_num
    int pos1, start1;
    if (scanf("%d, %d", &pos1, &start1) != 2) {
        printf("Error reading position and start step.\n");
        return;
    }
    while (getchar() != '\n'); 
    printf(MDELIM);
    on_off(c, pos1, start1);

    //back to start and counting the on_num
    printf(MDELIM);
    density_classfication(c,pos1,start1,steps);
}


// a function to check the density of classfication
void density_classfication(CA_t *c, int pos, int start, int steps) {
    state_t* back = c->run->init;

    for (int j = 0; j < steps; j++) {
        if (!back || !back->next) {
            printf("Error\n");
            return;
        }
        back = back->next;
    }

    //back to the last state in step1
    state_t* state_at_start1 = c->run->init;
    for (int i = 0; i < steps; i++) {
        if (state_at_start1->next) {
            state_at_start1 = state_at_start1->next;
        }
    }
    
    //printing the state at start1
    printf("%4d: ", steps);
    for (int i = 0; i < c->size; i++) {
        printf("%c", state_at_start1->clls[i]);
    }
    printf("\n");

    int on = 0, off = 0;
    for (int i = 0; i < c->size; i++) {
        if (back->clls[i] == '*') {
            on++;
        } else {
            off++;
        }
    }


    if (on < off) {
        printf("AT T=%d: #ON/#CELLS < 1/2\n", steps);
    } else if(on > off) {
        printf("AT T=%d: #ON/#CELLS > 1/2\n", steps);
    }else{
        printf("AT T=%d: #ON/#CELLS = 1/2\n", steps);
    }
}




//stage 0

void
stage0_processing(CA_t *c, char* neighbors[]){
    printf(SDELIM, 0);
    int s;
    scanf("%d\n", &s);
    int r;
    scanf("%d\n", &r);
    cells_t* cells = malloc((sizeof(cells_t)) * s);
    assert(cells != NULL);
    int i;
    for (i = 0; i < s; i++) {
        cells[i] = mygetchar();
    }
    mygetchar();
    

    //Extract the binary bits and store them in a c->rule array
    int temp = r;

    for (i = 0; i < NBRHDS; i++) {
        unsigned char bit = temp & 1;
        c->rule[i] = bit;
        temp = temp >> 1;
    }
    c->code = r;
    c->size = s;
    c->run = make_empty_list();
    c->run = insert_at_foot(c->run, cells);
    
    printf("SIZE: %d\n", c->size);
    printf("RULE: %d\n", c->code);
    printf(MDELIM);
    for (i = 0; i < NBRHDS; i++) {
        printf("%4s", neighbors[i]);
    }
    printf("\n");


    for (i = 0; i < NBRHDS; i++) {
        printf("%3d ", c->rule[i]);
    }
    printf("\n");
    printf(MDELIM);
    printf("%4d: ",0);
    for(i=0; i< c->size; i++){
        printf("%c", c->run->init->clls[i]);
    }

    printf("\n");


}

// a function to free the state 
void free_run(run_t* run) {
    state_t* current = run->init;
    while (current) {
        state_t* next = current->next;
        free(current->clls); // Free the cells
        free(current);       // Free the state
        current = next;     // Move to the next 
    }
    free(run); 
}
