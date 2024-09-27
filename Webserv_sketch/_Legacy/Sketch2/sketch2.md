

Another webserver sketch based on the dependency injection pattern:

    - take an executer, may be single threadded or multi
    - take a log (single, multi, ostream...)
    - take an interpreter (the http interpreter in our case)
    - take an eventpoll (tcp coordination in our case)

    - more stuff we feel like



Upon construction, we build the parts of the server that need to be built
and built the server with those in mind