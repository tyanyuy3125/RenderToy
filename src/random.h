#ifndef RANDOM_H
#define RANDOM_H

namespace OpenPT
{
    class Random
    {
    public:
        Random();

        int Int() const;
        float Float() const;

    private:
        mutable int z_m[4];
    };
}

#endif // RANDOM_H