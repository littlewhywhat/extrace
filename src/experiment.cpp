#include "arguments.h"
#include "toolbox.h"
#include "androidtoolbox.h"

#include <memory>
#include <cstdio>

using namespace std;

#include <string>
#include <vector>

class CmdLineArgs {
  public:
    CmdLineArgs(int argc, const char** argv);
    uint32_t getCount() const;
    const string & get(uint32_t id) const;
  private:
    vector<string> m_Args;
};

CmdLineArgs::CmdLineArgs(int argc, const char** argv) {
  for (int i = 0; i < argc; i++) {
    m_Args.push_back(argv[i]);
  }
}

uint32_t CmdLineArgs::getCount() const {
  return m_Args.size();
}

const string & CmdLineArgs::get(uint32_t id) const {
  return m_Args[id];
}

//! A parser of command line arguments
class CmdLineArgsParser {
  public:
    CmdLineArgsParser(const shared_ptr<ToolBox> & toolBox);
    //! Registers boolean option with specified name under provided specifier
    void register_boolean(const char * specifier, const char * option_name);
    //! Registers integer option with specified name under provided specifier
    void register_integer(const char * specifier, const char * option_name);
    //! Registers string option with specified name under provided specifier
    void register_string(const char * specifier, const char * option_name);
    //! Registers comma separated values list under provided optionName
    void registerCommaSepList(const char * specifier, const char * optionName);
    //! Parses argv using list of registered options to arguments
    // Returns index of last processed arg from argv or argc value if processed
    // all without error
    uint32_t parse(Arguments & arguments, const CmdLineArgs & cmdLineArgs);
  private:
    shared_ptr<ToolBox> m_ToolBox;
    //! Stores registered boolean options
    std::map<std::string, std::string> boolean_opt_names;
    //! Stores registered integer options
    std::map<std::string, std::string> integer_opt_names;
    //! Stores registered string options
    std::map<std::string, std::string> string_opt_names;
    //! Stores registered comma separated list of options
    map<string, string> m_CommaSepListOptNames;
    //! Checks if specifier is registered in booleans
    bool is_boolean_spec(const char * spec);
    //! Checks if specifier is registered in integers
    bool is_integer_spec(const char * spec);
    //! Checks if specifier is registered in strings
    bool is_string_spec(const char * spec);
    //! Checks if specifier is registered in CommaSepListSpec
    bool isCommaSepListSpec(const char * specifier) const;    
};

CmdLineArgsParser::CmdLineArgsParser(const shared_ptr<ToolBox> & toolBox) {
  m_ToolBox = toolBox;
}

bool CmdLineArgsParser::is_boolean_spec(const char * spec)
{
  return this->boolean_opt_names.find(spec) != this->boolean_opt_names.end();
}

bool CmdLineArgsParser::is_integer_spec(const char * spec)
{
  return this->integer_opt_names.find(spec) != this->integer_opt_names.end();
}

bool CmdLineArgsParser::is_string_spec(const char * spec)
{
  return this->string_opt_names.find(spec) != this->string_opt_names.end();
}

bool CmdLineArgsParser::isCommaSepListSpec(const char * specifier) const {
  return m_CommaSepListOptNames.find(specifier) != m_CommaSepListOptNames.end();
}

void CmdLineArgsParser::register_boolean(const char * specifier, const char * option_name)
{
  this->boolean_opt_names[specifier] = option_name;
}

void CmdLineArgsParser::register_integer(const char * specifier, const char * option_name)
{
  this->integer_opt_names[specifier] = option_name;
}

void CmdLineArgsParser::register_string(const char * specifier, const char * option_name)
{
  this->string_opt_names[specifier] = option_name;
}

void CmdLineArgsParser::registerCommaSepList(const char * specifier, const char * optionName) {
  m_CommaSepListOptNames[specifier] = optionName;
}

