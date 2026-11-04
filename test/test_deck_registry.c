
#include <assert.h>
#include <stdio.h>

#include "source/cards.h"

int main(void)
{
    CardRegistry registry;
    Deck deck;

    // 1. Inicializar mazo y registro
    deck_init(&deck, &registry);

    assert(deck.card_count == 52);
    assert(deck.next_card == 0);
    assert(registry.card_count == 52);
    assert(registry.next_card_id == 52);

    // 2. Comprobar que todos los IDs del mazo existen
    for (int i = 0; i < deck.card_count; i++)
    {
        assert(card_registry_find(&registry, deck.card_ids[i]) != NULL);
    }

    // 3. Buscar una carta desde el mazo
    uint16_t first_id = deck.card_ids[0];

    Card *found = deck_find_card(&deck, &registry, first_id);

    assert(found != NULL);
    assert(found->id == first_id);

    // 4. Robar una carta
    Card drawn;

    assert(deck_draw(&deck, &registry, &drawn));
    assert(drawn.id == first_id);
    assert(deck.next_card == 1);

    // 5. Eliminar una referencia del mazo
    assert(deck_remove_card(&deck, first_id));
    assert(deck.card_count == 51);
    assert(deck.next_card == 0);

    // La carta sigue existiendo en el registro
    assert(card_registry_find(&registry, first_id) != NULL);

    // Pero ya no pertenece al mazo
    assert(deck_find_card(&deck, &registry, first_id) == NULL);

    // 6. Añadir una carta nueva
    Card new_card = {0};

    new_card.suit = SUIT_SPADES;
    new_card.rank = RANK_ACE;
    new_card.nominal = 11;

    assert(deck_add_card(&deck, &registry, new_card));

    assert(deck.card_count == 52);
    assert(registry.card_count == 53);

    uint16_t new_id = registry.cards[registry.card_count - 1].id;

    assert(deck_find_card(&deck, &registry, new_id) != NULL);

    // 7. Eliminar una carta definitivamente
    assert(card_registry_remove(&registry, new_id));

    assert(card_registry_find(&registry, new_id) == NULL);
    assert(deck_find_card(&deck, &registry, new_id) == NULL);

    // 8. Barajar y comprobar cantidad
    deck_shuffle(&deck);

    assert(deck.next_card == 0);
    assert(deck.card_count == 52);

    // 9. Comprobar que no hay IDs repetidos
    for (int i = 0; i < deck.card_count; i++)
    {
        for (int j = i + 1; j < deck.card_count; j++)
        {
            assert(deck.card_ids[i] != deck.card_ids[j]);
        }
    }

    printf("Todas las pruebas de Deck y CardRegistry pasaron.\n");

    return 0;
}
