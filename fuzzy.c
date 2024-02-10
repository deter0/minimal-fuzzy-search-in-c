#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#include "fuzzy.h"

int fuzzy_search(const char *text, const char *query, bool build_score, int **score, size_t *score_len) {
  // score = int[text_size]{0};
  // For t, t_idx in text
    // For q in query
      // If q matches t
        // score[t_idx]++
      // For each subsequent match (streak)
        // score[t_idx] + streak*4
      // t_idx += streak

  size_t total_score = 0;
  if (build_score) { // Build score is an optimization when searching through large database
    (*score) = (int*)malloc(sizeof(int) * strlen(text));
    memset(*score, 0, sizeof(int)*strlen(text));
    *score_len = strlen(text);
  }

  size_t first_character_boosts = 1;

  for (size_t t_idx = 0; t_idx < strlen(text); t_idx++) {
    char t = tolower(text[t_idx]); // NOTE(deter0): to lower performs kind of strangely probably due to UTF8
    for (size_t q_idx = 0; q_idx < strlen(query); q_idx++) {
      char q = tolower(query[q_idx]);

      if (t == q) {
       // Start of word awards more but falls off fast
        if (t_idx == 0 || (t_idx > 0 && isspace(text[t_idx - 1]))) {
          int factor = 8/(first_character_boosts++);
          
          if (build_score) (*score)[t_idx] += factor;
          total_score += factor;
        } else { 
          if (build_score) (*score)[t_idx]++;
          total_score++;
        }
        
        size_t streak = 0;
        for (size_t s_idx = 1; s_idx < strlen(query)-q_idx; s_idx++) {
          char sq = tolower(query[q_idx + s_idx]);
          char st = tolower(text[t_idx + s_idx]);

          if (sq != st) break;
          streak++;
          // Beginning of string yields few points more; eg -> "Term" :: "Terminus", "Fluent Terminal"
          if (((float)t_idx / (float)strlen(text)) <= 0.35) {
            streak++;
          }

          int factor = streak*3/(strlen(query)*0.2);
          if (build_score)
            (*score)[t_idx + s_idx] += factor;
          total_score += factor;
        }

        // (N * (N + 1) ) /2
        // (*score)[t_idx] += (streak * (streak + 1)) / 2;
        t_idx += streak;
      }
    }
  }

  return total_score;
}

const char *apps[] = {
    "alacritty",
    "babun",
    "cmder",
    "color tool",
    "iterm",
    "conemu",
    "console z",
    "fluent terminal",
    "hyper",
    "kitty",
    "moba xterm",
    "mremoteng",
    "mtputty",
    "putty",
    "terminus",
    "windows terminal",
    "firefox web browser",
    "vim",
    "chormium",
    "GNU Image Manipulation Program",
    "こんにちは",
    "Οὐχὶ ταὐτὰ παρίσταταί"
};

const char *queries[] = {
  "gimp",
  "forefpx",
  "firefox",
  "ft",
  "Οὐχὶ",
};

#define ITERATIONS_BENCH (1)
int main(void) {
  for (size_t qi = 0; qi < sizeof(queries)/sizeof(*queries); qi++) {
    const char *search_query = queries[qi];

    int *score = NULL;
    size_t score_len = 0;
    int max_score = 0;
    const char *text = "N/A";

    clock_t t; 
    t = clock();

    for (int it = 0; it < ITERATIONS_BENCH; it++) {
      if (score) free(score);
      score = NULL;
      max_score = 0;
      
      for (size_t i = 0; i < sizeof(apps)/sizeof(*apps); i++) {
        int *score_tmp;
        size_t score_len_tmp;
        
        int total_score = fuzzy_search(apps[i], search_query, true, &score_tmp, &score_len_tmp);
        if (total_score > max_score) {
          if (score) free(score);
          
          score = score_tmp;
          score_len = score_len_tmp;
          text = apps[i];

          max_score = total_score;
        } else {
          free(score_tmp);
        }
      }
    }

    t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
  
    printf("took %f seconds to execute %d times.\n", time_taken, ITERATIONS_BENCH); 

    printf("Query: `%s`. Result: `%s`. Total score: %d\n", search_query, text, max_score);

    for (size_t i = 0; i < score_len; i++) {
      printf("%c: ", text[i]);
      for (int j = 0; j <= score[i]; j++) {
        printf("=");
      }
      printf("\n");
    }
    
    free(score);

    printf("\n=====================================\n\n");
  }
  return 0;
}