uint32_t CmdLineArgsParser::parse(Arguments & arguments, const CmdLineArgs & cmdLineArgs)
{
  uint32_t i = 0;
  while (i < cmdLineArgs.getCount())
  {
    const char * arg = cmdLineArgs.get(i).c_str();
    if (is_boolean_spec(arg))
    {
      arguments.put_boolean(this->boolean_opt_names[arg].c_str());
      i++;
      continue;
    }
    if ((i+1) == cmdLineArgs.getCount())
    {
      break;
    }
    const char * optarg = cmdLineArgs.get(i+1).c_str();
    if (is_integer_spec(arg)) {
      arguments.put_integer(this->integer_opt_names[arg].c_str(), std::stoi(optarg));
    }
    else if (is_string_spec(arg)) {
      arguments.put_string(this->string_opt_names[arg].c_str(), optarg);
    }
    else if (isCommaSepListSpec(arg)) {
      set<string> tokens;
      m_ToolBox->parseToTokens(optarg, ",", tokens);
      for (auto & token : tokens) {
        arguments.putToStringList(m_CommaSepListOptNames[arg].c_str(), token);
      }
    }
    else
    {
      break;
    }
    i+=2;
  }
  return i;
}

class Wire {
  public:
    Wire(FILE * outputStream, FILE * errorStream):
         m_OutputStream(outputStream), m_ErrorStream(errorStream) {}
    FILE * getOutputStream() const;
    FILE * getErrorStream() const;
  private:
    FILE * m_OutputStream = NULL;
    FILE * m_ErrorStream = NULL;
};

FILE * Wire::getOutputStream() const {
  return m_OutputStream;
}

FILE * Wire::getErrorStream() const {
  return m_ErrorStream;
}

/************/

using namespace std;

class Signal {
  public:
    void fire();
    bool isFired() const;
  private:
    bool m_Fired = false;
};

void Signal::fire() {
  m_Fired = true;
}

bool Signal::isFired() const {
  return m_Fired;
}

/************/

class Environment {
  public:
    Environment(const Signal & signal, const string & name):
                m_Signal(signal), m_Name(name) {}
    bool isInterrupted() const;
    const string & getName() const;
  private:
    const Signal & m_Signal;
    const string & m_Name;
};

bool Environment::isInterrupted() const {
  return m_Signal.isFired();
}

const string & Environment::getName() const {
  return m_Name;
}

/************/

class TraceArguments {
  public:
    TraceArguments();
    void setHelpMessage(const string & message);
    void enableCircleBuffer();
    void enableIgnoreSignals();
    void enableCompression();
    void enableAsyncStart();
    void enableAsyncStop();
    void enableAsyncDump();
    void enableStream();
    void enableListCategories();
    void enableCoreServices();
    void setKernelCategoryFilename(const string & filename);
    void setOutputFilename(const string & filename);
    void setBufferSize(uint32_t size);
    void setInitSleepDuration(uint32_t duration);
    void setMidSleepDuration(uint32_t duration);
    void setApps(const vector<string> & apps);
    void setAndroidCategories(const vector<string> & categories);
    void setKernelCategories(const vector<string> & categories);
    void setKernelFunctions(const vector<string> & functions);
    bool hasHelpMessage() const;
    bool hasKernelCategoryFilename() const;
    bool hasOutputFilename() const;
    bool hasApps() const;
    bool hasAndroidCategories() const;
    bool hasKernelCategories() const;
    bool hasKernelFunctions() const;
    bool circleBufferEnabled() const;
    bool ignoreSignalsEnabled() const;
    bool compressionEnabled() const;
    bool asyncStartEnabled() const;
    bool asyncStopEnabled() const;
    bool asyncDumpEnabled() const;
    bool streamEnabled() const;
    bool listCategoriesEnabled() const;
    bool coreServicesEnabled() const;
    const string & getKernelCategoryFilename() const;
    const string & getOutputFilename() const;
    const string & getHelpMessage() const;
    uint32_t getBufferSize() const;
    uint32_t getInitSleepDuration() const;
    uint32_t getMidSleepDuration() const;
    const vector<string> getApps() const;
    const vector<string> & getAndroidCategories() const;
    const vector<string> & getKernelCategories() const;
    const vector<string> & getKernelFunctions() const;
  private:
    bool m_CircleBuffer = false;
    bool m_IgnoreSignals = false;
    bool m_Compression = false;
    bool m_AsyncStart = false;
    bool m_AsyncStop = false;
    bool m_AsyncDump = false;
    bool m_Stream = false;
    bool m_ListCategories = false;
    bool m_CoreServices = false;
    string m_HelpMessage;
    string m_OutputFilename;
    string m_KernelCategoryFilename;
    uint32_t m_BufferSize = 1024;
    uint32_t m_InitSleepDuration = 0;
    uint32_t m_MidSleepDuration = 5;
    vector<string> m_Apps;
    vector<string> m_AndroidCategories;
    vector<string> m_KernelCategories;
    vector<string> m_KernelFunctions;
};

