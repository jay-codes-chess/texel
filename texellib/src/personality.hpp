/*
    personality.hpp — World Champion personalities for Texel (Puzzle Champions fork).
    Ported from the MadChess "Human Engine" + Xiphos personalities approach.

    Each personality scales a small set of eval DIMENSIONS (per128, 128 = neutral).
    The eval applies these scales to its component scores. Distinct from the
    full Texel parameter set — personalities bias BEHAVIOUR, not the tuned values.
*/

#ifndef PERSONALITY_HPP_
#define PERSONALITY_HPP_

#include <string>
#include <vector>

/** Eval dimensions a personality can scale (per128; 128 = neutral). */
struct PersonalityScale {
    int kingSafety;      // 128 = neutral. >128 attacks the king harder.
    int threat;          // threats / tactical sharpness
    int mobility;        // piece activity
    int endgame;         // endgame/technique emphasis
    int pawnStructure;   // sensitivity to pawn weaknesses (own + opponent)
    int exchangeSac;     // willingness to sacrifice the exchange for compensation
    int passedPawn;      // passed pawn / race emphasis
    int drawTolerance;   // >128 = accepts draws, <128 = fights to win
    int searchSelectivity; // >128 = sharper (prune quiet moves, tactical);
                           // <128 = wider (search more quiet moves, positional)
    int searchDepth;       // >128 = deeper tactical search, <128 = shallower/broader
};

/** Return the scale for a named personality. Falls back to neutral. */
PersonalityScale getPersonalityScale(const std::string& name);

/** All supported personality names (for the UCI combo). */
std::vector<std::string> personalityNames();

/** Set the active personality (scales the eval). Defined in evaluate.cpp. */
void setPersonality(const std::string& name);

/** Current personality scale (for the search to read selectivity/depth). */
const PersonalityScale& currentPersonality();

#endif
