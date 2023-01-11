/*
 *  OpenPT - Model Importer Module
 *  File created on 2023/1/11
 *  Last edited on 2023/1/11
 *  Tianyu Huang <tianyu@illumiart.net>
 */

#ifndef IMPORTER_H
#define IMPORTER_H

#include "meshobj.h"

#include <string>
#include <vector>

namespace OpenPT
{
    class Importer
    {
    public:
        Importer() = delete;
        Importer(std::string path_);

        // https://stackoverflow.com/questions/15704565/efficient-way-to-return-a-stdvector-in-c
        std::vector<Mesh> Import();

    private:
        std::string path;
    };
};

#endif