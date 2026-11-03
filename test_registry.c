
#include <assert.h>
#include <stdio.h>

#include "source/cards.h"

int main(void)
{
    CardRegistry registry;
    card_registry_init(&registry);

    uint16_t new_id;

    // 1. Inicialización
    assert(registry.card_count == 0);
    assert(registry.next_card_id == 0);

    // 2. Agregar una carta
    Card card = {0};
    card.suit = SUIT_HEARTS;
    card.rank = RANK_ACE;
    card.nominal = 11;

    assert(card_registry_add(&registry, card, &new_id));
    assert(new_id == 0);
    assert(registry.card_count == 1);

    // 3. Buscar la carta
    Card *found = card_registry_find(&registry, new_id);

    assert(found != NULL);
    assert(found->rank == RANK_ACE);
    assert(found->suit == SUIT_HEARTS);

    // 4. Agregar una carta duplicada
    assert(card_registry_add(&registry, card, &new_id));
    assert(new_id == 1);
    assert(registry.card_count == 2);

    // 5. Los IDs deben ser diferentes
    assert(registry.cards[0].id != registry.cards[1].id);

    // 6. Eliminar una carta
    assert(card_registry_remove(&registry, 0));
    assert(registry.card_count == 1);
    assert(card_registry_find(&registry, 0) == NULL);
    assert(card_registry_find(&registry, 1) != NULL);

    // 7. No reutilizar IDs
    assert(card_registry_add(&registry, card, &new_id));
    assert(new_id == 2);

    // 8. Rechazar IDs agotados
    registry.next_card_id = (uint32_t)UINT16_MAX + 1;

    int previous_count = registry.card_count;
    assert(!card_registry_add(&registry, card, &new_id));
    assert(registry.card_count == previous_count);

    // 9. Rechazar eliminación inexistente
    assert(!card_registry_remove(&registry, 999));

    printf("Todas las pruebas del registro pasaron correctamente.\n");

    return 0;
}
