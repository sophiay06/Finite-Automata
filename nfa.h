/*
* File: nfa.h
 * Creator: George Ferguson
 * Created: Thu Sep  1 17:54:41 2016
 */

#ifndef _nfa_h
#define _nfa_h

#include <stdbool.h>
#include "IntHashSet.h"

/**
 * The data structure used to represent a nondeterministic finite automaton.
 * @see FOCS Section 10.3
 * @see Comments for DFA in dfa.h
 */
typedef struct NFA {
    int num_states;
    char symbols[256];
    IntHashSet** transitions;
    int initial_state;
    IntHashSet accepting_states;
} NFA;

/**
 * Allocate and return a new NFA containing the given number of states.
 */
extern NFA* new_NFA(int nstates);

/**
 * Free the given NFA.
 */
extern void NFA_free(NFA* nfa);

/**
 * Return the number of states in the given NFA.
 */
extern int NFA_get_size(NFA* nfa);

/**
 * Return the set of next states specified by the given NFA's transition
 * function from the given state on input symbol sym.
 */
extern IntHashSet NFA_get_transitions(NFA* nfa, int state, char sym);

/**
 * For the given NFA, add the state dst to the set of next states from
 * state src on input symbol sym.
 */
extern void NFA_add_transition(NFA* nfa, int src, char sym, int dst);

/**
 * Add a transition for the given NFA for each symbol in the given str.
 */
extern void NFA_add_transition_str(NFA* nfa, int src, char *str, int dst);

/**
 * Add a transition for the given NFA for each input symbol.
 */
extern void NFA_add_transition_all(NFA* nfa, int src, int dst);

extern void NFA_add_transition_all_but(NFA* nfa, int src, char sym, int dst);

/**
 * Set whether the given NFA's state is accepting or not.
 */
extern void NFA_set_accepting(NFA* nfa, int state, bool value);

/**
 * Return true if the given NFA's state is an accepting state.
 */
extern bool NFA_get_accepting(NFA* nfa, int state);

IntHashSet NFA_get_all_accepting(NFA* nfa);

/**
 * Run the given NFA on the given input string, and return true if it accepts
 * the input, otherwise false.
 */
extern bool NFA_execute(NFA* nfa, char *input);

/**
 * Print the given NFA to System.out.
 */
extern void NFA_print(NFA* nfa);

//Run any NFA in a REPL
extern void NFA_repl(NFA *nfa);

// Specific NFA pattern functions
extern NFA* NFA_for_ends_in_gh();
extern NFA* NFA_for_contains_moo();
extern NFA* NFA_for_contains_specific_letter();

#endif // NFA_H
