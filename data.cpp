#include "data.h"

#ifdef WIN32
#ifdef ENABLE_JEMALLOC
#include "jemalloc/jemalloc.h"
#endif
#endif

namespace yk
{

    Data::Data(const char* src, int size) {
#ifdef WIN32
#ifdef ENABLE_JEMALLOC
        this->data_ = (char*)je_malloc(size);
#else
        this->data_ = (char *) malloc(size);
#endif
#else
        this->data_ = (char *) malloc(size);
#endif
        if (src) {
            memcpy(this->data_, src, size);
        }
        this->size_ = size;
    }

    std::shared_ptr<Data> Data::From(const std::string& data) {
        return std::make_shared<Data>((char*)data.data(), (int)data.size());
    }

    Data::~Data() {
        if (this->data_) {
#ifdef WIN32
#ifdef ENABLE_JEMALLOC
            je_free(this->data_);
#else
            free(this->data_);
#endif
#else
            free(this->data_);
#endif

        }
    }

    const char *Data::CStr() {
        return this->data_;
    }

    std::string Data::AsString() {
        std::string val;
        val.resize(size_);
        memcpy(val.data(), this->data_, size_);
        return val;
    }

    void Data::ConvertToStr(std::string& out) {
        out.resize(size_);
        memcpy((char*)out.data(), this->data_, size_);
    }

    int Data::Size() {
        return this->size_;
    }

    std::shared_ptr<Data> Data::Make(const char *data_, int size) {
        return std::make_shared<Data>(data_, size);
    }

    char Data::At(uint64_t offset) {
        return *(this->data_ + offset);
    }

    char* Data::DataAddr() {
        return this->data_;
    }

    std::shared_ptr<Data> Data::Dup() {
        return Data::Make(this->data_, this->size_);
    }

    bool Data::Append(char* data, int size) {
        if (offset_ + size > size_) {
            return false;
        }
        memcpy(data_ + offset_, data, size);
        offset_ += size;
        return true;
    }

    int Data::Offset() {
        return offset_;
    }

    void Data::Reset() {
        offset_ = 0;
    }

    void Data::Save(const std::string& path) {
//        auto file = File::OpenForWriteB(path);
//        file->Write(0, data_, size_);
//        file->Close();
    }

}
