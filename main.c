#include <stdio.h>
#include "dfa.h"
#include "nfa.h"
#include "nfa_to_dfa.h"

int main(void) {
    printf("CSC173 Project 1 by Sophia Yang and Andhiya Madamala:\n");

    // Test DFA for "xyzzy" pattern
    printf("\nTesting DFA for containing 'xyzzy':\n");
    DFA *dfa_xyzzy = DFA_for_contains_xyzzy();
    DFA_repl(dfa_xyzzy);
    DFA_free(dfa_xyzzy);

    // Test DFA for "987" pattern
    printf("\nTesting DFA for containing '987':\n");
    DFA *dfa_987 = DFA_for_contains_987();
    DFA_repl(dfa_987);
    DFA_free(dfa_987);


    // Test DFA for "two or three fours" pattern
    printf("\nTesting DFA for containing 'two or three fours':\n");
    DFA *dfa_4s = DFA_for_contains_4s();
    DFA_repl(dfa_4s);
    DFA_free(dfa_4s);

    // Test DFA for "an odd number of 0's and an odd number of 1's" pattern
    printf("\nTesting DFA for containing 'an odd number of 0’s and an odd number of 1’s':\n");
    DFA *dfa_0s_and_1s = DFA_for_odd_0s_and_1s();
    DFA_repl(dfa_0s_and_1s);
    DFA_free(dfa_0s_and_1s);

    // Test NFA for "ends in gh" pattern
    printf("\nTesting NFA for ending in 'gh':\n");
    NFA *nfa_gh = NFA_for_ends_in_gh();
    NFA_repl(nfa_gh);
    NFA_free(nfa_gh);

    //Test NFA for "moo" pattern
    printf("\nTesting NFA for containing 'moo':\n");
    NFA *nfa_moo = NFA_for_contains_moo();
    NFA_repl(nfa_moo);
    NFA_free(nfa_moo);

    //Test NFA for strings that have more than one a or i, or more than two y’s, or more than three c’s or l’s
    printf("\nTesting NFA for containing 'more than one a or i, or more than two y’s, or more than three c’s or l’s':\n");
    NFA *nfa = NFA_for_contains_specific_letter();
    NFA_repl(nfa);
    NFA_free(nfa);

    //Test DFA for "ends in gh" pattern
    printf("\nTesting NFA to DFA for ending in 'gh':\n");
    NFA *nfa1 = NFA_for_ends_in_gh();
    DFA *dfa1 = NFA_to_DFA(nfa1);
    printf("Number of states = %d\n", dfa1->num_states);
    DFA_repl(dfa1);
    NFA_free(nfa1);
    DFA_free(dfa1);

    //Test DFA for "moo" pattern
    printf("\nTesting NFA to DFA for containing 'moo':\n");
    NFA *nfa2 = NFA_for_contains_moo();
    DFA *dfa2 = NFA_to_DFA(nfa2);
    printf("Number of states = %d\n", dfa2->num_states);
    DFA_repl(dfa2);
    NFA_free(nfa2);
    DFA_free(dfa2);

    //Test DFA for strings that have more than one a or i, or more than two y’s, or more than three c’s or l’s
    printf("\nTesting NFA to DFA for containing 'more than one a or i, or more than two y’s, or more than three c’s or l’s':\n");
    NFA *nfa3 = NFA_for_contains_specific_letter();
    DFA *dfa3 = NFA_to_DFA(nfa3);
    printf("Number of states = %d\n", dfa3->num_states);
    DFA_repl(dfa3);
    NFA_free(nfa3);
    DFA_free(dfa3);
    return 0;
}
