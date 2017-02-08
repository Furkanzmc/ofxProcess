#include "ofxProcess.h"
// openFrameworks
#include "ofLog.h"
// Poco
#include "Poco/PipeStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"

ofxProcess::ofxProcess()
    : m_PID()
    , m_PipeOut()
    , m_PipeIn()
    , m_ProcessPath("")
    , m_EnvironmentPath("")
    , m_Args()
{

}

void ofxProcess::setup(const std::string &processPath, const std::string &environmentPath, const Poco::Process::Args &args)
{
    m_ProcessPath = processPath;
    m_EnvironmentPath = environmentPath;
    m_Args.assign(args.begin(), args.end());
}

std::string ofxProcess::getProcessPath() const
{
    return m_ProcessPath;
}

void ofxProcess::setProcessPath(const std::string &path)
{
    m_ProcessPath = path;
}

std::string ofxProcess::getEnvironmentPath() const
{
    return m_EnvironmentPath;
}

void ofxProcess::setEnvironmentPath(const std::string &path)
{
    m_EnvironmentPath = path;
}

Poco::Process::Args ofxProcess::getArgs() const
{
    return m_Args;
}

void ofxProcess::setArgs(const Poco::Process::Args &args)
{
    m_Args = args;
}

Poco::Process::PID ofxProcess::getPID() const
{
    return m_PID;
}

void ofxProcess::launch()
{
    if (m_ProcessPath.length() == 0) {
        ofLogError("ofxIPC", "Process path is empty!");
        return;
    }

    if (m_EnvironmentPath.length() == 0) {
        ofLogWarning("ofxIPC", "Environment path is empty. Using the current path.");
        m_EnvironmentPath = Poco::Path::current();
    }

    if (Poco::Process::isRunning(m_PID)) {
        ofLogNotice("ofxIPC", "The process is already running.");
        return;
    }

    const Poco::ProcessHandle ph = Poco::Process::launch(m_ProcessPath, m_Args, m_EnvironmentPath, &m_PipeIn, &m_PipeOut, nullptr);
    m_PID = ph.id();
}

void ofxProcess::kill()
{
    if (Poco::Process::isRunning(m_PID) == false) {
        Poco::Process::kill(m_PID);
    }
}

void ofxProcess::requestTermination()
{
    if (Poco::Process::isRunning(m_PID) == false) {
        Poco::Process::requestTermination(m_PID);
    }
}

void ofxProcess::write(const string &message)
{
    if (Poco::Process::isRunning(m_PID) == false) {
        ofLogError("ofxIPC", "The process is not running.");
        return;
    }

    Poco::PipeOutputStream ostr(m_PipeIn);
    ostr << message;
    ostr.close();
}

std::string ofxProcess::read() const
{
    if (Poco::Process::isRunning(m_PID) == false) {
        ofLogWarning("ofxIPC", "The process is not running. Returning the last output from the process.");
    }

    Poco::PipeInputStream istr(m_PipeOut);
    std::string str;

    Poco::StreamCopier::copyToString(istr, str);

    return str;
}
