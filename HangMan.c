/**********************************************************************/
/*                                                                    */
/* This program is a two player game of hangman. The admin and will   */
/* enter a word and the program will processes the players guesses.   */
/* Title: Hangman                                                     */
/* Programer: Joseph Patchen                                          */
/* Date: July 10, 2015                                                */
/**********************************************************************/

#include <stdio.h>   /* printf, scanf                                 */
#include <ctype.h>   /* tolower                                       */
#include <stdlib.h>  /* free, exit, null, delay                       */
#include <stdbool.h> /* true, false                                   */
#include <unistd.h>  /* sleep                                         */
#ifdef __unix__
# include <unistd.h>
#elif defined _WIN32
# include <windows.h>
#define sleep(x) Sleep(1000 * x) /* wait for three secconds           */
#endif

/**********************************************************************/
/*                         Symbolic Constants                         */
/**********************************************************************/
#define NEW_LINE          '\n'      /* Carriage return                */
#define HEADER_ALLOC_ERR  1         /* Error allocating memory for    */
                                    /* list header                    */
#define TRAILER_ALLOC_ERR 2         /* Error allocating memory for    */
                                    /* list trailer                   */
#define LETTER_ALLOC_ERR  3         /* Error allocating memory for    */
                                    /* a letter                       */
#define INPUT_ERR         4         /* Error in input for admin word  */
#define LIST_HEADER       '@'       /* Lowest possible alphabet       */
                                    /* letter                         */
#define LIST_TRAILER      '['       /* Highest possible alphabet      */
                                    /* letter                         */
#define MAX_STRIKES       7         /* Max number of incorrect guesses*/
#define MAX_INPUT         100       /* Max number of charicters for   */
                                    /* the admin word                 */
#define CLEAR_SCREEN      printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
                                    /* Clear the screen of characters */

/**********************************************************************/
/*                         Program Structures                         */
/**********************************************************************/
/* Linked list letter                                                 */
struct letter
{
   char letter;
   struct letter *p_next_letter;
};
typedef struct letter LETTER;

/**********************************************************************/
/*                         Function Prototypes                        */
/**********************************************************************/
void print_title();
   /* Print the program title                                         */
void print_instructions();
   /* Print the program instructions                                  */
LETTER *create_list();
   /* Create a linked list                                            */
void fill_lists(LETTER *p_admin_word, LETTER *p_player_word);
   /* Fill the admin list with the word and the player list with      */
   /* spaces                                                          */
char get_admin_response();
   /* Get response to play again or not                               */
void print_display(LETTER *p_player_word, int strike);
   /* Print the game display                                          */
void get_guess(char *guess, int *tries);
   /* Get the players guess                                           */
bool check_word(char guess, LETTER *p_admin_word, int *strike);
   /* Check if guess is in the admin word                             */
void update_word(char guess, LETTER *p_admin_word, LETTER *p_player_word);
   /* Update the player word with the matched letters                 */
bool check_if_win(LETTER *p_admin_word, LETTER *p_player_word);
   /* Check if player word equals the admin word                      */
void congrats_display(LETTER *p_admin_word, int tries, int strike);
   /* Print the display when the player wins                          */
void failure_display(LETTER *p_admin_word);
   /* Print the display when the player wins                          */
bool clear_stream();
   /* Clear all characters from the input stream                      */

