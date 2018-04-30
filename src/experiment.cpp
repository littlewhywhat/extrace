#include <cstdio>
#include <cstdlib>

#include "arguments.h"
#include "argsparser.h"
#include "filesystem.h"
#include "filesystem_impl.h"
#include "kernelsystem.h"
#include "kernelsystem_impl.h"
#include "action.h"
#include "actionrunner.h"
#include "actionrunner_impl.h"

class ExtraceArguments {
  public:

    ExtraceArguments(const Arguments & arguments): m_Arguments(arguments) {}
    static void registerTo(ArgsParser & argsParser);
    static void printHelp();
    const string & getFilename() const;
    bool hasFilename() const;
    bool hasDumpFilename() const;
    const string & getDumpFilename() const;
    bool enableCompression() const;
  private:
    static const char * FILENAME_OPT;
    const Arguments & m_Arguments;
};

const char * ExtraceArguments::FILENAME_OPT = "filename";

void ExtraceArguments::registerTo(ArgsParser & argsParser) {
   argsParser.register_string("-f", FILENAME_OPT);
   argsParser.register_string("-d", "dumpfilename");
   argsParser.register_boolean("-c", "compression");
   argsParser.register_boolean("--async_start", "AsyncStart");
}

const string & ExtraceArguments::getFilename() const {
  return m_Arguments.get_string(FILENAME_OPT);
}

bool ExtraceArguments::hasFilename() const {
  return m_Arguments.has_string(FILENAME_OPT);
}

void ExtraceArguments::printHelp() {
  printf("help\n");
}

bool ExtraceArguments::hasDumpFilename() const {
  return m_Arguments.has_string("dumpfilename");
}

const string & ExtraceArguments::getDumpFilename() const {
  return m_Arguments.get_string("dumpfilename");
}

bool ExtraceArguments::enableCompression() const {
  return m_Arguments.is_enabled("compression");
}

class FileSystemCreator {
  public:
    FileSystemImpl * create() const;
};

FileSystemImpl * FileSystemCreator::create() const {
  FileSystemImpl * fileSystem = new FileSystemImpl();
  return fileSystem;
}

class Core {
  public:
    ~Core();
    void setFileSystem(FileSystem * fileSystem);
    FileSystem * getFileSystem() const;
  private:
    FileSystem * m_FileSystem;
};

Core::~Core() {
  delete m_FileSystem;
}

void Core::setFileSystem(FileSystem * fileSystem) {
  m_FileSystem = fileSystem;
}

FileSystem * Core::getFileSystem() const {
  return m_FileSystem;
}

class CoreCmdLineCreator {
  public:
    Core * createFrom(const ExtraceArguments & arguments) const;
  private:
    FileSystemCreator m_FileSystemCreator;
};

Core * CoreCmdLineCreator::createFrom(const ExtraceArguments & arguments) const {
  arguments.hasFilename();
  Core * core = new Core();
  FileSystemImpl * fileSystemImpl = m_FileSystemCreator.create();
  fileSystemImpl->set_errstream(stderr);
  core->setFileSystem(fileSystemImpl);
  return core;
}

class FileExistsAction : public Action {
  public:
    void setFileSystem(FileSystem * fileSystem);
    void setFilename(const string & filename);
    bool tryRun() override;
  private:
    FileSystem * m_FileSystem;
    string m_Filename;
};

void FileExistsAction::setFileSystem(FileSystem * fileSystem) {
  m_FileSystem = fileSystem;
}

void FileExistsAction::setFilename(const string & filename) {
  m_Filename = filename;
}

bool FileExistsAction::tryRun() {
  return m_FileSystem->fileExists(m_Filename.c_str());
}

class FileExistsActionCreator {
  public:
    FileExistsAction * createFrom(const ExtraceArguments & arguments);
};

FileExistsAction * FileExistsActionCreator::createFrom(const ExtraceArguments & arguments) {
  auto * fileExistsAction = new FileExistsAction();
  fileExistsAction->setFilename(arguments.getFilename());
  return fileExistsAction;
}

class DumpAction : public Action {
  public:
    bool tryRun() override;
    void setKernelSystem(KernelSystem * kernelSystem);
    void setErrorStream(FILE * errorStream);
    void setOutputFile(const char * outputFile);
    void setOutputStream(FILE * outStream);
    void enableCompression();
  private:
    KernelSystem * m_KernelSystem;
    FILE * m_ErrorStream;
    FILE * m_OutStream;
    string m_OutputFile;
    bool m_Compress = false;
};

void DumpAction::setKernelSystem(KernelSystem * kernelSystem) {
  m_KernelSystem = kernelSystem;
}