void TraceArguments::setHelpMessage(const string & message) {
  m_HelpMessage = message;
}

void TraceArguments::enableCircleBuffer() {
  m_CircleBuffer = true;
}

void TraceArguments::enableIgnoreSignals() {
  m_IgnoreSignals = true;
}

void TraceArguments::enableCompression() {
  m_Compression = true;
}

void TraceArguments::enableAsyncStart() {
  m_AsyncStart = true;
}

void TraceArguments::enableAsyncStop() {
  m_AsyncStop = true;
}

void TraceArguments::enableAsyncDump() {
  m_AsyncDump = true;
}

void TraceArguments::enableStream() {
  m_Stream = true;
}

void TraceArguments::enableListCategories() {
  m_ListCategories = true;
}

void TraceArguments::enableCoreServices() {
  m_CoreServices = true;
}

void TraceArguments::setKernelCategoryFilename(const string & filename) {
  m_KernelCategoryFilename = filename;
}

void TraceArguments::setOutputFilename(const string & filename) {
  m_OutputFilename = filename;
}

void TraceArguments::setBufferSize(uint32_t size) {
  m_BufferSize = size;
}

void TraceArguments::setInitSleepDuration(uint32_t duration) {
  m_InitSleepDuration = duration;
}

void TraceArguments::setMidSleepDuration(uint32_t duration) {
  m_MidSleepDuration = duration;
}

void TraceArguments::setApps(const vector<string> & apps) {
  m_Apps = apps;
}

void TraceArguments::setAndroidCategories(const vector<string> & categories) {
  m_AndroidCategories = categories;
}

void TraceArguments::setKernelCategories(const vector<string> & categories) {
  m_KernelCategories = categories;
}

void TraceArguments::setKernelFunctions(const vector<string> & functions) {
  m_KernelFunctions = functions;
}

bool TraceArguments::hasHelpMessage() const {
  return !m_HelpMessage.empty();
}
bool TraceArguments::hasKernelCategoryFilename() const {
  return !m_KernelCategoryFilename.empty();
}
bool TraceArguments::hasOutputFilename() const {
  return !m_OutputFilename.empty();
}
bool TraceArguments::hasApps() const {
  return !m_Apps.empty();
}
bool TraceArguments::hasAndroidCategories() const {
  return !m_AndroidCategories.empty();
}
bool TraceArguments::hasKernelCategories() const {
  return !m_KernelCategories.empty();
}
bool TraceArguments::hasKernelFunctions() const {
  return !m_KernelFunctions.empty();
}

bool TraceArguments::circleBufferEnabled() const {
  return m_CircleBuffer;
}

bool TraceArguments::ignoreSignalsEnabled() const {
  return m_IgnoreSignals;
}

bool TraceArguments::compressionEnabled() const {
  return m_Compression;
}

bool TraceArguments::asyncStartEnabled() const {
  return m_AsyncStart;
}

bool TraceArguments::asyncStopEnabled() const {
  return m_AsyncStop;
}

