#pragma once
// Poco
#include "Poco/Process.h"
#include "Poco/Pipe.h"

/**
 * @brief The ofxProcess class
 * ofxProcess is a simple wrapper to use Poco::Process and read/write operations for a process.
 * You can:
 *     - Launch a process
 *     - Read the output from that process
 *     - Write to the process
 *
 * **Example Usage**
 * @code
 *     std::vector<std::string> args;
 *     #if defined(_WIN32)
 *         process.setup("ipconfig", "", args);
 *     #else
 *         args.push_back("ipinfo.io/ip");
 *         process.setup("curl", "", args);
 *     #endif // defined(_WIN32)
 *
 *     process.launch();
 *     std::cout << "Output: " << process.read() << std::endl;
 * @endcode
 */
class ofxProcess
{
public:
    ofxProcess();

    /**
     * @brief Sets up the necessary variables. You NEED to call this before launching the process for the first time.
     * After that, everytime you re-launch the process the same arguments will be used. You can call this multiple times to update the variables.
     * @param processPath
     */
    void setup(const std::string &processPath, const std::string &environmentPath, const Poco::Process::Args &args);

    std::string getProcessPath() const;

    /**
     * @brief Upodates the process path. This does not re-launch the process. The next time the process is launched, the new path is used.
     * @param path
     */
    void setProcessPath(const std::string &path);

    std::string getEnvironmentPath() const;
    void setEnvironmentPath(const std::string &path);

    Poco::Process::Args getArgs() const;
    void setArgs(const Poco::Process::Args &args);

    Poco::Process::PID getPID() const;

    void launch();

    /**
     * @brief Starts the process with only the write pipe so that the process doesn't lock the thread.
     */
    void launchDetached();
    void kill();

    void requestTermination();
    void write(const std::string &message);
    std::string read() const;

private:
    Poco::Process::PID m_PID;
    Poco::Pipe m_PipeOut, m_PipeIn;

    /**
     * @brief The process path to launch
     */
    std::string m_ProcessPath;
    std::string m_EnvironmentPath;
    Poco::Process::Args m_Args;
};
