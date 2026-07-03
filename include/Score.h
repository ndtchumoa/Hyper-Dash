#pragma once

#include <string>

class Score
{
public:
    Score();

    void reset();
    void increase();

    int getScore() const;
    std::string getText() const;

private:
    int score;
};