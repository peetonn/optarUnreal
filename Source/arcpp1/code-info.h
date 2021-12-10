#pragma once

#include "git-describe.h"

#define STRINGIZE_VERSION(v) STRINGIZE_TOKEN(v)
#define STRINGIZE_TOKEN(t) #t
#define CODE_VERSION STRINGIZE_VERSION(GIT_DESCRIBE)
#define BRANCH_NAME STRINGIZE_VERSION(GIT_BRANCH)
#define CODE_COMMIT STRINGIZE_VERSION(GIT_COMMIT)