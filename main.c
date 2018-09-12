#include <misc.h>
#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_rcc.h>


volatile uint8_t led =0;

//Kesme fonksiyonu. Timer3 sifirlandigi zaman
//program akisi bu fonksiyona gelecek
void TIM3_IRQHandler(void)
{
    if(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update) != RESET){
		TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
		//ledin durum degistirmesi
		led ^=1;
    }
}

int main(void)
{
    GPIO_InitTypeDef        GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM3_InitStructure;
    NVIC_InitTypeDef        NVIC_InitStructure;

    //Kullanacagimiz portun ve timerin aktif edilmesi
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    //Ledin bagli oldugu portun yapilandirmasi
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

    //Tim3 timerin yapilandirilmasi
    TIM3_InitStructure.TIM_Prescaler         = 7200;
    TIM3_InitStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM3_InitStructure.TIM_Period            = 1000;
    TIM3_InitStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3, &TIM3_InitStructure);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    //Tim3 kesmesinin ayarlanmasi
    NVIC_InitStructure.NVIC_IRQChannel          = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd       = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM3, ENABLE);


    while(1)
    {
        if(led)
            GPIO_SetBits(GPIOC, GPIO_Pin_13);
        else
            GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    }
    return (0);
}
