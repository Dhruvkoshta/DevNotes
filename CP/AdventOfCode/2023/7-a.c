
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELIMITERS " "
#define EXCEPTION_FORMAT "Error: Format.\n"
#define HAND_SIZE 5
#define PLAYER_LIST_CAPACITY 1024

enum Card {
  CARD_TWO,
  CARD_THREE,
  CARD_FOUR,
  CARD_FIVE,
  CARD_SIX,
  CARD_SEVEN,
  CARD_EIGHT,
  CARD_NINE,
  CARD_TEN,
  CARD_JACK,
  CARD_QUEEN,
  CARD_KING,
  CARD_ACE,
  CARD_NONE
};
struct Hand {
  int frequency[CARD_NONE];
  int count;
  int max_frequency;
};

enum HandType {
  HAND_TYPE_HIGH_CARD,
  HAND_TYPE_ONE_PAIR,
  HAND_TYPE_TWO_PAIR,
  HAND_TYPE_THREE_OF_A_KIND,
  HAND_TYPE_FULL_HOUSE,
  HAND_TYPE_FOUR_OF_A_KIND,
  HAND_TYPE_FIVE_OF_A_KIND,
  HAND_TYPE_NONE
};

struct Player {
  enum Card cards[HAND_SIZE];
  enum HandType handType;
  int bid;
};

struct PlayerList {
  struct Player items[PLAYER_LIST_CAPACITY];
  int count;
};

typedef const void *Object;
typedef char *String;
typedef enum Card Card;
typedef enum HandType HandType;
typedef struct Hand *Hand;
typedef struct Player *Player;
typedef struct PlayerList *PlayerList;

Card getCardFromString(char symbol) {
  switch (symbol) {
  case '2':
    return CARD_TWO;
  case '3':
    return CARD_THREE;
  case '4':
    return CARD_FOUR;
  case '5':
    return CARD_FIVE;
  case '6':
    return CARD_SIX;
  case '7':
    return CARD_SEVEN;
  case '8':
    return CARD_EIGHT;
  case '9':
    return CARD_NINE;
  case 'T':
    return CARD_TEN;
  case 'J':
    return CARD_JACK;
  case 'Q':
    return CARD_QUEEN;
  case 'K':
    return CARD_KING;
  case 'A':
    return CARD_ACE;
  default:
    return CARD_NONE; // Invalid card
  }
}

HandType hand_get_type(Hand hand) {
  if (hand->max_frequency == 5) {
    return HAND_TYPE_FIVE_OF_A_KIND;
  } else if (hand->max_frequency == 4) {
    return HAND_TYPE_FOUR_OF_A_KIND;
  } else if (hand->max_frequency == 3 && hand->count == 2) {
    return HAND_TYPE_FULL_HOUSE;
  } else if (hand->max_frequency == 3) {
    return HAND_TYPE_THREE_OF_A_KIND;
  } else if (hand->max_frequency == 2 && hand->count == 3) {
    return HAND_TYPE_TWO_PAIR;
  } else if (hand->max_frequency == 2) {
    return HAND_TYPE_ONE_PAIR;
  } else {
    return HAND_TYPE_HIGH_CARD;
  }
}

int player_compare(Object left, Object right) {
  if (!left && !right) {
    return 0;
  }

  if (!left) {
    return -1;
  }

  if (!right) {
    return 1;
  }

  const struct Player *leftPlayer = left;
  const struct Player *rightPlayer = right;
  int handDifference = rightPlayer->handType - leftPlayer->handType;

  if (handDifference) {
    return handDifference;
  }

  for (int i = 0; i < HAND_SIZE; i++) {
    int cardDifference = rightPlayer->cards[i] - leftPlayer->cards[i];
    if (cardDifference) {
      return cardDifference;
    }
  }

  return 0;
}

void player_list_sort(PlayerList instance) {
  qsort(instance->items, instance->count, sizeof *instance->items,
        player_compare);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
    return 1;
  }

  FILE *fptr;
  fptr = fopen(argv[1], "r");
  if (fptr == NULL) {
    perror("Error opening file");
    return 1;
  }

  char line[1028];

  struct PlayerList players;
  players.count = 0;

  while (fgets(line, sizeof(line), fptr)) {

    String cards = strtok(line, " ");
    char *bid = strtok(NULL, " ");

    struct Hand hand = {0};
    Player player = players.items + players.count;
    players.count++;

    for (int i = 0; i < HAND_SIZE; i++) {
      Card drawnCard = getCardFromString(cards[i]);
      if (drawnCard == CARD_NONE) {
        fprintf(stderr, EXCEPTION_FORMAT);
        fclose(fptr);
        return 1;
      }
      // hand add
      int frequency = hand.frequency[drawnCard];
      if (!frequency) {
        hand.count++;
      }
      frequency++;

      hand.frequency[drawnCard] = frequency;

      if (frequency > hand.max_frequency) {
        hand.max_frequency = frequency;
      }

      player->cards[i] = drawnCard;
    }

    HandType handType = hand_get_type(&hand);

    if (!bid || handType == HAND_TYPE_NONE) {
      fprintf(stderr, EXCEPTION_FORMAT);

      return 1;
    }
    player->handType = handType;
    player->bid = atoi(bid);
  }

  player_list_sort(&players);
  long sum = 0;

  for (int i = 0; i < players.count; i++) {
    sum += (players.count - i) * players.items[i].bid;
  }

  printf("%ld\n", sum);
  return 0;
}
