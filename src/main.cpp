#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>

std::vector<std::string> getWords(const char* filename)
{
    std::ifstream dictionaryFile(filename);
    std::string str;
    std::vector<std::string> v;

    while (std::getline(dictionaryFile, str))
    {
        if (!(str.empty()))
        {
            auto i = 0;
            while (str[i])
            {
                if (str[i] >= 'A' && str[i] <= 'Z')
                    str[i] = str[i] + 32;
                i += 1;
            }
            v.push_back(str);
        }
    }
    dictionaryFile.close();
    return v;
}

int main()
{
    // get words from dictionary and number of words
    auto words = getWords("/usr/share/dict/words");
    auto nWords = words.size();

    // initialisations for random picker
    unsigned long wordIndex;
    time_t *t {nullptr};

    // seed RNG with current system time
    std::mt19937 random((unsigned long) time(t));

    // randomly choose a word index from the dictionary
    if (nWords > std::mt19937::max())
    {
        // generate this way if we have more words than possible numbers in RNG
        unsigned long numMultiples = nWords / RAND_MAX;

        auto rand1 = random();
        auto rand2 = random() % numMultiples;

        wordIndex = (rand1 * rand2) % words.size();
    } else
    {
        wordIndex = random() % words.size();
    }

    // initialise game variables
    auto word = words.at(wordIndex);
    auto wordLen = word.size();
    bool guessed = false;
    bool* lettersGuessed = new bool[wordLen];
    int lives = 10;

    // game loop
    while (!guessed && lives != 0)
    {
        // output what user has currently guessed and how many lives remain
        std::cout << "You currently have: ";
        for (auto i = 0; i < wordLen; i++)
        {
            if (lettersGuessed[i])
                std::cout << word[i];
            else
                std::cout << '_';
        }
        std::cout << " with " << lives;
        if (lives == 1) std::cout << " life remaining" << std::endl;
        else std::cout << " lives remaining" << std::endl;

        // get user guess
        char c;
        std::cout << "Please enter your guess" << std::endl;
        std::cin >> c;

        // convert to lower case
        if (c < 'A' || (c > 'Z' && c < 'a') || c > 'z')
        {
            std::cout << "Invalid character, please enter an alphabetic character." << std::endl;
            continue;
        }
        else if (c >= 'A' && c <= 'Z')
        {
            c = (char) ((int) c + 32);
        }

        int contains = 0;
        // check if guess is in string one or more times
        for (auto i = 0; i < wordLen; i++)
        {
            if (word[i] == c)
            {
                contains = contains + 1;
                lettersGuessed[i] = true;
            }
        }

        // if letter was not in word, decrement lives
        if (contains == 0) lives = lives - 1;

        // check if we have guessed all letters
        int correct = 0;
        for (auto i = 0; i < wordLen; i++)
        {

            if (!(lettersGuessed[i]))
                break;
            else
                correct = correct + 1;

            if (correct == wordLen)
                guessed = true;
        }
    }
    // result output
    if (guessed)
        std::cout << "Congratulations, you correctly guessed " << word << '.' << std::endl;
    else
        std::cout << "Tough luck! You missed " << word << ". Try again." << std::endl;

    // free memory
    delete[] lettersGuessed;

    return 0;
}