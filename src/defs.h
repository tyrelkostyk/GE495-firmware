// defs.h
// Contains type definitions and function stubs.

#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>

/* COMMAND (CMD) */

typedef struct _command_t {
    uint32_t id;  // TODO: What is the actual size required for this?

    // TODO: What else goes in this struct?
} command_t;

void cmdSendUpstream (void);
void cmdReceiveDownstream (void);
void cmdParse (command_t* command);

#endif  // DEFS_H
