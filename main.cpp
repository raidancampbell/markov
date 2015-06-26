#include <iostream>
#include <cstdlib>
#include <regex>
#include <fstream>
#include "Markov_Link.h"


static const int VOCAB_LENGTH = 30;
using namespace std;

/*prototypes*/
string read_text();/*read the text from the file*/
string clean_text(std::string input);/*remove all non-word characters, keeping newlines*/
int find_correct_link_in_chain(Markov_Link chain[VOCAB_LENGTH], char char_in_question);
int find_correct_character_in_link(Markov_Link chain[], int link_index, char proceeding_character);
int add_character_to_link(Markov_Link chain[], int link_index, char proceeding_character);
int add_link_to_chain(Markov_Link chain[VOCAB_LENGTH], char char_to_add);
Markov_Link* build_chain(std::string input_text);
char get_next_char(Markov_Link* chain, char current_char);
void zero_chain(Markov_Link chain[VOCAB_LENGTH]);

int main() {
    srand(127);
    /*seed the random number generator with a known value to make it deterministic*/


    string text = clean_text(read_text());
    Markov_Link *chain = build_chain(text);/*chain is a pointer to an array, and needs to be deleted*/
    cout << "seeding with character '";
    cout << chain[0].my_character;
    cout << "'\r\n";
    char post_seeding_char = get_next_char(chain, chain[0].my_character);
    for(int i = 0; i < 100; i++){
        cout << post_seeding_char;
        post_seeding_char = get_next_char(chain, post_seeding_char);
    }

    delete[] chain;
    return EXIT_SUCCESS;
}


char get_next_char(Markov_Link* chain, char current_char){
    int link_index = find_correct_link_in_chain(chain, current_char);
    /*okay, time for a bit of clever code:
     * we use rand() to get a random value between 0 and 1
     * pass through the context once to sum the occurrences
     * pass through a second time to compute the partial sum as a fraction of the total sum
     * using this partial sum fraction, we compare it to the output of rand()
     * if that partial sum is larger, then return the character at that index
     * else iterate again, to add to the partial sum*/

    double random_value = (double)rand() / RAND_MAX; /*value between 0 and 1*/
    int total_sum = 0;
    for(int x = 0; x < VOCAB_LENGTH; x++){
        total_sum += chain[link_index].proceeding_char_occurance[x];
    }

    int partial_sum = 0;
    for(int x = 0; x < VOCAB_LENGTH; x++){
        partial_sum += chain[link_index].proceeding_char_occurance[x];
        if(((double)partial_sum / (double)total_sum) >= random_value) {
            return chain[link_index].proceeding_chars[x];
        }
    }
    cerr << "ERROR: no next character found for '";
    cerr << current_char;
    cerr << "' in 'get_next_char'\r\n";
    return -1;
}


/*
 * read the text from a predetermined input file,
 * and return it as a string
 * */
string read_text(){
    std::ifstream t("input.txt");
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string str = buffer.str();
    return str;
}

/*
 * this is a well-defined problem.  I care only about word characters, and a newline -or- space
 * */
string clean_text(std::string input){
    std::string clean_string;

    /*I'm very sure there are things wrong with this*/
    for(auto x = input.begin() ; x != input.end(); x++){
        if(isalpha(*x) || *x == '\n' || *x == ' ') clean_string.append(x.base());
        else {
            cerr << "BAD CHARACTER ENCOUNTERED: '";
            cerr << *x;
            cerr << "'\r\n";
        }
    }
    return clean_string;
}


/*
 * this return value is a pointer to an array.
 * !!!My return value must be 'delete'd!!!
 * */
Markov_Link* build_chain(std::string input_text){
    Markov_Link* chain = new Markov_Link[VOCAB_LENGTH]();/*the '()' zeroes the memory, which I require*/
    zero_chain(chain);

    for(auto x = input_text.begin(); x != input_text.end(); x++)
    {/*for every character in the dataset, grab the correct link, and iterate its proceeding character's count*/
        int link_index = find_correct_link_in_chain(chain, *x);

        if(x+1 != input_text.end()){/*safety checking*/
            int index = find_correct_character_in_link(chain, link_index, *(x+1));/*creates the link if needed*/
            chain[link_index].proceeding_char_occurance[index]++;
        }
    }
    return chain;
}

/*
 * quick abstraction for an O(n) loop*/
int find_correct_link_in_chain(Markov_Link chain[VOCAB_LENGTH], char char_in_question){
    for(int i = 0; i < VOCAB_LENGTH; i++){
        if(chain[i].my_character == char_in_question ) return i;
    }

    return add_link_to_chain(chain, char_in_question);
}

/* analogous to the previous method, at a layer of abstraction lower.*/
int find_correct_character_in_link(Markov_Link chain[], int link_index, char proceeding_character){
    /*I know my link, now find the index of the next character in my context table*/
    for(int i = 0; i < VOCAB_LENGTH; i++){
        if(chain[link_index].proceeding_chars[i] == proceeding_character ) return i;
    }
    return add_character_to_link(chain, link_index, proceeding_character);
}

/* add a proceeding character to a link's (origin character's) context table*/
int add_character_to_link(Markov_Link chain[], int link_index, char char_to_add){
    for(int i = 0; i < VOCAB_LENGTH; i++){
        if(chain[link_index].proceeding_chars[i] > 0) continue;
        chain[link_index].proceeding_chars[i] = char_to_add;
        chain[link_index].proceeding_char_occurance[i] = 0;/*it is not my responsibility to increment this here*/
        return i;
    }
    cerr << "ERROR: unable to add proceeding character to origin character's context table!";
    return -1;
}

/* add a link (origin character) to the chain */
int add_link_to_chain(Markov_Link chain[VOCAB_LENGTH], char char_to_add){
    for(int i = 0; i < VOCAB_LENGTH; i++){
        if(chain[i].my_character > 0) continue;
        chain[i].my_character = char_to_add;
        return i;
    }
    cerr << "ERROR: could not add link to chain!";
    return -1; /*uh oh: I couldn't add it because everything was full*/
}


void zero_chain(Markov_Link chain[VOCAB_LENGTH]){
    for(int i = 0; i< VOCAB_LENGTH; i++){
        chain[i].my_character = 0;
        for(int j =0; j< VOCAB_LENGTH; j++){
            chain[i].proceeding_char_occurance[j] =0;
            chain[i].proceeding_chars[j] = 0;
        }
    }
}