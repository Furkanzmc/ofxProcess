# ofxProcess

ofxProcess is a simple wrapper to use Poco::Process and read/write operations for a process.

You can:
    - Launch a process
    - Read the output from that process
    - Write to the process


# Example

```c++
void ofApp::setup()
{
    std::vector<std::string> args;

#if defined(_WIN32)
    m_Process.setup("ipconfig", "", args);
#else
    args.push_back("ipinfo.io/ip");
    m_Process.setup("curl", "", args);
#endif // defined(_WIN32)

    m_Process.launch();
}

void ofApp::keyReleased(int key)
{
    if (key == 'r') {
        std::cout << "Reading...\n" << std::flush;
        std::cout << m_Process.read() << std::endl;
    }
}
```
