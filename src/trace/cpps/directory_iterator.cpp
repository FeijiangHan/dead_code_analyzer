/* **********************************************************
 * Copyright (c) 2017-2022 Google, Inc.  All rights reserved.
 * **********************************************************/

/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Google, Inc. nor the names of its contributors may be
 *   used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL VMWARE, INC. OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

#include <algorithm>
#include "../headers/directory_iterator.h"
#include "../headers/utils.h"

// Following typical stream iterator convention, the default constructor
// produces an EOF object.
directory_iterator_t::directory_iterator_t(const std::string &directory)
{
    dir_ = opendir(directory.c_str());
    if (dir_ == nullptr) {
        error_descr_ = "Failed to access directory.";
        return;
    }
    ++*this;
    at_eof_ = false;
}

directory_iterator_t::~directory_iterator_t()
{
    if (dir_ != nullptr)
        closedir(dir_);
}

// Work around clang-format bug: no newline after return type for single-char operator.
// clang-format off
const std::string &
directory_iterator_t::operator*()
// clang-format on
{
    return cur_file_;
}

directory_iterator_t &
directory_iterator_t::operator++()
{
    ent_ = readdir(dir_);
    if (ent_ == nullptr)
        at_eof_ = true;
    else
        cur_file_ = ent_->d_name;
    return *this;
}

bool
directory_iterator_t::is_directory(const std::string &path)
{
    DIR *pdir;
    bool is_dir = false;

    pdir = opendir(path.c_str());
    if (pdir != nullptr) {
        is_dir = true;
        (void)closedir(pdir);
    }
    return is_dir;
}
