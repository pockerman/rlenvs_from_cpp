// SPDX-FileCopyrightText: 2024 <copyright holder> <email>
// SPDX-License-Identifier: Apache-2.0

#include "rlenvs/utils/io/json_file_reader.h"
#include "rlenvs/utils/io/file_formats.h"

namespace rlenvscpp{
namespace utils{
namespace io{

JSONFileReader::JSONFileReader(const std::string& filename)
    :
FileReaderBase(filename, FileFormats::Type::JSON),
data_()
{}

void
JSONFileReader::open(){

    this -> FileReaderBase::open();
    auto& f = this->get_file_stream();
    data_ = json::parse(f);
}

}
}
}