/**********************************************************************/
/*                           Main Function                            */
/**********************************************************************/
int main()
{
   char   guess;          /* Letter that the player guessed           */
   LETTER *p_admin_word,  /* Points to the Word that the player is    */
                          /* trying guess                             */
          *p_letter,      /* Points to every letter in both words     */
          *p_player_word; /* Points to the Letters in the word that   */
                          /* the player has guessed correctly         */
   int    strike,         /* Number of incorrect guesses              */
          tries;          /* Number of total guesses                  */

   /* Print the program title and instructions                        */
   print_title();
   print_instructions();

   /* Loop until player no longer wants to play                       */
   printf("\nWould you like to play? (y/n):"                          );
   while (get_admin_response() == 'y')
   {
      strike = 0;
      tries = 0;

      /* Create the lists for the admin and player words. Fill the    */
      /* admin word and the player word                               */
      p_admin_word  = create_list();
      p_player_word = create_list();
      fill_lists(p_admin_word, p_player_word);

      /* Loop until player wins or looses                             */
      while (check_if_win(p_admin_word, p_player_word) && strike < MAX_STRIKES)
      {
         /* Update display and get the player's letter guess          */
         CLEAR_SCREEN;
         print_display(p_player_word, strike);
         get_guess    (&guess, &tries);

         /* Check if letter in word and if so update the players      */
         /* guess word and the display                                */
         if (check_word(guess, p_admin_word, &strike))
         {
            update_word(guess, p_admin_word, p_player_word);
         }
      }
      if (strike == MAX_STRIKES)
         failure_display(p_admin_word);
      else
         congrats_display(p_admin_word, tries, strike);

      /* Free the memory allocated to the linked lists                */
      while (p_letter = p_admin_word->p_next_letter ,
             p_letter->p_next_letter != NULL)
      {
         free(p_admin_word);
         p_admin_word = p_letter;
      }
      free(p_letter);
      p_letter = p_admin_word;
      while (p_letter = p_player_word->p_next_letter ,
             p_letter->p_next_letter != NULL)
      {
         //free(p_admin_word);
         p_player_word = p_letter;
      }
      free(p_letter);
      p_letter = p_admin_word;

      printf("\nWould you like to play again? (y/n):"                 );
   }

   /* Print termination message and terminate the program.            */
   printf("\nThank you using this program. Good bye"                  );
   return 0;
}

/**********************************************************************/
/*                     Print the program title                        */
/**********************************************************************/
void print_title()
{
   printf("\n    __  __ ___     _   __ ______ __  ___ ___     _   __" );
   printf("\n   / / / //   |   / | / // ____//  |/  //   |   / | / /" );
   printf("\n  / /_/ // /| |  /  |/ // / __ / /|_/ // /| |  /  |/ / " );
   printf("\n / __  // ___ | / /|  // /_/ // /  / // ___ | / /|  /  " );
   printf("\n/_/ /_//_/  |_|/_/ |_/ \\____//_/  /_//_/  |_|/_/ |_/  " );

   return;
}

/**********************************************************************/
/*                 Print the program instructions                     */
/**********************************************************************/
void print_instructions()
{
   printf("\n                    INSTRUCTIONS                       " );
   printf("\n=======================================================" );
   printf("\n=  The admin will enter the name to be guessed.       =" );
   printf("\n=  The player will guess the letters of the word      =" );
   printf("\n=  the player will loose when you complete the man    =" );
   printf("\n=======================================================" );

   return;
}

/**********************************************************************/
/*                       Create a linked list                         */
/**********************************************************************/
LETTER *create_list()
{
   LETTER *p_new_list; /* Points to the newly created list            */

   /* Get a new node and make it the list header                      */
   if ((p_new_list = (LETTER *) malloc(sizeof(LETTER))) == NULL)
   {
      printf("\nError number %d occurred in create_list()",
                                                     HEADER_ALLOC_ERR );
      printf("\nNot enough memory for the list header"                );
      printf("\nThe program is aborting"                              );
      exit(HEADER_ALLOC_ERR);
   }
   p_new_list->letter = LIST_HEADER;

   /* Get a new node and make it the list trailer                     */
   if ((p_new_list->p_next_letter = (LETTER *) malloc(sizeof(LETTER))) == NULL)
   {
      printf("\nError number %d occurred in create_list()",
                                                     TRAILER_ALLOC_ERR);
      printf("\nNot enough memory for the list trailer"               );
      printf("\nThe program is aborting"                              );
      exit(TRAILER_ALLOC_ERR);
   }
   p_new_list->p_next_letter->letter = LIST_TRAILER;
   p_new_list->p_next_letter->p_next_letter = NULL;

   return p_new_list;
}