bool TraceArguments::asyncDumpEnabled() const {
  return m_AsyncDump;
}

bool TraceArguments::streamEnabled() const {
  return m_Stream;
}

bool TraceArguments::listCategoriesEnabled() const {
  return m_ListCategories;
}

bool TraceArguments::coreServicesEnabled() const {
  return m_CoreServices;
}

const string & TraceArguments::getKernelCategoryFilename() const {
  return m_KernelCategoryFilename;
}

const string & TraceArguments::getOutputFilename() const {
  return m_OutputFilename;
}

uint32_t TraceArguments::getBufferSize() const {
  return m_BufferSize;
}

uint32_t TraceArguments::getInitSleepDuration() const {
  return m_InitSleepDuration;
}

uint32_t TraceArguments::getMidSleepDuration() const {
  return m_MidSleepDuration;
}

const vector<string> TraceArguments::getApps() const {
  return m_Apps;
}

const vector<string> & TraceArguments::getAndroidCategories() const {
  return m_AndroidCategories;
}

const vector<string> & TraceArguments::getKernelCategories() const {
  return m_KernelCategories;
}

const vector<string> & TraceArguments::getKernelFunctions() const {
  return m_KernelFunctions;
}

const string & TraceArguments::getHelpMessage() const {
  return m_HelpMessage;
}

TraceArguments::TraceArguments() {
}

/************/

class TraceSystem {
  public:
    TraceSystem(const Wire & wire, const TraceArguments & traceArguments): m_Wire(wire) {}
  private:
    const Wire & m_Wire;
};

/************/

class Action {
  public:
    virtual ~Action() {}
    virtual bool tryRunIn(Environment & environment) = 0;
};

/************/

class TraceAction {
  public:
    TraceAction(const Wire & wire, const TraceArguments & traceArguments): m_Wire(wire) {
      if (traceArguments.hasHelpMessage()) {
        m_HelpMessage = traceArguments.getHelpMessage();
      }
    }
    bool tryRunIn(Environment & environment, TraceSystem & system);
  private:
    const Wire & m_Wire;
    string m_HelpMessage;
};

bool TraceAction::tryRunIn(Environment & environment, TraceSystem & system) {
  if (m_HelpMessage.empty())
    fprintf(m_Wire.getErrorStream(), "Hello %s\n", environment.getName().c_str());
  else
    fprintf(m_Wire.getErrorStream(), m_HelpMessage.c_str(), environment.getName().c_str());
  return true;
}

/************/

class TraceActionsRunner : public Action {
  public:
    TraceActionsRunner(const Wire & wire, TraceSystem * traceSystem, const TraceArguments & traceArguments):
                       m_Wire(wire), m_TraceSystem(traceSystem) {};
    ~TraceActionsRunner();
    void addTraceAction(TraceAction * traceAction);
    bool tryRunIn(Environment & environment) override;
  private:
    const Wire & m_Wire;
    TraceSystem * m_TraceSystem = NULL;
    vector<TraceAction*> m_TraceActions;
};

TraceActionsRunner::~TraceActionsRunner() {
  delete m_TraceSystem;
  for (auto * traceAction : m_TraceActions) {
    delete traceAction;
  }
}

void TraceActionsRunner::addTraceAction(TraceAction * traceAction) {
  m_TraceActions.push_back(traceAction);
}

bool TraceActionsRunner::tryRunIn(Environment & environment) {
  for (auto * traceAction : m_TraceActions) {
    if (!traceAction->tryRunIn(environment, *m_TraceSystem)) {
      return false;
    }
  }
  return true;
}

/************/

class ActionRunner {
  public:
    ActionRunner(Action * action): m_Action(action) {}
    ~ActionRunner();
    bool tryRunIn(Environment & environment);
  private:
    Action * m_Action = NULL;
};

ActionRunner::~ActionRunner() {
  delete m_Action;
}

bool ActionRunner::tryRunIn(Environment & environment) {
  if (m_Action) {
    return m_Action->tryRunIn(environment);
  }
  return false;
}

