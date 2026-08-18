/*
    exchangeSac.cpp — Positional exchange sacrifice evaluator.
    Ported from the MadChess "Human Engine" EvaluateExchangeSacrifice
    (Rodent-inspired compensation model).

    Detects when a side is down one exchange (rook for minor piece) with equal
    queens, then awards positional compensation for:
      1. The bishop pair (strongest signal)
      2. Destroyed enemy center pawns
      3. Advanced central pawns (rolling pawns)
      4. Open files for the remaining rook(s)
      5. Shattered enemy king pawn shield
    Returns a centipawn score (positive = good for WHITE).
*/

#include "position.hpp"
#include "bitBoard.hpp"
#include "piece.hpp"

namespace ExchangeSac {

static const int BISHOP_PAIR       = 35;
static const int DESTROYED_CENTER  = 22;
static const int CENTRAL_PAWN_PUSH = 9;
static const int OPEN_FILE         = 18;
static const int EXPOSED_KING      = 20;

static bool isExchangeSacSide(const Position& pos, bool white,
                              int& rookExcess, int& minorExcess) {
    const Piece::Type WR = Piece::WROOK, BR = Piece::BROOK;
    const Piece::Type WN = Piece::WKNIGHT, BN = Piece::BKNIGHT;
    const Piece::Type WB = Piece::WBISHOP, BB = Piece::BBISHOP;
    const Piece::Type WQ = Piece::WQUEEN, BQ = Piece::BQUEEN;

    int ourRooks   = BitBoard::bitCount(pos.pieceTypeBB(white ? WR : BR));
    int ourKnights = BitBoard::bitCount(pos.pieceTypeBB(white ? WN : BN));
    int ourBishops = BitBoard::bitCount(pos.pieceTypeBB(white ? WB : BB));
    int ourMinors  = ourKnights + ourBishops;
    int ourQueens  = BitBoard::bitCount(pos.pieceTypeBB(white ? WQ : BQ));

    int enemyRooks   = BitBoard::bitCount(pos.pieceTypeBB(white ? BR : WR));
    int enemyKnights = BitBoard::bitCount(pos.pieceTypeBB(white ? BN : WN));
    int enemyBishops = BitBoard::bitCount(pos.pieceTypeBB(white ? BB : WB));
    int enemyMinors  = enemyKnights + enemyBishops;
    int enemyQueens  = BitBoard::bitCount(pos.pieceTypeBB(white ? BQ : WQ));

    if (ourRooks >= enemyRooks || ourMinors <= enemyMinors || ourQueens != enemyQueens)
        return false;

    rookExcess  = enemyRooks - ourRooks;
    minorExcess = ourMinors - enemyMinors;
    if (rookExcess != minorExcess)
        return false;
    return true;
}

static int compensationFor(const Position& pos, bool white) {
    int total = 0;
    int rookExcess, minorExcess;
    if (!isExchangeSacSide(pos, white, rookExcess, minorExcess))
        return 0;

    const Piece::Type WB = Piece::WBISHOP, BB = Piece::BBISHOP;
    const Piece::Type WP = Piece::WPAWN, BP = Piece::BPAWN;
    const Piece::Type WR = Piece::WROOK, BR = Piece::BROOK;

    U64 ourBishops = pos.pieceTypeBB(white ? WB : BB);
    if ((ourBishops & BitBoard::maskDarkSq) && (ourBishops & BitBoard::maskLightSq))
        total += BISHOP_PAIR;

    U64 enemyPawns = pos.pieceTypeBB(white ? BP : WP);
    U64 centerFiles = BitBoard::maskFile[3] | BitBoard::maskFile[4];
    if (BitBoard::bitCount(enemyPawns & centerFiles) <= 1)
        total += DESTROYED_CENTER;

    U64 ourPawns = pos.pieceTypeBB(white ? WP : BP);
    U64 centralPushMask;
    if (white)
        centralPushMask = (BitBoard::maskRow4 | BitBoard::maskRow5 | BitBoard::maskRow6);
    else
        centralPushMask = (BitBoard::maskRow5 | BitBoard::maskRow4 | BitBoard::maskRow3);
    total += BitBoard::bitCount(ourPawns & centralPushMask & centerFiles) * CENTRAL_PAWN_PUSH;

    U64 ourRookBB = pos.pieceTypeBB(white ? WR : BR);
    while (ourRookBB) {
        int sq = BitBoard::firstSquare(ourRookBB);
        ourRookBB &= ourRookBB - 1;
        int file = sq & 7;
        if (!(BitBoard::maskFile[file] & ourPawns))
            total += OPEN_FILE;
    }

    int enemyKingSq = pos.getKingSq(!white);
    U64 enemyKingZone = BitBoard::kingAttacks[enemyKingSq] | (1ULL << enemyKingSq);
    (void)enemyKingZone;
    int kFile = enemyKingSq & 7;
    U64 shieldFiles = 0;
    if (kFile > 0) shieldFiles |= BitBoard::maskFile[kFile-1];
    shieldFiles |= BitBoard::maskFile[kFile];
    if (kFile < 7) shieldFiles |= BitBoard::maskFile[kFile+1];
    int shieldPawns = BitBoard::bitCount(enemyPawns & shieldFiles);
    if (shieldPawns < 2)
        total += EXPOSED_KING;

    return total;
}

int evaluate(const Position& pos) {
    int wComp = compensationFor(pos, true);
    int bComp = compensationFor(pos, false);
    return wComp - bComp;
}

} // namespace ExchangeSac
