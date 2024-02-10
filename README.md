# Minimal Fuzzy Search in C Implementation

Fuzzy search in C optimized for searching applications it works with UTF-8, at least on Linux.

It is `O(n*m)` where `n` is the query length and `m` is the text you are trying to find the score off. You can make it multi-threaded pretty easily. Splitting your database and running the function for the split views of the databse, getting the maximum score of the view & comparing with other threads' views on the main thread will yield you the best match. 

* Q `gimp` => R `GNU Image Manipulation Program`.
* `firefox` => `firefox web browser`.
* `ft` => `fluent terminal`.
* `forefpx` => `firefox web browser`.

## Details

### Definition
`int fuzzy_search(const char *text, const char *query, bool build_score, int **score, size_t *score_len)`

Total score is returned by function, there isn't really a set range of values as it depends on the text size. Maybe in the future I would want to make it return 0->1.

`build_score` parameter is to fill out `int **score` & `size_t *score_len` this is mainly used for building heatmaps. See the test result for an example of heatmap. 

