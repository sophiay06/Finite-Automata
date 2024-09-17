#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dfa.h"

DFA* new_DFA(int nstates) {
    DFA *dfa = malloc(sizeof(DFA));
    if (dfa == NULL) {
        perror("Failed to allocate memory for DFA");
        exit(EXIT_FAILURE);
    }

    dfa->num_states = nstates;
    dfa->transition_table =(int**)malloc(nstates * sizeof(int*));
    for (int i=0;i < nstates;i++) {
        dfa->transition_table[i] = (int*)malloc(128 * sizeof(int));
        memset (dfa->transition_table[i], -1, 128 * sizeof(int));
    }
    dfa->initial_state = 0;
    dfa->is_accepting = (bool *)calloc(nstates, sizeof(bool));
    if (dfa->is_accepting == NULL) {
        perror("Failed to allocate memory for accepting states");
        free(dfa);
        exit(EXIT_FAILURE);
    }
    return dfa;
}

void DFA_free(DFA *dfa) {
    for (int i = 0; i < dfa->num_states;i++) {
        free(dfa->transition_table[i]);
    }
    free(dfa->transition_table);
    free(dfa->is_accepting);
    free(dfa);
}

int DFA_get_size(DFA *dfa) {
    return dfa->num_states;
}

int DFA_get_transition(DFA *dfa, int src, char sym) {
    return dfa->transition_table[src][(int)sym];
}

void DFA_set_transition(DFA *dfa, int src, char sym, int dst){
    dfa->transition_table[src][(int)sym] = dst;
}

void DFA_set_transition_all(DFA *dfa, int src, int dst){
    for (int i = 0; i<128; i++) {
        DFA_set_transition(dfa, src, i, dst);
    }
}

void DFA_add_transition_all_but(DFA *dfa, int src, char sym, int dst) {
    for (int i = 0; i<128; i++) {
        if (i != (int)sym) {
            DFA_set_transition(dfa, src, i, dst);
        }
    }
}

void DFA_set_accepting(DFA *dfa, int state, bool value) {
    if (state >= 0 && state<dfa->num_states) {
        dfa->is_accepting[state] = value;
    }
}

bool DFA_get_accepting(DFA *dfa, int state) {
    if (state >= 0 && state<dfa->num_states) {
        return dfa->is_accepting[state];
    }
    return false;
}

bool DFA_execute(DFA *dfa, char *input) {
    int curr_state = dfa->initial_state;

    for (int i = 0;input[i] != '\0'; i++) {
        curr_state = DFA_get_transition(dfa,curr_state,input[i]);
        if (curr_state == -1) {

            return false;
        }
    }
    return DFA_get_accepting(dfa, curr_state);
}

void DFA_repl(DFA *dfa) {
    char input[128];
    while (true) {
        printf("Enter an input (""quit"" to quit): ");
        fgets(input, sizeof(input), stdin);
        // removing newline character if present
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "quit") == 0) {
            break;}
        bool result =DFA_execute(dfa, input);
        if (result) {
            printf("Result for input \"%s\": true\n", input);
        }
        else {
            printf("Result for input \"%s\": false\n", input);
        }
    }
}

// DFA for string containing xyzzy
DFA* DFA_for_contains_xyzzy() {
    DFA *dfa = new_DFA(6);
    DFA_set_transition(dfa, 0, 'x', 1);
    DFA_set_transition(dfa, 1, 'y', 2);
    DFA_set_transition(dfa, 2, 'z', 3);
    DFA_set_transition(dfa, 3, 'z', 4);
    DFA_set_transition(dfa, 4, 'y', 5);
    DFA_set_accepting(dfa, 5, true);

    return dfa;
}

//DFA for string containing 987
DFA* DFA_for_contains_987() {
    DFA *dfa = new_DFA(4);
    DFA_set_transition(dfa,0,'9',1);
    DFA_set_transition(dfa,1,'8',2);
    DFA_set_transition(dfa,2,'7',3);

    DFA_add_transition_all_but(dfa,0,'9',0);
    DFA_add_transition_all_but(dfa,1,'8',1);
    DFA_add_transition_all_but(dfa,2,'7',2);
    DFA_set_transition_all(dfa,3,3);

    DFA_set_accepting(dfa, 3, true);
    return dfa;
}


//DFA for string containing two or three fours
DFA* DFA_for_contains_4s() {
    DFA *dfa = new_DFA(4);
    DFA_set_transition(dfa, 0, '4', 1);
    DFA_set_transition(dfa, 1, '4', 2);
    DFA_set_transition(dfa, 2, '4', 3);
    DFA_add_transition_all_but(dfa,0, '4', 0);
    DFA_add_transition_all_but(dfa,1, '4', 1);
    DFA_add_transition_all_but(dfa,2, '4', 2);
    DFA_add_transition_all_but(dfa,3, '4', 3);

    DFA_set_accepting(dfa, 2, true);
    DFA_set_accepting(dfa, 3, true);
    return dfa;

}

// DFA for binary input with odd number of 0s and 1s
DFA* DFA_for_odd_0s_and_1s() {
    DFA* dfa = new_DFA(4);
    DFA_set_transition(dfa, 0, '0', 1);
    DFA_set_transition(dfa, 0, '1', 2);
    DFA_set_transition(dfa, 1, '0', 0);
    DFA_set_transition(dfa, 1, '1', 3);
    DFA_set_transition(dfa, 2, '0', 3);
    DFA_set_transition(dfa, 2, '1', 0);
    DFA_set_transition(dfa, 3, '0', 2);
    DFA_set_transition(dfa, 3, '1', 1);
    DFA_set_accepting(dfa, 3, true);

    return dfa;
}