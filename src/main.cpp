#include <iostream>
#include <cstring>
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"
#include <vector>
#include "TcpServer.h"
#include <QtCore/QCoreApplication>
#include "thread"
#include <signal.h>

//#include <g3log/g3log.hpp>
//#include <g3log/logworker.hpp>
//using namespace g3;

TcpServer *tcpServer;

void startListen(int port)
{
    tcpServer = new TcpServer(port);
}

/*
std::unique_ptr<g3::LogWorker> logworker;

void init_g3log() {

    // 日志路径
    const std::string path_to_log_file = "./logs/";
    // 日志前缀
    const string log_file = "log_dump";

    // 创建日志对象
    logworker = g3::LogWorker::createLogWorker();

    std::string strModule = "aggregation";
    // 设置日志路径
    auto sinkHandle = logworker->addSink(std::make_unique<FileSink>(log_file, path_to_log_file, strModule), &FileSink::fileWrite);

    // 初始化日志对象
    initializeLogging(logworker.get());

    // 生成日志文件名
    std::future<std::string> log_file_name = sinkHandle->call(&FileSink::fileName);

}
*/

void init_log()
{
    try 
    {
        auto logger = spdlog::basic_logger_mt("basic_logger", "logs/edgeplat.log");
        spdlog::set_default_logger(logger);
//        spdlog::set_an
        // change log pattern
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [thread %t] [%@] > %v");
        SPDLOG_INFO("global log with source info"); 
        spdlog::flush_every(std::chrono::seconds(5));

        // auto console = spdlog::stdout_logger_mt("console");
        // spdlog::set_default_logger(console);
        // spdlog::set_pattern("[source %s] [function %!] [line %#] %v");

        // SPDLOG_LOGGER_INFO(console, "log with source info"); 
        spdlog::set_level((spdlog::level::level_enum)1);

        int log = spdlog::get_level();
        SPDLOG_INFO("log leve:{:d}!", log);

        std::cout << "Log init succeed!"<<std::endl;
    }
    catch (const spdlog::spdlog_ex &ex)
    {
        std::cout << "Log init failed: " << ex.what() << std::endl;
    }
}

static void show_usage(std::string name)
{
    std::cerr << "Usage: app <option(s)> SOURCES"
              << "Options:\n"
              << "\t-h,--help\t\tShow this help message\n"
              << "\t-d,--destination DESTINATION\tSpecify the destination path"
              << std::endl;
}

void close_handle(int signal){
	printf("Received signal %d!  Closing.......\n", signal);
	emit tcpServer->close();
	QCoreApplication::exit();
}

int main(int argc, char* argv[])
{
//    system("chcp 65001");
    // 注册信号 SIGINT 和信号处理程序
    signal( SIGTERM, close_handle);
    signal( SIGILL, close_handle);
    if (argc < 3) {
            show_usage(argv[0]);
            return -1;
    }
    int port = 8080;
    for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if ((arg == "-h") || (arg == "--help")) {
                show_usage(argv[0]);
                return 0;
            } else if ((arg == "-p") || (arg == "--port")) {
                if (i + 1 <= argc) { // Make sure we aren't at the end of argv!
                    port = std::stoi(argv[++i]); // Increment 'i' so we don't get the argument as the next argv[i].
                } else { // Uh-oh, there was no argument to the destination option.
                      std::cerr << "--destination option requires one argument." << std::endl;
                    return -1;
                }
            }
        }

    QCoreApplication a(argc, argv);

    init_log();
//    init_g3log();
    startListen(port);

    SPDLOG_INFO("Spdlog version {}.{}.{}", SPDLOG_VER_MAJOR,SPDLOG_VER_MINOR,SPDLOG_VER_PATCH);
    // spdlog::enable_backtrace(10); //创建一个环形buffer可以容纳10个消息
    // spdlog::dump_backtrace(); //开始记日志
    SPDLOG_INFO("Node started.");

    // spdlog::shutdown(); //释放所有资源
    
    return a.exec(); 
}