/**********************************************************************/
/* Fill the admin list with the word and the player list with spaces  */
/**********************************************************************/
void fill_lists(LETTER *p_admin_word, LETTER *p_player_word)
{
   char   letter;          /* Every character in the word             */
   LETTER *p_admin_letter              = p_admin_word,
                           /* Points to every letter in the admin word*/
          *p_new_admin_letter,
                           /* Points to the newly created admin letter*/
          *p_new_player_letter,
                           /* Points to the newly created player      */
                           /* letter                                  */
          *p_player_letter             = p_player_word;
                           /* Points to every letter in the player    */
                           /* word                                    */

   getchar(); /* Get rid of carriage return from last input           */
   CLEAR_SCREEN;
   printf("   \nWill the admin please enter word to be guessed :"     );
   letter = getchar();
   do
   {
      /* Insert valid letter before the list trailer in admin word    */
      while (p_admin_letter->p_next_letter->letter != LIST_TRAILER)
      {
             p_admin_letter = p_admin_letter->p_next_letter;
      }
      if ((p_new_admin_letter = (LETTER *) malloc(sizeof(LETTER))) == NULL)
      {
         printf("\nError number %d occurred in fill_lists()",
                                                      LETTER_ALLOC_ERR);
         printf("\nNot enough memory for the list trailer"            );
         printf("\nThe program is aborting"                           );
         exit(LETTER_ALLOC_ERR);
      }
      p_new_admin_letter->letter = letter;
      p_new_admin_letter->p_next_letter = p_admin_letter->p_next_letter;
      p_admin_letter->p_next_letter = p_new_admin_letter;

      /* Insert a space in the player word                            */
      if ((p_new_player_letter = (LETTER *) malloc(sizeof(LETTER))) == NULL)
      {
         printf("\nError number %d occurred in fill_lists()",
                                                      LETTER_ALLOC_ERR);
         printf("\nNot enough memory for the list trailer"            );
         printf("\nThe program is aborting"                           );
         exit(LETTER_ALLOC_ERR);
      }
      p_new_player_letter->letter = '-';
      p_new_player_letter->p_next_letter = p_player_letter->p_next_letter;
      p_player_letter->p_next_letter = p_new_player_letter;

   }
   while (letter = getchar(), letter != NEW_LINE);

   return;
}


/**********************************************************************/
/*              Get the response to play again or not                 */
/**********************************************************************/
char get_admin_response()
{
   char response[2]; /* Response to continue or not                   */

   do
   {
      scanf ("%1s", response                                          );
      response[0] = tolower(response[0]);
   }
   while (response[0] != 'y' && response[0] != 'n');

   return response[0];
}

/**********************************************************************/
/*                       Print the game display                       */
/**********************************************************************/
void print_display(LETTER *p_player_word, int strike)
{
   int strike_counter; /* Counts the number of strikes                */

   printf("\n=======================================================" );
   printf("\n                    WORD: "                              );
   while (p_player_word = p_player_word->p_next_letter,
          p_player_word->letter != LIST_TRAILER)
      printf("%c", p_player_word->letter);
   printf("\n=---------------------|                               =" );
   for (strike_counter = 1; strike_counter <= strike; strike_counter++)
   {
      switch(strike_counter)
      {
         case 1 : printf("\n=                    (_)                              =");
                  break;
         case 2 : printf("\n=                    \\"                  );
                  break;
         case 3 : printf("|"                                          );
                  break;
         case 4 : printf("/                              ="           );
                  break;
         case 5 : printf("\n=                     |                               =");
                  break;
         case 6 : printf("\n=                    /"                   );
                  break;
         case 7 : printf(" \\                              ="         );
                  break;
      }
   }
   printf("\n=======================================================" );
   return;
}

/**********************************************************************/
/*                       Get the players guess                        */
/**********************************************************************/
void get_guess(char *guess, int *tries)
{
   int  guess_attempt = 0; /* The number of attempts at valid input   */
   char seccond_char;   /* Invalid second character                */

   do
   {
      if (guess_attempt == 0)
         guess_attempt++;
      else
      {
         if (isalpha(*guess))
            printf("\nInvalid input: Entered more than one letter"    );
         else
            printf("\nInvalid input: Entered a character not in alphabet.");
      }
      printf("\n Please enter a guess: "                              );
   } while (((scanf(" %c%c", guess, &seccond_char)!=2 ||
              seccond_char != NEW_LINE) && clear_stream()) ||
              !isalpha(*guess));
   (*tries)++;

   return;
}

