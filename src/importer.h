#ifndef IMPORTER_H
#define IMPORTER_H

// TODO: refactor

#include "object.h"
#include "world.h"

#include <string>
#include <vector>

namespace RenderToy
{
    /// @brief Import Wavefront OBJ Model.
    class OBJModelImporter
    {
    public:
        OBJModelImporter() = delete;
        OBJModelImporter(const OBJModelImporter &) = delete;
        OBJModelImporter(const OBJModelImporter &&) = delete;

        static void Import(World &world, const std::string &path);
    };

    class MTLImporter
    {
    public:
        MTLImporter() = delete;

        static void Import(World &world, const std::string &path);
    };

    class TARGAImporter
    {
    public:
        TARGAImporter() = delete;
        TARGAImporter(const TARGAImporter &) = delete;
        TARGAImporter(const TARGAImporter &&) = delete;

        // static void Import(ImageTexture &image, const std::string &path);
    };
};

#endif // IMPORTER_H