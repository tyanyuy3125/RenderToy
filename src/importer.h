/*
 *  RenderToy - Importer Module
 *  File created on 2023/1/11
 *  Last edited on 2023/1/11
 *  Tianyu Huang <tianyu@illumiart.net>
 */

#ifndef IMPORTER_H
#define IMPORTER_H

#include "object.h"
#include "world.h"

#include <string>
#include <vector>

namespace RenderToy
{
    class OBJModelImporter
    {
    public:
        OBJModelImporter() = delete;
        OBJModelImporter(const OBJModelImporter &) = delete;
        OBJModelImporter(const OBJModelImporter &&) = delete;

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