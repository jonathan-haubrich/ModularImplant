## Modular Implant

### Modules
 - Registered via a callback
 - Modules know their type
 - Shared header defining types
 - Types:
   - Loader
   - Comms
   - Crypto? (MUST have crypto, can be plaintext if user wants)
   - Misc?
 - How to categorize modules -- necessary?

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
