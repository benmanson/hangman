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
    auto words = getWords("/usr/share/dict/words");
    auto len = words.size();
    unsigned long wordIndex;
    time_t *t {nullptr};

    std::mt19937 random((unsigned long) time(t));

    if (len > std::mt19937::max())
    {
        unsigned long numMultiples = len / RAND_MAX;

        auto rand1 = random();
        auto rand2 = random() % numMultiples;

        wordIndex = (rand1 * rand2) % words.size();
    } else
    {
        wordIndex = random() % words.size();
    }

    auto word = words.at(wordIndex);
    auto wordLen = word.size();
    bool guessed = false;
    bool* lettersGuessed = new bool[wordLen];
    int lives = 10;

    while (!guessed && lives != 0)
    {
        std::cout << "You currently have: ";
        for (auto i = 0; i < wordLen; i++)
        {
            if (lettersGuessed[i])
                std::cout << word[i];
            else
                std::cout << '_';
        }
        std::cout << " with " << lives << " lives remaining" << std::endl;


        char c;
        std::cout << "Please enter your guess" << std::endl;
        std::cin >> c;

        int contains = 0;
        // check if we have guessed all letters
        for (auto i = 0; i < wordLen; i++)
        {
            if (word[i] == c)
            {
                contains = contains + 1;
                lettersGuessed[i] = true;
            }
        }

        if (contains == 0) lives = lives - 1;

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
    if (guessed)
        std::cout << "Congratulations, you correctly guessed " << word << std::endl;
    else
        std::cout << "Tough luck! Try again." << std::endl;
    delete[] lettersGuessed;

    return 0;
}