#include <iostream>
#include <cstdlib>
#include <regex>
#include <string>
#include "Markov_Link.h"


using namespace std;

/*prototypes*/
string read_text();/*read the text from the file*/
string clean_text(std::string input);/*remove all non-word characters, keeping newlines*/
Markov_Link* find_correct_link_in_chain(Markov_Link chain[30], char char_in_question);
int find_correct_character_in_link(Markov_Link* link, char proceeding_character);
int add_character_to_link(Markov_Link* link, char char_to_add);
int add_link_to_chain(Markov_Link chain[30], char char_to_add);
Markov_Link* build_chain(std::string input_text);
char get_next_char(Markov_Link* chain, char current_char);

int main() {
    srand(127);
    /*seed the random number generator with a known value to make it deterministic*/


    string text = clean_text(read_text());
    Markov_Link *chain = build_chain(text);/*chain is a pointer to an array, and needs to be deleted*/

    char post_seeding_char = get_next_char(chain, 'a');/*TODO: this is bad. pick a random char.*/
    for(int i = 0; i < 100; i++){
        printf(&post_seeding_char);
        post_seeding_char = get_next_char(chain, post_seeding_char);
    }

    delete chain;
    return EXIT_SUCCESS;
}


char get_next_char(Markov_Link* chain, char current_char){
    Markov_Link* link = find_correct_link_in_chain(chain, current_char);
    /*okay, time for a bit of clever code:
     * we use rand() to get a random value between 0 and 1
     * pass through the context once to sum the occurrences
     * pass through a second time to compute the partial sum as a fraction of the total sum
     * using this partial sum fraction, we compare it to the output of rand()
     * if that partial sum is larger, then return the character at that index
     * else iterate again, to add to the partial sum*/

    double random_value = (double)rand() / RAND_MAX; /*value between 0 and 1*/
    int total_sum = 0;
    for(int x = 0; x < 30; x++){
        total_sum += link->proceeding_char_occurance[x];
    }

    int partial_sum = 0;
    for(int x = 0; x < 30; x++){
        partial_sum += link->proceeding_char_occurance[x];
        if(((double)partial_sum / (double)total_sum) > random_value) return link->proceeding_chars[x];
    }

}


/*
 * read the text from a predetermined input file,
 * and return it as a string in all lower case
 * */
string read_text(){
    /*TODO: read file, and put it in lower case*/
    return "this\nis\nthe\ntext.\nit\nwill\nbe\nread\nfrom\na\nfile\nlater,\nbut\nright\nnow\nthis\nis\na\nplaceholder";
}

/*
 * this is a well-defined problem.  I care only about word characters, and a newline -or- space
 * */
string clean_text(std::string input){
    std::string clean_string;

    /*I'm very sure there are things wrong with this*/
    for(auto x = input.begin() ; x != input.end(); x++){
        if(isalpha(*x) || *x == '\n') clean_string.append(x.base());

    }
    return clean_string;
}


/*
 * this return value is a pointer to an array.
 * !!!My return value must be 'delete'd!!!
 * */
Markov_Link* build_chain(std::string input_text){
    Markov_Link* chain = new Markov_Link[30]();/*the '()' zeroes the memory, which I require*/
    for(auto x = input_text.begin(); x != input_text.end(); x++)
    {/*for every character in the dataset, grab the correct link, and iterate its proceeding character's count*/
        Markov_Link* link = find_correct_link_in_chain(chain, *x);

        if(x+1 != input_text.end()){/*safety checking*/
            int index = find_correct_character_in_link(link, *x);/*creates the link if needed*/
            link->proceeding_char_occurance[index]++;
        }
    }
    return chain;
}

/*
 * quick abstraction for an O(n) loop*/
Markov_Link* find_correct_link_in_chain(Markov_Link chain[30], char char_in_question){
    for(int i = 0; i < 30; i++){
        if(chain[i].my_character == char_in_question ) return &chain[i];
    }

    return &chain[add_link_to_chain(chain, char_in_question)];
}

/* analogous to the previous method, at a layer of abstraction lower.*/
int find_correct_character_in_link(Markov_Link* link, char proceeding_character){
    /*I know my link, now find the index of the next character in my context table*/
    for(int i = 0; i < 30; i++){
        if(link->proceeding_chars[i] == proceeding_character ) return i;
    }
    return add_character_to_link(link, proceeding_character);
}

/* add a proceeding character to a link's (origin character's) context table*/
int add_character_to_link(Markov_Link* link, char char_to_add){
    for(int i = 0; i < 30; i++){
        if(link->proceeding_chars > 0) continue;
        link->proceeding_chars[i] = char_to_add;
        link->proceeding_char_occurance[i] = 0;/*it is not my responsibility to increment this here*/
        return i;
    }
    return -1;
}

/* add a link (origin character) to the chain*/
int add_link_to_chain(Markov_Link chain[30], char char_to_add){
    for(int i = 0; i < 30; i++){
        if(chain[i].my_character > 0) continue;
        chain[i].my_character = char_to_add;
        return i;
    }
    return -1; /*uh oh: I couldn't add it because everything was full*/
}