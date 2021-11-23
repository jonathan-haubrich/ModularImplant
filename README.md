## Modular Implant

### Modules
 - Registered via a callback
 - Modules know their type
 - Shared header defining types
 - Required modules:
   - Loader
   - Comms
   - Crypto
   - Control
 - Rest are functional modules
   - Interface:
     - InitModule export with local and remote initialization
     - GetModuleInfo export
       - Returns a MODULE_INFO that contains module name/description and list of FUNCTION_INFO
       - FUNCTION_INFO contains name, description, parameters, function pointer
       - Uses CONTAINING_RECORD, LIST_ENTRY or SINGLE_LIST_ENTRY, and STRING structs
       - Used by Control module to present capabilities to user/remote end
       - ISSUE: strings are on target machine which is not great. Better to maintain strings/info on C2 client and sync when loading/unloading modules. Is added complexity of syncing worthwhile?


### Currently planned modules
 - Loader (reflective injection)
 - Comms (async)
 - Host survey (registry, directory api)
 - File transfer (file io, async possible)
 - Shell (process api)
 - Some C# module (or config util for core) (.NET, managed code)

### Core
  - Struct containing function pointers
  - Need to create api for interacting with modules
    - Easiest: pass strings for parsing to module function call
    - Pass core to modules for interoperability
      - How does comms get access to crypto?
      - File transfer access to crypto and comms?
      - Is interop necessary?
  - Design a stack: file/survey -> encrypt -> transmit (network, io, registry, etc)
     - How to design that?
     - generate -> encrypt -> transmit -> loop
     - each step configurable
       - generate: shell, survey, file io
       - encrypt: xor, bcrypt, libssl, etc.
       - transmit: network, io, registry
       - is this usable?? 

### CNO Considerations
 - Init(Local | Remote) puts all logic in both local and remote binaries. Using #define and configurations would prevent that
 - Current design of building menu on remote means strings must be in binary on remote system, should be avoided
 - Strings in resources are not currently encoded
 - Functions are exported by name, need to export by ordinal and NONAME