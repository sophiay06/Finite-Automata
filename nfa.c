#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "IntHashSet.h"
#include "nfa.h"

NFA* new_NFA(int nstates) {
    NFA* nfa = (NFA*)malloc(sizeof(NFA));
    nfa->num_states = nstates;
    nfa->initial_state = 0;
    nfa->transitions = (IntHashSet**)malloc(nstates * sizeof(IntHashSet*));
    for (int i = 0; i < nstates; i++) {
        nfa->transitions[i] = (IntHashSet*)malloc(128 * sizeof(IntHashSet));
        for (int j = 0; j < 128; j++) {
            nfa->transitions[i][j] = new_IntHashSet(20);
        } }
    nfa->accepting_states = new_IntHashSet(nstates);
    return nfa;
    
}


void NFA_free(NFA* nfa) {
    if (nfa) {
        for (int i = 0; i < nfa->num_states; i++) {
            for (int j = 0; j < 128; j++) {
                IntHashSet_free(nfa->transitions[i][j]);
            }
            free(nfa->transitions[i]);
        }
        free(nfa->transitions);
        IntHashSet_free(nfa->accepting_states);
        free(nfa);
    }
}

int NFA_get_size(NFA* nfa) {
    return nfa->num_states;
}

IntHashSet NFA_get_transitions(NFA* nfa, int state, char sym) {
    return nfa->transitions[state][(int)sym];
}

void NFA_add_transition(NFA* nfa, int src, char sym, int dst) {
    IntHashSet_insert(nfa->transitions[src][(int)sym], dst);
}

void NFA_add_transition_all(NFA* nfa, int src, int dst) {
    for (int i = 0; i < 128; i++) {
        IntHashSet_insert(nfa->transitions[src][i], dst);
    }
}

void NFA_add_transition_all_but(NFA* nfa, int src, char sym, int dst) {
    for (int i = 0; i < 128; i++) {
        if (i != (int)sym) {
            IntHashSet_insert(nfa->transitions[src][i], dst);
        }
    }
}

void NFA_set_accepting(NFA* nfa, int state, bool value) {
    if (value) {
        IntHashSet_insert(nfa->accepting_states, state);
    }
}

bool NFA_get_accepting(NFA* nfa, int state) {
    return IntHashSet_lookup(nfa->accepting_states, state);
}

IntHashSet NFA_get_all_accepting(NFA* nfa) {
    return nfa->accepting_states;
}

bool NFA_execute(NFA* nfa, char* input) {
    IntHashSet current_states = new_IntHashSet(nfa->num_states);
    IntHashSet_insert(current_states, nfa->initial_state);

    for (int i=0;input[i] !='\0'; i++) {
        IntHashSet nextstates = new_IntHashSet(nfa->num_states);
        for (int j = 0; j < nfa->num_states; j++) {
            if (IntHashSet_lookup(current_states, j)) {
                IntHashSet transitions = nfa->transitions[j][(int)input[i]];
                IntHashSet_union(nextstates, transitions);
            }
        }
        IntHashSet_free(current_states);
        current_states = nextstates;
    }

    bool res = false;
    for (int i = 0; i < nfa->num_states; i++) {
        if (IntHashSet_lookup(current_states, i) && NFA_get_accepting(nfa, i)) {
            res = true;
            break;
        }
        
    }
    IntHashSet_free(current_states);
    return res;
}

void NFA_repl(NFA* nfa) {
    char input[128];
    while (true) {
        printf("Enter an input (""quit"" to quit): ");
        fgets(input, sizeof(input), stdin);
        // Remove newline character if present
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "quit")==0) {
            break;
        }
        bool res = NFA_execute(nfa, input);
        if (res) {
            printf("res for input \"%s\": true\n", input);
        } else {
            printf("res for input \"%s\": false\n", input);
        }
    }
}

