#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "source/scoring.h"

int main(void)
{
    Card cards[1] = {0};
    PokerResult poker = {0};

    cards[0].rank = RANK_2;
    cards[0].suit = SUIT_DIAMONDS;
    cards[0].nominal = 2;

    poker.type = HAND_HIGH_CARD;
    poker.scoring_count = 1;
    poker.scoring_indices[0] = 0;

    // Bonus: +30 fichas.
    cards[0].enhancement = ENHANCEMENT_BONUS;

    ScoreResult result = score_hand(cards, 1, &poker, NULL, 0);

    assert(result.total_chips == 37);
    assert(result.total_mult == 1);
    assert(result.score == 37);

    // Mult: +4 multiplicador.
    cards[0].enhancement = ENHANCEMENT_MULT;

    result = score_hand(cards, 1, &poker, NULL, 0);

    assert(result.total_chips == 7);
    printf("Mult obtenido: %d\n", result.total_mult);
    if (result.total_mult != 5)
    {
        printf("ERROR: se esperaba 5, se obtuvo %d\n",
               result.total_mult);
        return 1;
    }
    assert(result.score == 35);

    // Glass: x2 multiplicador.
    cards[0].enhancement = ENHANCEMENT_GLASS;

    result = score_hand(cards, 1, &poker, NULL, 0);

    assert(result.total_chips == 7);
    assert(result.total_mult == 2);
    assert(result.score == 14);

    // Stone: +50 fichas.
    cards[0].enhancement = ENHANCEMENT_STONE;

    result = score_hand(cards, 1, &poker, NULL, 0);

    assert(result.total_chips == 55);
    assert(result.total_mult == 1);
    assert(result.score == 55);

    // Una carta debilitada no activa estas mejoras.
    cards[0].debuffed = 1;

    result = score_hand(cards, 1, &poker, NULL, 0);

    assert(result.total_chips == 5);
    assert(result.total_mult == 1);
    assert(result.score == 5);

    printf("Todas las pruebas de mejoras pasaron.\n");

    return 0;
}