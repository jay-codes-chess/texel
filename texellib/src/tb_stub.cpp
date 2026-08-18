/*
    tb_stub.cpp — No-op tablebase stubs for the Puzzle Champions Texel build.
    
    Texel's engine does not require Syzygy/Gaviota tablebase support to play
    (optional feature, compiled only with -DGTB/-DSYZYGY). This provides minimal
    no-op implementations so the engine links cleanly without the gtb/ and
    syzygy/ subdirectories (which need external data files).

    All probes return "no tablebase info available" — correct when TBs are unconfigured.
*/

#include "tbprobe.hpp"
#include "tbgen.hpp"
#include "position.hpp"
#include "moveGen.hpp"
#include "transpositionTable.hpp"

namespace TBProbeData {
    int maxPieces = 0;
}

void TBProbe::initialize(const std::string&, int, const std::string&) {}
bool TBProbe::tbEnabled() { return false; }
bool TBProbe::tbProbe(Position&, int, int, int, TranspositionTable&, TranspositionTable::TTEntry&, int) { return false; }
bool TBProbe::getSearchMoves(Position&, const MoveList&, std::vector<Move>&, TranspositionTable&) { return false; }
void TBProbe::extendPV(const Position&, std::vector<Move>&, TranspositionTable&) {}

template <typename T>
TBGenerator<T>::TBGenerator(T& storage, const PieceCount& pc) : pieceCount(pc), table(storage) {}
template <typename T>
bool TBGenerator<T>::generate(RelaxedShared<S64>&, bool) { return false; }
template <typename T>
bool TBGenerator<T>::probeDTM(const Position&, int, int&) { return false; }

template class TBGenerator<TTStorage>;
