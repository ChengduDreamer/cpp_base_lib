#include "file.h"
#include <io.h> // ���� _filelengthi64 �� _fileno ������
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
        return { feof(fp_) };
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
    uint64_t read_size = fread(read_data, 1, size, fp_);
    if (read_size <= 0) {
        free(read_data);
        return nullptr;
    }
    auto data = Data::Make((char*)read_data, read_size);
    free(read_data);
    return data;
}

/*
https://blog.csdn.net/yanbober/article/details/8696628  δ����ʵ�ʲ���
��Ӧ�ļ���ָ��ƫ�ƣ��ں��������в�û��������˵���������ֵ������offset��Ϊ������ʱ�������ļ�βƫ�Ƶģ���Ϊ������ʱ�������ļ�ͷ����ƫ�Ƶģ�������Ҫ��Ҫע��ƫ�ƵĴ�С���ļ���С�߽�Ĺ�ϵ��
��offset�����ļ�β����ƫ�Ƶ�ʱ������ƫ�����Ƿ񳬳��ļ�β��fseek���Ƿ���0����ƫ����û�г����ļ�β��ʱ���ļ�ָ��ʽָ��������ƫ�Ƶ�ַ�ģ���ƫ���������ļ�β��ʱ���ļ�ָ����ָ���ļ�β�ġ������᷵��ƫ�Ƴ���-1ֵ��
��offset�����ļ�ͷ����ƫ�Ƶ�ʱ�����offsetû�г����ļ�ͷ��������ƫ�ƣ��ļ�ָ��ָ����ȷ��ƫ�Ƶ�ַ��fseek����ֵΪ0.��offset�����ļ�ͷʱ��fseek���س���-1ֵ���ļ�ָ�벻�仹�Ǵ���ԭ���ĵ�ַ
*/

bool File::Seek(int64_t offset) {
    return 0 == _fseeki64(fp_, offset, SEEK_SET);
}

}