/************/

class ActionCmdLineBuilder {
  public:
    virtual ~ActionCmdLineBuilder() {}
    virtual Action * build(const Wire & wire, const CmdLineArgs & cmdLineArgs) const = 0;
};

/************/

#include <memory>

static const char * HELP_OPTION_NAME = "Help";
static const char * CIRCLE_BUFFER_OPTION_NAME = "CircleBuffer";
static const char * IGNORE_SIGNALS_OPTION_NAME = "IgnoreSignals";
static const char * COMPRESSED_OPTION_NAME = "Compressed";
static const char * ASYNC_START_OPTION_NAME = "AsyncStart";
static const char * ASYNC_STOP_OPTION_NAME = "AsyncStop";
static const char * ASYNC_DUMP_OPTION_NAME = "AsyncDump";
static const char * STREAM_OPTION_NAME = "Stream";
static const char * LIST_CATEGORIES_OPTION_NAME = "ListCategories";
static const char * CORE_SERVICES_OPTION_NAME = "CoreServices";
static const char * KERNEL_CATEG_FILE_OPTION_NAME = "KernelCategoriesFilename";
static const char * OUT_FILE_OPTION_NAME = "OutputFile";
static const char * BUFFER_SIZE_OPTION_NAME = "BufferSize";
static const char * INIT_SLEEP_OPTION_NAME = "InitSleep";
static const char * MID_SLEEP_OPTION_NAME = "MidSleep";
static const char * APPS_OPTION_NAME = "Apps";
static const char * ANDROID_CATEG_OPTION_NAME = "AndroidCategories";
static const char * KERNEL_CATEG_OPTION_NAME = "KernelCategories";
static const char * KERNEL_FUNC_OPTION_NAME = "KernelFunctions";
static const string HELP_MESSAGE = "usage: %s [options]\n"
         "options include:\n"
         "  -a appname      enable app-level tracing for a comma\n"
         "                  separated list of cmdlines\n"
         "  -b N            use a trace buffer size of N KB\n"
         "  -c              trace into a circular buffer\n"
         "  -d aname,...    trace the listed android categories\n"
         "  -e kname,...    trace the listed kernel categories\n"
         "  -f filename     use the categories written in a file as space-separated\n"
         "                    values in a line\n"
         "  -k fname,...    trace the listed kernel functions\n"
         "  -n              ignore signals\n"
         "  -s N            sleep for N seconds before tracing [default 0]\n"
         "  -t N            trace for N seconds [defualt 5]\n"
         "  -z              compress the trace dump\n"
         "  --async_start   start circular trace and return immediatly\n"
         "  --async_dump    dump the current contents of circular trace buffer\n"
         "  --async_stop    stop tracing and dump the current contents of circular\n"
         "                    trace buffer\n"
         "  --stream        stream trace to outstream as it enters the trace buffer\n"
         "                    Note: this can take significant CPU time, and is best\n"
         "                    used for measuring things that are not affected by\n"
         "                    CPU performance, like pagecache usage.\n"
         "  --list_categories\n"
         "                  list the available tracing categories\n"
         " -o filename      write the trace to the specified file instead\n"
         "                    of outstream.\n"
         " --acore          add core services.\n";

class TraceArgumentsBuilder {
  public:
    const TraceArguments * build(const Wire & wire, const CmdLineArgs & cmdLineArgs) const;
  private:
    void registerCmdLineOpts(CmdLineArgsParser & cmdLineArgsParser) const;
    TraceArguments * createHelpTraceArguments() const;
    TraceArguments * createTraceArguments(const Arguments & arguments) const;
    const string & getHelpMessage() const;
};

