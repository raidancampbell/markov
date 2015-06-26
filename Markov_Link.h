//
// Created by Aidan Campbell on 6/25/15.
//

#ifndef MARKOV_MARKOV_LINK_H
#define MARKOV_MARKOV_LINK_H


class Markov_Link {
public:/*everything's public right now because I'm lazy.*/

    /* I will commonly refer to this as an "origin character"*/
    char my_character;

    /*we're working with index-aligning here, because I'm not 100% sure how to go about this.
      * I will commonly refer to these two fields as a "context table"
      *
      * here's to hoping these two array are zeroed when I initialize Markov_Link with zeroing
      * */
    char proceeding_chars[30]; /*one of these elements is the "proceeding character"*/
    int proceeding_char_occurance[30];
};


#endif //MARKOV_MARKOV_LINK_H
