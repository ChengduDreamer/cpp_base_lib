#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <list>
#include <sstream>

namespace yk {

typedef std::function<void()> VoidFunc;

enum ThreadTaskState {
    kIdle,
    kRunning,
    kReady,
};

class ThreadTask {
public:
    ThreadTask() = default;
    virtual ~ThreadTask() = default;
    virtual void Run() = 0;
public:
    uint64_t task_id_ = 0;
    ThreadTaskState state_ = ThreadTaskState::kIdle;
};

class SimpleThreadTask : public ThreadTask {
public:

    static std::shared_ptr<SimpleThreadTask> Make(VoidFunc&& ef) {
        return std::make_shared<SimpleThreadTask>(std::move(ef), []() {});
    }

    static std::shared_ptr<SimpleThreadTask> Make(VoidFunc&& ef, VoidFunc&& cbk) {
        return std::make_shared<SimpleThreadTask>(std::move(ef), std::move(cbk));
    }

    SimpleThreadTask(VoidFunc&& ef, VoidFunc&& cbk) {
        exec_func_ = ef;
        callback_ = cbk;
    }

    void Run() override {
        if (exec_func_) {
            exec_func_();
        }
        if (callback_) {
            callback_();
        }
    }

private:
    VoidFunc exec_func_;
    VoidFunc callback_;

};

template <class ExecFunc, class CallbackFunc>
class ReturnThreadTask : public ThreadTask {
public:

    static std::shared_ptr<ReturnThreadTask> Make(ExecFunc ef, CallbackFunc cbk) {
        return std::make_shared<ReturnThreadTask>(ef, cbk);
    }

    ReturnThreadTask(ExecFunc ef, CallbackFunc cbk) {
        exec_func_ = ef;
        callback_ = cbk;
    }

    void Run() override {
        if (exec_func_) {
            auto ret = exec_func_();
            if (callback_) {
                callback_(ret);
            }
        }
    }

private:
    ExecFunc exec_func_;
    CallbackFunc callback_;

};


typedef std::shared_ptr<ThreadTask> ThreadTaskPtr;
typedef std::function<void()>       OnceTask;

class Thread
{
public:
    static std::shared_ptr<Thread> Make(const std::string& name, int max_task);

    Thread() = delete;
    explicit Thread(const std::string& name, int max_task = -1);
    Thread(OnceTask&& task, const std::string& name, bool join = true);
    virtual ~Thread();

    void Poll();

    void Post(const ThreadTaskPtr& task);
    void Post(ThreadTaskPtr&& task);
    void Post(std::function<void()>&& task);
    bool RemoveTask(uint64_t task_id);
    bool TaskExists(uint64_t task_id);
    bool HasTask();
    int TaskSize();
    int MaxTaskSize();
    std::list<ThreadTaskPtr> GetTasks();
    void Exit();

    bool IsExit();
    bool IsLastTaskReturned();
    bool Joinable();
    void Join();

    unsigned long ExecCount();

    std::string GetId();

private:

    void TaskLoop();

    std::mutex init_mtx_;
    bool init_{false};

    std::mutex task_mtx_;
    std::list<ThreadTaskPtr> tasks_;

    std::shared_ptr<std::thread>  thread_;

    std::condition_variable take_var_;

    bool exit_{false};
    bool exit_loop_{false};

    int max_tasks_ = -1;
    bool last_task_returned_{false};
    unsigned long task_exec_count_{0};

    std::string name_;
};


typedef std::shared_ptr<Thread>  ThreadPtr;


}