/**********************************************************************/
/*               Check if guess is in the admin word                  */
/**********************************************************************/
bool check_word(char guess, LETTER *p_admin_word, int *strike)
{
   printf("here");
   while (p_admin_word = p_admin_word->p_next_letter,
          p_admin_word->letter != LIST_TRAILER)
   {
      if (p_admin_word->letter == guess)
         return true;
   }
   (*strike)++;

   return false;
}

/**********************************************************************/
/*           Update the player word with the matched letters          */
/**********************************************************************/
void update_word(char guess, LETTER *p_admin_word, LETTER *p_player_word)
{
   while (p_admin_word = p_admin_word->p_next_letter,
                p_admin_word->letter != LIST_TRAILER)
         {
            p_player_word = p_player_word->p_next_letter;
            if (p_player_word->letter == '-')
            {
               if (guess == p_admin_word->letter)
               {
                  p_player_word->letter = guess;
               }
            }
         }

   return;
}

/**********************************************************************/
/*          Check if the player word equals the admin word            */
/**********************************************************************/
bool check_if_win(LETTER *p_admin_word, LETTER *p_player_word)
{
   while (p_admin_word = p_admin_word->p_next_letter,
                p_admin_word->letter != LIST_TRAILER)
         {
            p_player_word = p_player_word->p_next_letter;
            if (p_player_word->letter != p_admin_word->letter)
            {
               return true;
            }
         }
   return false;
}

/**********************************************************************/
/*                  Print the display if player wins                  */
/**********************************************************************/
void congrats_display(LETTER *p_admin_word, int tries, int strike)
{
   CLEAR_SCREEN;
   printf("\n   You have Won!"                                        );
   printf("\n                                  _ "                    );
   printf("\n                                 | |"                    );
   printf("\n   ___ ___  _ __   __ _ _ __ __ _| |_ ___"               );
   printf("\n  / __/ _ \\| '_ \\ / _` | '__/ _` | __/ __|"            );
   printf("\n | (_| (_) | | | | (_| | | | (_| | |_\\__ \\ "           );
   printf("\n  \\___\\___/|_| |_|\\__, |_|  \\__,_|\\__|___/ "        );
   printf("\n                   __/ |"                                );
   printf("\n                  |___/"                                 );
   printf("\n   The word was "                                        );
   while (p_admin_word = p_admin_word->p_next_letter,
                p_admin_word->letter != LIST_TRAILER)
      printf("%c", p_admin_word->letter                               );
   printf("\nIt took you %d guesses.", tries                          );
   printf("\nYour grade :"                                            );
   switch(strike)
   {
      case 0 : printf("A+");
               break;
      case 1 : printf("A");
               break;
      case 2 : printf("B");
               break;
      case 3 : printf("C");
               break;
      case 4 : printf("D");
               break;
      default: printf("F");
   }
   sleep(5);

   return;
}

/**********************************************************************/
/*                  Print the display if player wins                  */
/**********************************************************************/
void failure_display(LETTER *p_admin_word)
{
   CLEAR_SCREEN;
   printf("\n      _______"                                           );
   printf("\n     |/      |"                                          );
   printf("\n     |      (_)"                                         );
   printf("\n     |      \\|/"                                        );
   printf("\n     |       |"                                          );
   printf("\n     |      / \\"                                        );
   printf("\n     |"                                                  );
   printf("\n  ___|___"                                               );
   printf("\n"                                                        );
   printf("\n   You have lost!"                                       );
   printf("\n   Try harder next time."                                );
   printf("\n   The word was "                                        );
   while (p_admin_word = p_admin_word->p_next_letter,
                p_admin_word->letter != LIST_TRAILER)
      printf("%c", p_admin_word->letter                               );
   sleep(5);

   return;
}

/**********************************************************************/
/*           Clear all characters from the input stream               */
/**********************************************************************/
bool clear_stream()
{
   while(getchar()!= NEW_LINE);
   return true;
}
