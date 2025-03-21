#pragma once
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace spdlog {
	class logger;
}

class YKLogger {
public:
	enum class EOutPutPosition {
		kStdOut,
		kFile
	};
	static EOutPutPosition out_position_;
	static YKLogger* Instance() {
		static YKLogger self;
		return &self;
	}
	std::shared_ptr<spdlog::logger> console_log_ptr_ = nullptr;
	std::shared_ptr<spdlog::logger> file_log_ptr_ = nullptr;
	~YKLogger();
private:
	YKLogger();
};

#define YK_LOGE(...) \
   YKLogger::Instance()->console_log_ptr_->error("[" __FUNCTION__ "] " +std::format(__VA_ARGS__)); \
   YKLogger::Instance()->file_log_ptr_->error("[" __FUNCTION__ "] " +std::format(__VA_ARGS__));

#define YK_LOGW(...) \
   YKLogger::Instance()->console_log_ptr_->warn("[" __FUNCTION__ "] " +std::format(__VA_ARGS__)); \
   YKLogger::Instance()->file_log_ptr_->warn("[" __FUNCTION__ "] " +std::format(__VA_ARGS__));


#define YK_LOGD(...) \
   YKLogger::Instance()->console_log_ptr_->debug("[" __FUNCTION__ "] " +std::format(__VA_ARGS__)); \
   YKLogger::Instance()->file_log_ptr_->debug("[" __FUNCTION__ "] " +std::format(__VA_ARGS__));


#define YK_LOGI(...) \
   YKLogger::Instance()->console_log_ptr_->info("[" __FUNCTION__ "] " +std::format(__VA_ARGS__)); \
   YKLogger::Instance()->file_log_ptr_->info("[" __FUNCTION__ "] " +std::format(__VA_ARGS__));


//std::forward<Args>(args)... 是一个参数包展开的表达式。它将参数包 args 中的每个参数进行展开，并将展开后的参数传递给 目标函数。
