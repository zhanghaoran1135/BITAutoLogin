//
// Created by God on 2023/12/12.
//

#include <windows.h>
#include <cstdio>
#include "main.h"

// 服务线程暂停时间；
#define SLEEP_TIME 5000
// 日志文件输入路径
#define LOGFILE "C:\\BITAutoLogin.txt"

int WriteToLog(const std::string& str) {
    FILE* log;
    log = fopen(LOGFILE, "a+");
    if (log == nullptr)
        return -1;
    fprintf(log, "%s\n", str.c_str());
    fclose(log);
    return 0;
}


SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE ServiceStatusHandle;

void ServiceMain(int argc, char** argv);
void ServiceControlHandler(DWORD request);
int InitService();

int main() {

    SERVICE_TABLE_ENTRY ServiceTable[2];
    ServiceTable[0].lpServiceName = (LPSTR)"BITAutoLogin";  // 服务名称
    ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

    ServiceTable[1].lpServiceName = nullptr;
    ServiceTable[1].lpServiceProc = nullptr;

    // 启动服务控制分派器
    StartServiceCtrlDispatcher(ServiceTable);
    return 0;
}

void ServiceMain(int argc, char** argv) {
    // 初始化服务类型、 状态、 接受的控制方法以及期待的返回值

    if (argc != 3 ) {
        WriteToLog("Error: Insufficient command line parameters.");
        return;
    }
    username = argv[1];
    password = argv[2];
    if (username == nullptr || password == nullptr) {
        WriteToLog("Error: Username or password not specified.");
        return;
    }


    ServiceStatus.dwServiceType = SERVICE_WIN32;
    ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    ServiceStatus.dwWin32ExitCode = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint = 0;
    ServiceStatus.dwWaitHint = 0;

    // 注册服务控制处理函数
    ServiceStatusHandle = RegisterServiceCtrlHandler(
            reinterpret_cast<LPCSTR>(L"BITAutoLogin"), // 服务名称必须与安装服务时的名称一致；
            (LPHANDLER_FUNCTION)ServiceControlHandler
    );
    if (ServiceStatusHandle == nullptr) {
        // 服务注册失败
        return;
    }
    // 初始化服务
    int error = InitService();
    if (error) {
        // 初始化服务失败， 设置服务状态为 STOPPED 并返回
        ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        ServiceStatus.dwWin32ExitCode = -1;
        SetServiceStatus(ServiceStatusHandle, &ServiceStatus);
        return;
    }
    // 现在服务已经成功运行起来了， 向服务管理器汇报状态。
    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(ServiceStatusHandle, &ServiceStatus);

    // 服务线程主循环， 每隔 5 秒钟读取系统内存状态， 输出到日志文件
    while (ServiceStatus.dwCurrentState == SERVICE_RUNNING) {
        std::string buffer = auto_login();
        int result = WriteToLog(buffer);
        if (result) {
            ServiceStatus.dwCurrentState = SERVICE_STOPPED;
            ServiceStatus.dwWin32ExitCode = -1;
            SetServiceStatus(ServiceStatusHandle, &ServiceStatus);
            return;
        }

        Sleep(SLEEP_TIME);  // 服务线程暂停 5 秒钟
    }
}

int InitService() {
    int result;
    result = WriteToLog("Monitoring started.");
    return(result);
}

void ServiceControlHandler(DWORD request) {
    switch (request) {
        case SERVICE_CONTROL_STOP:
        case SERVICE_CONTROL_SHUTDOWN:
            // 服务停止
            WriteToLog("Monitoring stopped.");
            ServiceStatus.dwWin32ExitCode = 0;
            ServiceStatus.dwCurrentState = SERVICE_STOPPED;
            break;
        default:
            break;
    }
    // 向服务管理器汇报服务状态
    SetServiceStatus(ServiceStatusHandle, &ServiceStatus);
}
