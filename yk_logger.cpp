#include "yk_logger.h"
#include "path_util.h"
#include "str_util.h"

YKLogger::EOutPutPosition YKLogger::out_position_ = YKLogger::EOutPutPosition::kFile;

YKLogger::YKLogger() {
	std::string log_flag = "yk";
	std::wstring pathw = yk::GetModulePathW(NULL);
	std::string path = yk::ToUTF8(pathw);
	console_log_ptr_ = spdlog::stdout_color_mt(log_flag);
	console_log_ptr_->set_level(spdlog::level::info);
	if (EOutPutPosition::kFile == out_position_) {
		auto max_size = 1048576 * 5 * 10;
		auto max_files = 20;
		std::wstring file_name = pathw + YK_PATH_SEP + L"yk_log.txt";
		file_log_ptr_ = spdlog::rotating_logger_mt(log_flag + "_log", file_name, max_size, max_files);
		file_log_ptr_->set_level(spdlog::level::info);
		spdlog::flush_every(std::chrono::seconds(5));
	}
}

YKLogger::~YKLogger() {
	
}
