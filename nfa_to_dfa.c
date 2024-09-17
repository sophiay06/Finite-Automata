#include <stdlib.h>
#include "nfa.h"
#include "dfa.h"
#include "IntHashSet.h"

DFA* NFA_to_DFA(NFA* nfa) {
    // Create a new DFA with an initial estimate of the number of states
    DFA* dfa = new_DFA(1 << nfa->num_states);  // Power set of NFA states
    IntHashSet* state_map = (IntHashSet*)malloc((1 << nfa->num_states) * sizeof(IntHashSet));

    // Initialize the queue for subset construction
    IntHashSet initial_set = new_IntHashSet(nfa->num_states);
    IntHashSet_insert(initial_set, nfa->initial_state);
    state_map[0] = initial_set;
    int current_dfa_state = 0;

    // Queue for new DFA states (NFA state sets)
    int queue[1 << nfa->num_states];
    int queue_start = 0, queue_end = 1;
    queue[0] = current_dfa_state;

    // Mark accepting states in DFA
    if (NFA_get_accepting(nfa, nfa->initial_state)) {
        DFA_set_accepting(dfa, current_dfa_state, true);
    }

    while (queue_start < queue_end) {
        int dfa_state = queue[queue_start++];
        IntHashSet current_nfa_states = state_map[dfa_state];

        // Iterate over all possible input symbols
        for (int sym = 0; sym < 128; sym++) {
            IntHashSet next_nfa_states = new_IntHashSet(nfa->num_states);

            // For each NFA state in the current DFA state, compute its transitions
            for (int nfa_state = 0; nfa_state < nfa->num_states; nfa_state++) {
                if (IntHashSet_lookup(current_nfa_states, nfa_state)) {
                    IntHashSet transitions = NFA_get_transitions(nfa, nfa_state, (char)sym);
                    IntHashSet_union(next_nfa_states, transitions);
                }
            }

            // Check if this NFA state set has already been mapped to a DFA state
            int found = -1;
            for (int i = 0; i < queue_end; i++) {
                if (IntHashSet_equals(state_map[i], next_nfa_states)) {
                    found = i;
                    break;
                }
            }

            if (found == -1) {
                // Insert new DFA state
                state_map[queue_end] = next_nfa_states;  // Will be freed later
                queue[queue_end] = queue_end;
                found = queue_end++;
                for (int nfa_state = 0; nfa_state < nfa->num_states; nfa_state++) {
                    if (IntHashSet_lookup(next_nfa_states, nfa_state) && NFA_get_accepting(nfa, nfa_state)) {
                        DFA_set_accepting(dfa, found, true);
                    }
                }
            } else {
                // Free next_nfa_states if it's not new
                IntHashSet_free(next_nfa_states);
            }

            // Set the transition in the DFA
            DFA_set_transition(dfa, dfa_state, (char)sym, found);
        }
    }

    // Free temporary memory
    for (int i = 0; i < queue_end; i++) {
        IntHashSet_free(state_map[i]);
    }
    free(state_map);

    return dfa;
}

