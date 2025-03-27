#pragma once

#include <string>
#include <memory>
#include <functional>
#include <optional>

namespace yk {

class Data;
typedef std::shared_ptr<Data> DataPtr;

class File {
public:
    static std::shared_ptr<File> OpenForReadB(const std::string& path);
    static std::shared_ptr<File> OpenForWriteB(const std::string& path);
    static std::shared_ptr<File> OpenForAppendB(const std::string& path);

    File(const std::string& path, const std::string& mode);
    ~File();

    std::optional<uint64_t> Size();

    bool IsOpen();

    void Close();

    DataPtr Read(uint64_t size);

    std::optional<uint64_t> Read(uint8_t* buf, uint64_t len);

    std::optional<uint64_t> Write(uint8_t* buf, uint64_t len);

    std::optional<uint64_t> Write(DataPtr data);

    std::optional<bool> IsEnd();

    int GetFileDescriptor();

    void Flush();

    bool Seek(int64_t offset);

    std::optional<int64_t> GetOffset();

private:
    std::string file_path_;
    FILE* fp_ = nullptr;
};


}