// NFA for strings ending in "gh"
NFA* NFA_for_ends_in_gh() {
    NFA* nfa = new_NFA(3);
    NFA_add_transition(nfa, 0, 'g', 1);
    NFA_add_transition(nfa, 1, 'h', 2);
    NFA_add_transition_all_but(nfa, 0, 'g', 0);
    NFA_add_transition_all_but(nfa, 1, 'h', 0);
    NFA_set_accepting(nfa, 2, true);
    
    return nfa;
}

// NFA for strings contain "moo"
NFA* NFA_for_contains_moo() {
    NFA* nfa = new_NFA(4);
    NFA_add_transition(nfa, 0, 'm', 1);
    NFA_add_transition(nfa, 1, 'o', 2);
    NFA_add_transition(nfa, 2, 'o', 3);

    NFA_add_transition_all_but(nfa, 0, 'm', 0);
    NFA_add_transition_all_but(nfa, 1, 'o', 0);
    NFA_add_transition_all_but(nfa, 2, 'o', 0);
    NFA_add_transition_all(nfa, 3, 3);

    NFA_set_accepting(nfa, 3, true);
    return nfa;
}

//Test NFA for strings that have more than one a or i, or more than two y’s, or more than three c’s or l’s
NFA* NFA_for_contains_specific_letter() {
    NFA* nfa = new_NFA(16);
    NFA_add_transition_all(nfa, 0, 0);

    //more than one a's
    NFA_add_transition(nfa, 0, 'a', 1);
    NFA_add_transition(nfa, 1, 'a', 2);
    NFA_add_transition_all_but(nfa, 1, 'a', 1);
    NFA_add_transition_all(nfa, 2, 2);

    //more than one i's
    NFA_add_transition(nfa, 0, 'i', 3);
    NFA_add_transition(nfa, 3, 'i', 4);
    NFA_add_transition_all_but(nfa, 3, 'i', 3);
    NFA_add_transition_all(nfa, 4, 4);

    //more than two y's
    NFA_add_transition(nfa, 0, 'y', 5);
    NFA_add_transition(nfa, 5, 'y', 6);
    NFA_add_transition(nfa, 6, 'y', 7);
    NFA_add_transition_all_but(nfa, 5, 'y', 5);
    NFA_add_transition_all_but(nfa, 6, 'y', 6);
    NFA_add_transition_all(nfa, 6, 6);
    NFA_add_transition_all(nfa, 7, 7);

    //more than three c's
    NFA_add_transition(nfa, 0, 'c', 8);
    NFA_add_transition(nfa, 8, 'c', 9);
    NFA_add_transition(nfa, 9, 'c', 10);
    NFA_add_transition(nfa, 10, 'c', 11);
    NFA_add_transition_all_but(nfa, 8, 'c', 8);
    NFA_add_transition_all_but(nfa, 9, 'c', 9);
    NFA_add_transition_all_but(nfa, 10, 'c', 10);
    NFA_add_transition_all(nfa, 9, 9);
    NFA_add_transition_all(nfa, 10, 10);
    NFA_add_transition_all(nfa, 11, 11);

    //more than three l's
    NFA_add_transition(nfa, 0, 'l', 12);
    NFA_add_transition(nfa, 12, 'l', 13);
    NFA_add_transition(nfa, 13, 'l', 14);
    NFA_add_transition(nfa, 14, 'l', 15);
    NFA_add_transition_all_but(nfa, 12, 'l', 12);
    NFA_add_transition_all_but(nfa, 13, 'l', 13);
    NFA_add_transition_all_but(nfa, 14, 'l', 14);
    NFA_add_transition_all(nfa, 13, 13);
    NFA_add_transition_all(nfa, 14, 14);
    NFA_add_transition_all(nfa, 15, 15);

    // Set accepting state
    NFA_set_accepting(nfa, 2, true);
    NFA_set_accepting(nfa, 4, true);
    NFA_set_accepting(nfa, 7, true);
    NFA_set_accepting(nfa, 11, true);
    NFA_set_accepting(nfa, 15, true);

    return nfa;
}