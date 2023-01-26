#ifndef RANDOM_H
#define RANDOM_H

namespace RenderToy
{
    class Random
    {
    public:
        Random() = delete;

        static const int Int(int min, int max);
        static const float Float();
    };
}

#endif // RANDOM_H