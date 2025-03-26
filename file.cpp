#include "file.h"
#include <io.h> // 包含 _filelengthi64 和 _fileno 的声明
#include <memory>
#include "string_ext.h"
#include "data.h"

namespace yk {

std::shared_ptr<File> File::OpenForReadB(const std::string& path) {
    return std::make_shared<File>(path.c_str(), "rb");
}

File::File(const std::string& path, const std::string& mode) {
    std::string origin_path = path;
    StringExt::Replace(origin_path, "\\", "/");
    this->file_path_ = origin_path;
#ifdef WIN32
    std::wstring origin_pathw = StringExt::ToWString(origin_path);
    std::wstring modew = StringExt::ToWString(mode);
    _wfopen_s(&fp_, origin_pathw.c_str(), modew.c_str());
#endif
}

File::~File() {
    Close();
}

std::optional<uint64_t> File::Size() {
    if (!IsOpen()) {
        return {};
    }
#ifdef WIN32 
   /* _fseeki64(fp_, 0, SEEK_END);
    auto size = _ftelli64(fp_);
    _fseeki64(fp_, 0, SEEK_SET);*/

    int fd = _fileno(fp_);
    if (-1 == fd) {
        perror("Failed to get file descriptor");
        return {};
    }
    auto file_size = _filelengthi64(fd);
    if (-1 == file_size) {
        return {};
    }
    return { file_size };
#endif
    //return size;
}

std::optional<bool> File::IsEnd() {
    if (fp_) {
        return { feof(fp_) }; // feof: 文件结束 返回非0值
    }
    return {};
}

bool File::IsOpen() {
    return fp_ != nullptr;
}

void File::Flush() {
    if (fp_) {
        fflush(fp_);
    }
}

void File::Close() {
    if (fp_) {
        fflush(fp_);
        fclose(fp_);
        fp_ = nullptr;
    }
}

int File::GetFileDescriptor() {
    if (fp_) {
#ifdef WIN32
        return _fileno(fp_);
#endif
    }
    return -1;
}

std::optional<uint64_t> File::Read(uint8_t* buf, uint64_t len) {
    if (!IsOpen()) {
        return {};
    }
    uint64_t readed_size = fread(buf, 1, len, fp_);
    return { readed_size };
}

DataPtr File::Read(uint64_t size) {
    if (!IsOpen()) {
        return nullptr;
    }
    uint8_t* read_data = (uint8_t*)malloc(size);
    if (!read_data) {
        // malloc error
        return nullptr;
    }
    std::shared_ptr<void> auto_free(nullptr, [=](void*) {
        free(read_data);
    });
    uint64_t read_size = fread(read_data, 1, size, fp_);
    if (read_size <= 0) {
        return nullptr;
    }
    return Data::Make((char*)read_data, read_size);
}

/*
https://blog.csdn.net/yanbober/article/details/8696628  未经过实际测试
对应文件的指针偏移，在函数定义中并没有真正的说明清楚返回值，对于offset当为正数的时候是向文件尾偏移的，当为负数的时候是向文件头方向偏移的，这里主要是要注意偏移的大小和文件大小边界的关系。
当offset是向文件尾方向偏移的时候，无论偏移量是否超出文件尾，fseek都是返回0，当偏移量没有超出文件尾的时候，文件指针式指向正常的偏移地址的，当偏移量超出文件尾的时候，文件指针是指向文件尾的。并不会返回偏移出错-1值。
当offset是向文件头方向偏移的时候，如果offset没有超出文件头，是正常偏移，文件指针指向正确的偏移地址，fseek返回值为0.当offset超出文件头时，fseek返回出错-1值，文件指针不变还是处于原来的地址
*/

bool File::Seek(int64_t offset) {
    return 0 == _fseeki64(fp_, offset, SEEK_SET);
}

std::optional<int64_t> File::GetOffset() {
    // 获取当前文件指针的偏移量
    int64_t current_pos = _ftelli64(fp_);
    if (-1 == current_pos) {
        return {};
    }
    return { current_pos };
}

}

