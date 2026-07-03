#include "Score.h"

Score::Score()
{
    score = 0;
}

void Score::reset()
{
    score = 0;
}

void Score::increase()
{
    score++;
}

int Score::getScore() const
{
    return score;
}

std::string Score::getText() const
{
    return "Hyper Dash - Score: " + std::to_string(score);
}