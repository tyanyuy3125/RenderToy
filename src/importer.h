/*
 *  OpenPT - Importer Module
 *  File created on 2023/1/11
 *  Last edited on 2023/1/11
 *  Tianyu Huang <tianyu@illumiart.net>
 */

#ifndef IMPORTER_H
#define IMPORTER_H

#include "meshobj.h"
#include "world.h"

#include <string>
#include <vector>

namespace OpenPT
{
    class OBJModelImporter
    {
    public:
        OBJModelImporter() = delete;
        OBJModelImporter(const OBJModelImporter&) = delete;
        OBJModelImporter(const OBJModelImporter&&) = delete;

        // https://stackoverflow.com/questions/15704565/efficient-way-to-return-a-stdvector-in-c
        static void Import(World &world, const std::string &path);
    };
};

#endif // IMPORTER_H