void TraceArgumentsBuilder::registerCmdLineOpts(CmdLineArgsParser & cmdLineArgsParser) const {
  cmdLineArgsParser.register_boolean("--help", HELP_OPTION_NAME);
  cmdLineArgsParser.register_boolean("-c", CIRCLE_BUFFER_OPTION_NAME);
  cmdLineArgsParser.register_boolean("-n", IGNORE_SIGNALS_OPTION_NAME);
  cmdLineArgsParser.register_boolean("-z", COMPRESSED_OPTION_NAME);
  cmdLineArgsParser.register_boolean("--async_start", ASYNC_START_OPTION_NAME);
  cmdLineArgsParser.register_boolean("--async_stop", ASYNC_STOP_OPTION_NAME);
  cmdLineArgsParser.register_boolean("--async_dump", ASYNC_DUMP_OPTION_NAME);
  cmdLineArgsParser.register_boolean("--stream", STREAM_OPTION_NAME);
  cmdLineArgsParser.register_boolean("--list_categories", LIST_CATEGORIES_OPTION_NAME);
  cmdLineArgsParser.register_boolean("--acore", CORE_SERVICES_OPTION_NAME);
  cmdLineArgsParser.register_string("-f", KERNEL_CATEG_FILE_OPTION_NAME);
  cmdLineArgsParser.register_string("-o", OUT_FILE_OPTION_NAME);
  cmdLineArgsParser.register_integer("-b", BUFFER_SIZE_OPTION_NAME);
  cmdLineArgsParser.register_integer("-s", INIT_SLEEP_OPTION_NAME);
  cmdLineArgsParser.register_integer("-t", MID_SLEEP_OPTION_NAME);
  cmdLineArgsParser.registerCommaSepList("-a", APPS_OPTION_NAME);
  cmdLineArgsParser.registerCommaSepList("-d", ANDROID_CATEG_OPTION_NAME);
  cmdLineArgsParser.registerCommaSepList("-e", KERNEL_CATEG_OPTION_NAME);
  cmdLineArgsParser.registerCommaSepList("-k", KERNEL_FUNC_OPTION_NAME);
}

TraceArguments * TraceArgumentsBuilder::createHelpTraceArguments() const {
  TraceArguments * traceArguments = new TraceArguments();
  traceArguments->setHelpMessage(getHelpMessage());
  return traceArguments;
}

TraceArguments * TraceArgumentsBuilder::createTraceArguments(const Arguments & arguments) const {
  TraceArguments * traceArguments = new TraceArguments();
  if (arguments.is_enabled(HELP_OPTION_NAME)) {
    traceArguments->setHelpMessage(getHelpMessage());
  }
  if (arguments.is_enabled(CIRCLE_BUFFER_OPTION_NAME)) {
    traceArguments->enableCircleBuffer();
  }
  if (arguments.is_enabled(IGNORE_SIGNALS_OPTION_NAME)) {
    traceArguments->enableIgnoreSignals();
  }
  if (arguments.is_enabled(COMPRESSED_OPTION_NAME)) {
    traceArguments->enableCompression();
  }
  if (arguments.is_enabled(ASYNC_START_OPTION_NAME)) {
    traceArguments->enableAsyncStart();
  }
  if (arguments.is_enabled(ASYNC_STOP_OPTION_NAME)) {
    traceArguments->enableAsyncStop();
  }
  if (arguments.is_enabled(ASYNC_DUMP_OPTION_NAME)) {
    traceArguments->enableAsyncDump();
  }
  if (arguments.is_enabled(STREAM_OPTION_NAME)) {
    traceArguments->enableStream();
  }
  if (arguments.is_enabled(LIST_CATEGORIES_OPTION_NAME)) {
    traceArguments->enableListCategories();
  }
  if (arguments.is_enabled(CORE_SERVICES_OPTION_NAME)) {
    traceArguments->enableCoreServices();
  }
  if (arguments.has_string(KERNEL_CATEG_FILE_OPTION_NAME)) {
    auto & filename = arguments.get_string(KERNEL_CATEG_FILE_OPTION_NAME);
    traceArguments->setKernelCategoryFilename(filename);
  }
  if (arguments.has_string(OUT_FILE_OPTION_NAME)) {
    auto & filename = arguments.get_string(OUT_FILE_OPTION_NAME);
    traceArguments->setOutputFilename(filename);
  }
  if (arguments.has_integer(BUFFER_SIZE_OPTION_NAME)) {
    traceArguments->setBufferSize(arguments.get_integer(BUFFER_SIZE_OPTION_NAME));
  }
  if (arguments.has_integer(INIT_SLEEP_OPTION_NAME)) {
    const auto & duration = arguments.get_integer(INIT_SLEEP_OPTION_NAME);
    traceArguments->setInitSleepDuration(duration);
  }
  if (arguments.has_integer(MID_SLEEP_OPTION_NAME)) {
    const auto & duration = arguments.get_integer(MID_SLEEP_OPTION_NAME);
    traceArguments->setMidSleepDuration(duration);
  }
  if (arguments.hasStringList(APPS_OPTION_NAME)) {
    const auto & list = arguments.getStringList(APPS_OPTION_NAME);
    traceArguments->setApps(list);
  }
  if (arguments.hasStringList(ANDROID_CATEG_OPTION_NAME)) {
    const auto & list = arguments.getStringList(ANDROID_CATEG_OPTION_NAME);
    traceArguments->setAndroidCategories(list);
  }
  if (arguments.hasStringList(KERNEL_CATEG_OPTION_NAME)) {
    const auto & list = arguments.getStringList(KERNEL_CATEG_OPTION_NAME);
    traceArguments->setKernelCategories(list);
  }
  if (arguments.hasStringList(KERNEL_FUNC_OPTION_NAME)) {
    const auto & list = arguments.getStringList(KERNEL_FUNC_OPTION_NAME);
    traceArguments->setKernelFunctions(list);
  }
  return traceArguments;
}