void DumpAction::setErrorStream(FILE * errorStream) {
  m_ErrorStream = errorStream;
}

void DumpAction::setOutputFile(const char * outputFile) {
  m_OutputFile = outputFile;
}

void DumpAction::setOutputStream(FILE * outStream) {
  m_OutStream = outStream;
}

void DumpAction::enableCompression() {
  m_Compress = true;
}

bool DumpAction::tryRun() {
  int outFd = fileno(m_OutStream);
  if (!m_OutputFile.empty()) {
    outFd = m_KernelSystem->tryOpenToWriteOrCreate(m_OutputFile.c_str());
    if (outFd == -1) {
      fprintf(m_ErrorStream, "error DumpAction::tryRun\n");
      return false;
    }
  } 
  dprintf(outFd, "TRACE:\n");

  int traceFD = m_KernelSystem->getTraceFd();
  if (traceFD == -1) {
    fprintf(m_ErrorStream, "error DumpAction::tryRun\n");
    return false;
  }
  bool ok = true;
  if (m_Compress) {
    ok &= m_KernelSystem->compress_trace_to(traceFD, outFd);
  } else {
    ok &= m_KernelSystem->try_sendfile(traceFD, outFd);
  }
  if (!ok) {
    fprintf(m_ErrorStream, "error DumpAction::tryRun\n");
  }
  close(traceFD);
  if (!m_OutputFile.empty()) {
    close(outFd);
  }
  return m_KernelSystem->clearTrace();
}

class DumpActionCreator {
  public:
    DumpAction * createFrom(const ExtraceArguments & arguments);
};

DumpAction * DumpActionCreator::createFrom(const ExtraceArguments & arguments) {
  auto * dumpAction = new DumpAction();
  if (arguments.hasDumpFilename()) {
    dumpAction->setOutputFile(arguments.getDumpFilename().c_str());
  }
  if (arguments.enableCompression()) {
    dumpAction->enableCompression();
  }
  return dumpAction;
}

class ActionBuilder {
  public:
    FileExistsAction * buildFileExistsAction(const Core & core, const ExtraceArguments & arguments) const;
    DumpAction * buildDumpAction(const Core & core, const ExtraceArguments & arguments) const;
};

FileExistsAction * ActionBuilder::buildFileExistsAction(const Core & core, const ExtraceArguments & arguments) const {
  FileExistsActionCreator fileExistsActionCreator;
  FileExistsAction * fileExistsAction = fileExistsActionCreator.createFrom(arguments);
  fileExistsAction->setFileSystem(core.getFileSystem());
  return fileExistsAction;
}

DumpAction * ActionBuilder::buildDumpAction(const Core & core, const ExtraceArguments & arguments) const {
  auto * dumpAction = DumpActionCreator().createFrom(arguments);
  dumpAction->setErrorStream(stderr);
  dumpAction->setOutputStream(stdout);
  dumpAction->setKernelSystem(core.getKernelSystem());
  return dumpAction;
}

class ActionRunnerBuilder {
  public:
    ActionRunner * buildFrom(const Core & core, const ExtraceArguments & args) const;
  private:
    ActionBuilder m_ActionBuilder;
};

ActionRunner * ActionRunnerBuilder::buildFrom(const Core & core, const ExtraceArguments & arguments) const {
  auto * actionRunner = new ActionRunnerImpl();
  if (arguments.hasFilename()) {
    actionRunner->addAction(m_ActionBuilder.buildFileExistsAction(core, arguments));
  }
  else if (arguments.enableCompression()) {
    actionRunner->addAction(m_ActionBuilder.buildDumpAction(core, arguments));
  }
  return actionRunner;
}

int main(int argc, const char** argv) {
  ArgsParser argsParser;
  Arguments arguments;
  ExtraceArguments::registerTo(argsParser);
  if (argsParser.parse(arguments, argc, argv) != argc) {
    ExtraceArguments::printHelp();
    return EXIT_FAILURE;
  }
  ExtraceArguments extraceArgs(arguments);
  CoreCmdLineCreator coreCmdLineCreator;
  Core * core = coreCmdLineCreator.createFrom(extraceArgs);
  ActionRunnerBuilder actionRunnerBuilder;
  ActionRunner * actionRunner = actionRunnerBuilder.buildFrom(*core, extraceArgs);
  if (actionRunner->tryRunActions()) {
    delete core;
    delete actionRunner;
    return EXIT_SUCCESS;
  }
  delete core;
  delete actionRunner;
  // TODO delete actions
  printf("Execution failed\n");
  return EXIT_FAILURE;
}