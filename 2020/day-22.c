#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <bsd/sys/tree.h>
#include <bsd/sys/queue.h>

#include "./read_file.h"

struct card {
  int value;
  TAILQ_ENTRY(card) link;
};
TAILQ_HEAD(deck, card);

struct prevDeck {
  struct deck* deck1;
  struct deck* deck2;
  TAILQ_ENTRY(prevDeck) link;
};
TAILQ_HEAD(prevDecks, prevDeck);

struct deck* deck_TAILQ_COPY(struct deck* a){
	struct deck* ret = malloc(sizeof(struct deck));
	TAILQ_INIT(ret);
	struct card* i = NULL;
	TAILQ_FOREACH(i, a, link){
		struct card* i_new = malloc(sizeof(struct card));
		i_new->value = i->value;
		TAILQ_INSERT_TAIL(ret, i_new, link);
	}
	return ret;
}

struct deck* deck_TAILQ_COPYN(struct deck* a, int n){
	struct deck* ret = malloc(sizeof(struct deck));
	TAILQ_INIT(ret);
	struct card* i = NULL;
	int j = 0;
	TAILQ_FOREACH(i, a, link){
		struct card* i_new = malloc(sizeof(struct card));
		i_new->value = i->value;
		TAILQ_INSERT_TAIL(ret, i_new, link);
		if(++j >= n) break;
	}
	return ret;
}

void deck_TAILQ_PRINT(struct deck* a){
	struct card* i = NULL;
	printf("> ");
	TAILQ_FOREACH(i, a, link){
		printf("%d, ", i->value);
	}
	printf("\n");
}

int deck_TAILQ_SIZE(struct deck* a){
	int ret = 0;
	struct card* i = NULL;
	TAILQ_FOREACH(i, a, link){
		++ret;
	}
	return ret;
}

void parse_input(struct string_content* input, struct deck* deck1, struct deck* deck2){
	struct deck* deck = NULL;
	for(int i = 0; i < input->length; ++i){
		if(!strcmp(input->content[i], "")){continue;}
		if(!strcmp(input->content[i], "Player 1:")){
			deck = deck1;
			continue;
		} else if (!strcmp(input->content[i], "Player 2:")){
			deck = deck2;
			continue;
		}
		struct card* card = malloc(sizeof(struct card));
		card->value = atoi(input->content[i]);
		TAILQ_INSERT_TAIL(deck, card, link);
	}
}

int calc_points(struct deck* a){
	int points = 0;
	int j = 1;
	struct card* i = NULL;
	TAILQ_FOREACH_REVERSE(i, a, deck, link){
		points += j * i->value;
		j++;
	}
	return points;
}

_Bool deck_equal(struct deck* a, struct deck* b){
	/* printf("\n"); */
	struct card* j1 = TAILQ_FIRST(a);
	struct card* j2 = TAILQ_FIRST(b);
	// check for same values
	for(; j1 != NULL && j2 != NULL; j1 = j1->link.tqe_next, j2 = j2->link.tqe_next){
		/* printf("j1: %d, j2: %d -> %d\n", j1->value, j2->value, j1->value != j2->value); */
		if(j1->value != j2->value){
			return 0;
		}
	}
	// check for different size
	if((j1 == NULL && j2 != NULL) || (j2 == NULL && j1 != NULL)){
		return 0;
	}
	// both matched
	/* printf("deck1:"); */
	/* deck_TAILQ_PRINT(a); */
	/* printf("deck2:"); */
	/* deck_TAILQ_PRINT(b); */
	/* printf("\n"); */
	return 1;
}

_Bool prevDecks_TAILQ_CONTAINS(struct prevDecks* prevDecks, struct deck* deck1, struct deck* deck2){
	struct prevDeck* i = NULL;
	TAILQ_FOREACH(i, prevDecks, link){
		if(deck_equal(i->deck1, deck1) && deck_equal(i->deck2, deck2)){
			return 1;
		}
	}
	return 0;
}