const TraceArguments * TraceArgumentsBuilder::build(const Wire & wire, const CmdLineArgs & cmdLineArgs) const {
  CmdLineArgsParser cmdLineArgsParser(make_shared<AndroidToolBox>());
  registerCmdLineOpts(cmdLineArgsParser);
  Arguments arguments;
  if (cmdLineArgsParser.parse(arguments, cmdLineArgs) != cmdLineArgs.getCount()) {
    return createHelpTraceArguments();
  }
  auto * traceArguments = createTraceArguments(arguments);
  if (traceArguments->hasHelpMessage()
       || traceArguments->listCategoriesEnabled()
       || traceArguments->hasKernelCategoryFilename()
       || traceArguments->hasKernelCategories()
       || traceArguments->hasAndroidCategories()
       || traceArguments->hasApps()
       || traceArguments->hasKernelFunctions()) {
    return traceArguments;
  }
  traceArguments->setHelpMessage(getHelpMessage());
  return traceArguments;
}

const string & TraceArgumentsBuilder::getHelpMessage() const {
  return HELP_MESSAGE;
}

/************/

class TraceSystemBuilder {
  public:
    TraceSystem * build(const Wire & wire, const TraceArguments & traceArguments) const;
};

TraceSystem * TraceSystemBuilder::build(const Wire & wire, const TraceArguments & traceArguments) const {
  return new TraceSystem(wire,traceArguments);
}

/************/

class TraceActionsRunnerBuilder {
  public:
    TraceActionsRunner * build(const Wire & wire, TraceSystem * traceSystem,
                               const TraceArguments & traceArguments) const;
};

TraceActionsRunner * TraceActionsRunnerBuilder::build(const Wire & wire, TraceSystem * traceSystem,
                                                      const TraceArguments & traceArguments) const {
  auto * traceActionsRunner = new TraceActionsRunner(wire, traceSystem, traceArguments);
  traceActionsRunner->addTraceAction(new TraceAction(wire, traceArguments));
  return traceActionsRunner;
}

/************/

