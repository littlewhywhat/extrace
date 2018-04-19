AndroidSystem androidSystem = new AndroidSystem();

AndroidApp * coreService = androidSystem.getCoreService();

AndroidComponent androidComponent = new AndroidComponent();
androidComponent.addApp(new AndroidApp().setName("messenger"));
androidComponent.addApp(new AndroidApp().setName("gmail"));
androidComponent.addCategory(new AndroidTraceCategory().setTag(ATRACE_TAG_WEBVIEW));
androidComponent.setAndroidSystem(androidSystem);

Trace trace = new Trace();
trace.addComponent(androidComponent);

TraceAction startAction = new StartTrace();

// TraceSession traceSession = TraceSessionBuilder.build(args);
AsyncTraceSession asyncSession = new AsyncTraceSession();
asyncSession.setAction(startAction);
asyncSession.setTrace(trace);

AtraceApp app
app.setTraceSession(asyncSession);
app.startSession();