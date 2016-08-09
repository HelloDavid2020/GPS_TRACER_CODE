#include "user_interrupt.h"
void EnterCriticalSection( void )
{
  __disable_irq();
}

void ExitCriticalSection( void )
{
  __enable_irq();
}