int part1(struct string_content* input){
	struct deck deck1; TAILQ_INIT(&deck1);
	struct deck deck2; TAILQ_INIT(&deck2);
	parse_input(input, &deck1, &deck2);
	/* printf("Player1:\n"); */
	/* deck_TAILQ_PRINT(&deck1); */
	/* printf("Player2:\n"); */
	/* deck_TAILQ_PRINT(&deck2); */

	while(!(TAILQ_EMPTY(&deck1) || TAILQ_EMPTY(&deck2))){
		struct card* card1 = TAILQ_FIRST(&deck1); TAILQ_REMOVE(&deck1, card1, link);
		struct card* card2 = TAILQ_FIRST(&deck2); TAILQ_REMOVE(&deck2, card2, link);

		if(card1->value < card2->value){
			TAILQ_INSERT_TAIL(&deck2, card2, link);
			TAILQ_INSERT_TAIL(&deck2, card1, link);
		} else {
			TAILQ_INSERT_TAIL(&deck1, card1, link);
			TAILQ_INSERT_TAIL(&deck1, card2, link);
		}
	}
	/* printf("Player1:\n"); */
	/* deck_TAILQ_PRINT(&deck1); */
	/* printf("Player2:\n"); */
	/* deck_TAILQ_PRINT(&deck2); */
	/* printf("Player1: %d Player2: %d\n", calc_points(&deck1), calc_points(&deck2)); */

	if (TAILQ_EMPTY(&deck1)){
		return calc_points(&deck2);
	}
	return calc_points(&deck1);
}

int part2_rec(struct deck* deck1, struct deck* deck2);

int part2(struct string_content* input){
	struct deck deck1; TAILQ_INIT(&deck1);
	struct deck deck2; TAILQ_INIT(&deck2);
	parse_input(input, &deck1, &deck2);

	return part2_rec(&deck1, &deck2);
}

int part2_rec(struct deck* deck1, struct deck* deck2){
	struct prevDecks* prevDecks = malloc(sizeof(struct prevDecks)); TAILQ_INIT(prevDecks);
	while(!(TAILQ_EMPTY(deck1) || TAILQ_EMPTY(deck2))){
		/* printf("Player1: "); */
		/* deck_TAILQ_PRINT(deck1); */
		/* printf("Player2: "); */
		/* deck_TAILQ_PRINT(deck2); */
		if(prevDecks_TAILQ_CONTAINS(prevDecks, deck1, deck2)){
			/* printf("Deck already contained\n"); */
			return calc_points(deck1); // player1 has won
		}
		// insert current decks to prevDeck
		struct prevDeck* n = malloc(sizeof(struct prevDeck));
		n->deck1 = deck_TAILQ_COPY(deck1);
		n->deck2 = deck_TAILQ_COPY(deck2);
		TAILQ_INSERT_HEAD(prevDecks, n, link);

		// deal like normal
		struct card* card1 = TAILQ_FIRST(deck1); TAILQ_REMOVE(deck1, card1, link);
		struct card* card2 = TAILQ_FIRST(deck2); TAILQ_REMOVE(deck2, card2, link);

		// check for possible sub game
		if(deck_TAILQ_SIZE(deck1) >= card1->value && deck_TAILQ_SIZE(deck2) >= card2->value){
			/* printf("Playing a subgame\n"); */
			/* printf("\n"); */
			if(part2_rec(deck_TAILQ_COPYN(deck1, card1->value), deck_TAILQ_COPYN(deck2, card2->value)) > 0){
				// player1 has won
				TAILQ_INSERT_TAIL(deck1, card1, link);
				TAILQ_INSERT_TAIL(deck1, card2, link);
			} else {
				// player2 has won
				TAILQ_INSERT_TAIL(deck2, card2, link);
				TAILQ_INSERT_TAIL(deck2, card1, link);
			}
		} else {
			// winning condition line normal
			/* printf("Player determined by normal winning condition\n"); */
			if(card1->value < card2->value){
				// player1 has won
				TAILQ_INSERT_TAIL(deck2, card2, link);
				TAILQ_INSERT_TAIL(deck2, card1, link);
			} else {
				// player2 has won
				TAILQ_INSERT_TAIL(deck1, card1, link);
				TAILQ_INSERT_TAIL(deck1, card2, link);
			}
		}
		/* printf("\n"); */
	}
	/* printf("Player1: "); */
	/* deck_TAILQ_PRINT(deck1); */
	/* printf("Player2: "); */
	/* deck_TAILQ_PRINT(deck2); */
	/* printf("Player1: %d Player2: %d\n", calc_points(deck1), calc_points(deck2)); */

	if (TAILQ_EMPTY(deck1)){
		return -calc_points(deck2);
	}
	return calc_points(deck1);
}

int main() {
	char* type;
	(void)type;

	struct string_content *input;
	input = read_file("./day-22.dat", type);
	printf("Part1:\n%d\n", part1(input));

	input = read_file("./day-22.dat", type);
	printf("\nPart2:\n%d\n", part2(input));
	free_file(input);
	return EXIT_SUCCESS;
}

// TODO free data structures
