#include "TaskLambda.hpp"

namespace Tasks {

TaskLambda::~TaskLambda (void) noexcept
{
}

void    TaskLambda::Run (void)
{
    iFn();
}

}//namespace Tasks
