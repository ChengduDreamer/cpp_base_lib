#ifndef DATA_H
#define DATA_H

#include <memory>
#include <string>

namespace yk
{

    class Data {
    public:
        static std::shared_ptr<Data> Make(const char* data, int size);
        static std::shared_ptr<Data> From(const std::string& data);

        Data(const char *data, int size);
        ~Data();

        const char *CStr();
        char* DataAddr();
        int Size();
        char At(uint64_t offset);
        std::string AsString();
        void ConvertToStr(std::string& out);
        std::shared_ptr<Data> Dup();
        bool Append(char* data, int size);
        int Offset();
        void Reset();
        void Save(const std::string& path);

    private:

        char* data_{nullptr};
        int   size_ = 0;
        int offset_ = 0;
};


typedef std::shared_ptr<Data> DataPtr;


}
#endif // DATA_H
