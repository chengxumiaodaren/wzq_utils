#ifndef __CMD__
#define __CMD__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

namespace wzq {
class Command {
   public:
    static std::string RunCmd(const std::string &cmd, int32_t result_max_size = 10240) {
        char *data = new char[result_max_size];
        if (data == nullptr) {
            return std::string("");
        }
        bzero(data, sizeof(data));
        const int max_buffer = 256;
        char buffer[max_buffer];
        FILE *fdp = popen(cmd.c_str(), "r");
        int data_len = 0;
        if (fdp) {
            while (!feof(fdp)) {
                if (fgets(buffer, max_buffer, fdp)) {
                    int len = strlen(buffer);
                    if ((data_len + len) > result_max_size) {
                        break;
                    }
                    memcpy(data + data_len, buffer, len);
                    data_len += len;
                }
            }
            pclose(fdp);
        }
        std::string ret = std::string(data, data_len);
        delete[] data;
        return ret;
    }
};
}  // namespace wzq

#endif