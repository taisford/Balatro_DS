#include "cards.h"

#include <stdlib.h>

void deck_init(Deck *deck, CardRegistry *registry)
{
    if (deck == NULL || registry == NULL)
        return;

    card_registry_init(registry);

    deck->next_card = 0;
    deck->card_count = 0;

    for (int suit = SUIT_DIAMONDS; suit <= SUIT_SPADES; suit++)
    {
        for (int rank = RANK_2; rank <= RANK_ACE; rank++)
        {
            Card card = {0};

            card.suit = (Suit)suit;
            card.rank = (Rank)rank;
            card.enhancement = ENHANCEMENT_NONE;
            card.edition = EDITION_NONE;
            card.seal = SEAL_NONE;
            card.debuffed = 0;

            if (rank <= RANK_10)
                card.nominal = (uint8_t)rank;
            else if (rank == RANK_ACE)
                card.nominal = 11;
            else
                card.nominal = 10;

            uint16_t new_id;

            if (!card_registry_add(registry, card, &new_id))
                return;

            deck->card_ids[deck->card_count++] = new_id;
        }
    }
}

void deck_shuffle(Deck *deck)
{
    if (deck == NULL || deck->card_count <= 0 ||
        deck->card_count > MAX_DECK_SIZE)
        return;

    for (int i = deck->card_count - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);

        uint16_t temp = deck->card_ids[i];
        deck->card_ids[i] = deck->card_ids[j];
        deck->card_ids[j] = temp;
    }

    deck->next_card = 0;
}

int deck_draw(Deck *deck, CardRegistry *registry, Card *card)
{
    if (deck == NULL || registry == NULL || card == NULL ||
        deck->card_count < 0 ||
        deck->card_count > MAX_DECK_SIZE ||
        deck->next_card < 0 ||
        deck->next_card >= deck->card_count)
        return 0;

    Card *found = card_registry_find(
        registry, deck->card_ids[deck->next_card]);

    if (found == NULL)
        return 0;

    *card = *found;
    deck->next_card++;

    return 1;
}

const char *card_rank_name(Rank rank)
{
    static const char *names[] = {
        "", "", "2", "3", "4", "5", "6", "7",
        "8", "9", "10", "Jack", "Queen", "King", "Ace"};

    if (rank < RANK_2 || rank > RANK_ACE)
        return "?";

    return names[rank];
}

const char *card_suit_name(Suit suit)
{
    static const char *names[] = {
        "Diamonds", "Clubs", "Hearts", "Spades"};

    if (suit < SUIT_DIAMONDS || suit > SUIT_SPADES)
        return "?";

    return names[suit];
}

Card *deck_find_card(Deck *deck, CardRegistry *registry, uint16_t id)
{
    if (deck == NULL || registry == NULL ||
        deck->card_count < 0 ||
        deck->card_count > MAX_DECK_SIZE)
        return NULL;

    for (int i = 0; i < deck->card_count; i++)
    {
        if (deck->card_ids[i] == id)
            return card_registry_find(registry, id);
    }

    return NULL;
}

int deck_add_card(Deck *deck, CardRegistry *registry, Card card)
{
    if (deck == NULL || registry == NULL ||
        deck->card_count < 0 ||
        deck->card_count >= MAX_DECK_SIZE ||
        deck->next_card < 0 ||
        deck->next_card > deck->card_count)
        return 0;

    uint16_t new_id;

    if (!card_registry_add(registry, card, &new_id))
        return 0;

    deck->card_ids[deck->card_count++] = new_id;

    return 1;
}

int deck_remove_card(Deck *deck, uint16_t id)
{
    if (deck == NULL ||
        deck->card_count <= 0 ||
        deck->card_count > MAX_DECK_SIZE ||
        deck->next_card < 0 ||
        deck->next_card > deck->card_count)
        return 0;

    int index = -1;

    for (int i = 0; i < deck->card_count; i++)
    {
        if (deck->card_ids[i] == id)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
        return 0;

    for (int i = index; i < deck->card_count - 1; i++)
        deck->card_ids[i] = deck->card_ids[i + 1];

    deck->card_count--;

    if (index < deck->next_card)
        deck->next_card--;

    return 1;
}

void card_registry_init(CardRegistry *registry)
{
    if (registry == NULL)
        return;

    registry->card_count = 0;
    registry->next_card_id = 0;
}

Card *card_registry_find(CardRegistry *registry, uint16_t id)
{
    if (registry == NULL ||
        registry->card_count < 0 ||
        registry->card_count > MAX_DECK_SIZE)
        return NULL;

    for (int i = 0; i < registry->card_count; i++)
    {
        if (registry->cards[i].id == id)
            return &registry->cards[i];
    }

    return NULL;
}

int card_registry_add(CardRegistry *registry, Card card, uint16_t *new_id)
{
    if (registry == NULL ||
        new_id == NULL ||
        registry->card_count < 0 ||
        registry->card_count >= MAX_DECK_SIZE ||
        registry->next_card_id > UINT16_MAX)
        return 0;

    card.id = (uint16_t)registry->next_card_id++;

    registry->cards[registry->card_count++] = card;

    *new_id = card.id;

    return 1;
}

int card_registry_remove(CardRegistry *registry, uint16_t id)
{
    if (registry == NULL ||
        registry->card_count <= 0 ||
        registry->card_count > MAX_DECK_SIZE)
        return 0;

    int index = -1;

    for (int i = 0; i < registry->card_count; i++)
    {
        if (registry->cards[i].id == id)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
        return 0;

    for (int i = index; i < registry->card_count - 1; i++)
        registry->cards[i] = registry->cards[i + 1];

    registry->card_count--;

    return 1;
}

int card_set_enhancement(
    CardRegistry *registry,
    uint16_t id,
    CardEnhancement enhancement)
{
    if (enhancement < ENHANCEMENT_NONE ||
        enhancement > ENHANCEMENT_LUCKY)
        return 0;

    Card *card = card_registry_find(registry, id);

    if (card == NULL)
        return 0;

    card->enhancement = enhancement;
    return 1;
}

int card_set_edition(
    CardRegistry *registry,
    uint16_t id,
    CardEdition edition)
{
    if (edition < EDITION_NONE ||
        edition > EDITION_NEGATIVE)
        return 0;

    Card *card = card_registry_find(registry, id);

    if (card == NULL)
        return 0;

    card->edition = edition;
    return 1;
}

int card_set_seal(
    CardRegistry *registry,
    uint16_t id,
    CardSeal seal)
{
    if (seal < SEAL_NONE || seal > SEAL_PURPLE)
        return 0;

    Card *card = card_registry_find(registry, id);

    if (card == NULL)
        return 0;

    card->seal = seal;
    return 1;
}

int card_set_debuffed(
    CardRegistry *registry,
    uint16_t id,
    uint8_t debuffed)
{
    Card *card = card_registry_find(registry, id);

    if (card == NULL)
        return 0;

    card->debuffed = debuffed ? 1 : 0;
    return 1;
}