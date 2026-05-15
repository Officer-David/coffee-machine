#include "gd32f30x.h"
#include "os_lyh_core.h"
#include "configboard.h"


void Relay2_ON(void)
{
    SetRelay2_On();
}

void Relay2_OFF(void)
{
    ClrRelay2_Off();
}

static void Relay2_GPIO_INIT(void)
{
    RELAY2_CONFIG();
    Relay2_OFF();
}

void gd_realy_init(void)
{
    Relay2_GPIO_INIT();
}