class TraceActionsRunnerCmdLineBuilder : public ActionCmdLineBuilder {
  public:
    TraceActionsRunnerCmdLineBuilder(TraceArgumentsBuilder     * traceArgumentsBuilder,
                                     TraceSystemBuilder        * traceSystemBuilder,
                                     TraceActionsRunnerBuilder * traceActionsRunner):
                                     m_TraceArgumentsBuilder(traceArgumentsBuilder),
                                     m_TraceSystemBuilder(traceSystemBuilder),
                                     m_TraceActionsRunnerBuilder(traceActionsRunner) {}

    Action * build(const Wire & wire, const CmdLineArgs & cmdLineArgs) const override;
  private:
    TraceArgumentsBuilder * m_TraceArgumentsBuilder = NULL;
    TraceSystemBuilder    * m_TraceSystemBuilder = NULL;
    TraceActionsRunnerBuilder * m_TraceActionsRunnerBuilder = NULL;
};

Action * TraceActionsRunnerCmdLineBuilder::build(const Wire & wire, const CmdLineArgs & cmdLineArgs) const {
  auto   sp_traceArguments  = unique_ptr<const TraceArguments>(m_TraceArgumentsBuilder->build(wire, cmdLineArgs));
  auto * traceSystem        = m_TraceSystemBuilder->build(wire, *sp_traceArguments);
  auto * traceActionsRunner = m_TraceActionsRunnerBuilder->build(wire, traceSystem, *sp_traceArguments);
  return traceActionsRunner;
}

/************/

#include <cstdlib>
#include <cstdio>

class CmdLineApp {
  public:
    CmdLineApp(): m_Wire(new Wire(stdout, stderr)) {}
    ~CmdLineApp();
    int run();
    void setArgs(const CmdLineArgs * cmdLineArgs);
    void setActionCmdLineBuilder(ActionCmdLineBuilder * actionCmdLineBuilder);
    void setAppName(const char * appName);
    void handleSignal();
  private:
    Wire * m_Wire = NULL;
    const CmdLineArgs * m_CmdLineArgs = NULL;
    const ActionCmdLineBuilder * m_ActionCmdLineBuilder = NULL;
    Signal m_Signal;
    string m_AppName;
};

void CmdLineApp::handleSignal() {
  m_Signal.fire();
}

void CmdLineApp::setArgs(const CmdLineArgs * cmdLineArgs) {
  m_CmdLineArgs = cmdLineArgs;
}

void CmdLineApp::setActionCmdLineBuilder(ActionCmdLineBuilder * actionCmdLineBuilder) {
  m_ActionCmdLineBuilder = actionCmdLineBuilder;
}

void CmdLineApp::setAppName(const char * appName) {
  m_AppName = appName;
}

CmdLineApp::~CmdLineApp() {
  delete m_CmdLineArgs;
  delete m_Wire;
}

int CmdLineApp::run() {
  if (!m_CmdLineArgs || !m_ActionCmdLineBuilder) {
    return EXIT_FAILURE;
  }
  Environment environment(m_Signal, m_AppName);
  ActionRunner actionRunner(m_ActionCmdLineBuilder->build(*m_Wire, *m_CmdLineArgs));
  if (actionRunner.tryRunIn(environment)) {
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}

/************/

CmdLineApp cmdLineApp;

void handleSignal(int /*signo*/)
{
  cmdLineApp.handleSignal();
}

#include <signal.h>

void registerSignalHandler()
{
  struct sigaction sa;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handleSignal;
  sigaction(SIGHUP, &sa, NULL);
  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGQUIT, &sa, NULL);
  sigaction(SIGTERM, &sa, NULL);
}

int main(int argc, const char ** argv) {
  cmdLineApp.setAppName(argv[0]);
  cmdLineApp.setArgs(new CmdLineArgs(argc-1, argv+1));
  cmdLineApp.setActionCmdLineBuilder(new TraceActionsRunnerCmdLineBuilder(
                                         new TraceArgumentsBuilder(),    // TODO
                                         new TraceSystemBuilder(),       // TODO
                                         new TraceActionsRunnerBuilder() // TODO
                                    ));
  registerSignalHandler();
  return cmdLineApp.run();
}