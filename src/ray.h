#include "mathfunc.h"

namespace OpenPT
{
    class Ray
    {
        public:
            Ray(Vector3f src_, Vector3f direction_);

            Vector3f src;
            Vector3f direction;
    };
}