#include<string>
using namespace std;
struct Suggestion
{
    string word;
    int frequency;
    bool operator<(const Suggestion &other) const
    {
        if (frequency != other.frequency)
            return frequency < other.frequency;   

        return word > other.word;                
    }
};