#ifndef __OWNSTRINGS__
#define __OWNSTRINGS__

#include "common/noncopyable.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace wzq {

class OwnedStrings : wzq::NonCopyAble {
   public:
    OwnedStrings(const std::vector<std::string> &src) {
        char_ptr_vec.reserve(src.size() + 1);
        std::transform(src.begin(), src.end(), char_ptr_vec.begin(), [](const std::string &str) {
            char *buffer = new char[str.size() + 1];
            std::copy(str.begin(), str.end(), buffer);
            buffer[str.size()] = 0;
            return buffer;
        });
        char_ptr_vec.push_back(nullptr);
    }

    char **data() { return char_ptr_vec.data(); }

    ~OwnedStrings() {
        for (char *elem : char_ptr_vec) {
            if (elem != nullptr) {
                delete[] elem;
            }
        }
    }

   private:
    std::vector<char *> char_ptr_vec;
};

}  // namespace wzq

#endif