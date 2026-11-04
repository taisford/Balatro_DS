
#include <assert.h>
#include <stdio.h>

#include "source/cards.h"

int main(void)
{
    CardRegistry registry;
    Deck deck;

    deck_init(&deck, &registry);

    assert(registry.card_count == 52);

    // Tomar una carta del registro.
    uint16_t id = deck.card_ids[0];

    // 1. Modificar sus propiedades.
    assert(card_set_enhancement(
        &registry, id, ENHANCEMENT_GLASS));

    assert(card_set_edition(
        &registry, id, EDITION_HOLOGRAPHIC));

    assert(card_set_seal(
        &registry, id, SEAL_RED));

    assert(card_set_debuffed(&registry, id, 1));

    // 2. Comprobar que los cambios se conservaron.
    Card *card = card_registry_find(&registry, id);

    assert(card != NULL);
    assert(card->enhancement == ENHANCEMENT_GLASS);
    assert(card->edition == EDITION_HOLOGRAPHIC);
    assert(card->seal == SEAL_RED);
    assert(card->debuffed == 1);

    // 3. Cambiar el estado de debilitamiento.
    assert(card_set_debuffed(&registry, id, 0));
    assert(card->debuffed == 0);

    // 4. Comprobar valores inválidos.
    assert(!card_set_enhancement(
        &registry, id, (CardEnhancement)999));

    assert(!card_set_edition(
        &registry, id, (CardEdition)999));

    assert(!card_set_seal(
        &registry, id, (CardSeal)999));

    // 5. Comprobar que no se modifican cartas inexistentes.
    assert(!card_set_enhancement(
        &registry, 999, ENHANCEMENT_GLASS));

    assert(!card_set_edition(
        &registry, 999, EDITION_FOIL));

    assert(!card_set_seal(
        &registry, 999, SEAL_RED));

    assert(!card_set_debuffed(&registry, 999, 1));

    // 6. Comprobar que las propiedades sobreviven al barajado.
    deck_shuffle(&deck);

    card = card_registry_find(&registry, id);

    assert(card != NULL);
    assert(card->enhancement == ENHANCEMENT_GLASS);
    assert(card->edition == EDITION_HOLOGRAPHIC);
    assert(card->seal == SEAL_RED);
    assert(card->debuffed == 0);

    printf("Todas las pruebas de propiedades pasaron.\n");

    return 0;
}
