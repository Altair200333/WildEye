#pragma once

namespace BD
{
    void createBin(std::string path)
    {
        unsigned char a[1] = { 1 };
        std::ifstream inp(path, std::ios::binary);
        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(inp), {});
        std::ofstream s("out.txt");
        s << "unsigned char audio[" << buffer.size() << "] = {";

        for (size_t i = 0; i < buffer.size(); ++i)
        {
            s << static_cast<int>(buffer[i]);
            if (i != buffer.size() - 1)
                s << ", ";
        }
        s << "};";
        s.flush();
    }
	
}
