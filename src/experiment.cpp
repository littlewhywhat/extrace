#include <cstdio>
#include <cstdlib>

#include "arguments.h"
#include "argsparser.h"
#include "filesystem.h"
#include "filesystem_impl.h"
#include "action.h"
#include "actionrunner.h"
#include "actionrunner_impl.h"

class ExtraceArguments {
  public:
    ExtraceArguments(const Arguments & arguments): m_Arguments(arguments) {}
    void registerTo(ArgsParser & argsParser);
    const string & getFilename() const;
    bool hasFilename() const;
    void printHelp() const;
  private:
    const Arguments & m_Arguments;
};

void ExtraceArguments::registerTo(ArgsParser & argsParser) {
   argsParser.register_string("-f", "filename");
}

const string & ExtraceArguments::getFilename() const {
  return m_Arguments.get_string("filename");
}

bool ExtraceArguments::hasFilename() const {
  return m_Arguments.has_string("filename");
}

void ExtraceArguments::printHelp() const {
  printf("help\n");
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

class ActionBuilder {
  public:
    FileExistsAction * buildFileExistsAction(const Core & core, const ExtraceArguments & arguments) const;
};

FileExistsAction * ActionBuilder::buildFileExistsAction(const Core & core, const ExtraceArguments & arguments) const {
  FileExistsActionCreator fileExistsActionCreator;
  FileExistsAction * fileExistsAction = fileExistsActionCreator.createFrom(arguments);
  fileExistsAction->setFileSystem(core.getFileSystem());
  return fileExistsAction;
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
  return actionRunner;
}

int main(int argc, const char** argv) {
  ArgsParser argsParser;
  Arguments arguments;
  ExtraceArguments extraceArgs(arguments);
  extraceArgs.registerTo(argsParser);
  if (argsParser.parse(arguments, argc, argv) != argc) {
    extraceArgs.printHelp();
    return EXIT_FAILURE;
  }
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