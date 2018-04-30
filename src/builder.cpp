// ActionBuilder {
//   build(action_id, args);
// }

// build(action_id, args) {
//   action_builders[action_id].build(core, args);
// }

// Action * action_builders[action_id].build(core, args) {
//   SleepAction * sleepAction = SleepAction::createFrom(args);
//   sleepAction->set(core.getKernelSystem());
//   sleepAction->set(core.getAndroidSystem());
//   return sleepAction;
// }

// Core core;
// core.setAndroidSystem()
// core.setKernelSystem()
// core.setFileSystem()
// core.setTrace()
// core.setSystemTime()
// core.setSignal()

// ActionBuilder actionBuilder(core);
// actionBuilder.build(action_id, args);


/* ideally - what can't be avoided */

/* Core is build by args only */

/*
   [x] write action.run method - inherit it from Action and define class
   [x] list components that needed - using setters in the class

   [ ] add new cmdline arguments
    - [x] register arguments with same ids as in action setup

   inherit ActionCmdLineCreator class with registerTo(ArgsParser)
                                      and  createBy(arguments)
   create ActionBuilder method to build your action
   add your action Builder to dictionary of ActionBuilder

   use createBy in this new method ActionBuilder
   set components if needed as actionBuilder has core

   [x] specify how arguments setup action 
    - [x] set components dependencies
    - [x] set other properties using ids from register - duration and so on
      

 [x] specify when to add action to application action list
 this method creates sequence of actions based on provided args
*/
// ActionRunner::Builder::build(actionBuilder, args) {
//   if (args.is_enabled("CoreServices")) {
//     actionRunner.add(actionBuilder.buildAddAndroidCoreToTrace(args));
//   }
// }