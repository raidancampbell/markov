# Markov
a barebones C++ Markov Chain implementation

- This is a first-order character-level Markov chain implementation.  
- It doesn't have any interface, save for an implicityly assumed input file "input.txt"
- It has character whitelisting in place for producing a 'clean' result.  It accepts only alphabetical characters, '\n' and ' '

I would recommend using a larger input file, as the current one (a verse of jingle bells) will fail once it hits the character 'y'
(this is the last character in the string, so no proceeding characters have been observed).
