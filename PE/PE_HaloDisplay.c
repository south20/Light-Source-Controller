#include "PE_HaloDisplay.h"



int PE_HaloDisplay_Green(IO_GPIO_Pin_State bitstatus)
{

	int rtn = 0;
	
	rtn = IO_GPIO_Write_Pin(IO_GPIO_GR,bitstatus);
	rtn = IO_GPIO_Write_Pin(IO_GPIO_RD,IO_GPIO_PIN_SET);
	rtn = IO_GPIO_Write_Pin(IO_GPIO_BL,IO_GPIO_PIN_SET);

	return rtn;
 

}

int PE_HaloDisplay_Red(IO_GPIO_Pin_State bitstatus)
{

	int rtn = 0;
	
	rtn = IO_GPIO_Write_Pin(IO_GPIO_RD,bitstatus);
	rtn = IO_GPIO_Write_Pin(IO_GPIO_GR,IO_GPIO_PIN_SET);
	rtn = IO_GPIO_Write_Pin(IO_GPIO_BL,IO_GPIO_PIN_SET);

	return rtn;
 

}
int PE_HaloDisplay_Blue(IO_GPIO_Pin_State bitstatus)
{

	int rtn = 0;
	
	rtn = IO_GPIO_Write_Pin(IO_GPIO_BL,bitstatus);
	rtn = IO_GPIO_Write_Pin(IO_GPIO_GR,IO_GPIO_PIN_SET);
	rtn = IO_GPIO_Write_Pin(IO_GPIO_RD,IO_GPIO_PIN_SET);

	return rtn;
 

}
int PE_HaloDisplay_Purple(IO_GPIO_Pin_State bitstatus)
{

	int rtn = 0;
	rtn = IO_GPIO_Write_Pin(IO_GPIO_RD,bitstatus);
	rtn = IO_GPIO_Write_Pin(IO_GPIO_BL,bitstatus);
	rtn = IO_GPIO_Write_Pin(IO_GPIO_GR,IO_GPIO_PIN_SET);

	return rtn;
 

}
int PE_HaloDisplay_Yellow(IO_GPIO_Pin_State bitstatus)
{

	int rtn = 0;
	
	rtn = IO_GPIO_Write_Pin(IO_GPIO_GR,bitstatus);
	rtn = IO_GPIO_Write_Pin(IO_GPIO_RD,bitstatus);
	rtn = IO_GPIO_Write_Pin(IO_GPIO_BL,IO_GPIO_PIN_SET);

	return rtn;
 

}

int PE_HaloDisplay_Close()
{

	int rtn = 0;
	
	rtn = IO_GPIO_Write_Pin(IO_GPIO_GR,IO_GPIO_PIN_SET);
	rtn = IO_GPIO_Write_Pin(IO_GPIO_RD,IO_GPIO_PIN_SET);
	rtn = IO_GPIO_Write_Pin(IO_GPIO_BL,IO_GPIO_PIN_SET);

	return rtn;
 

}

int PE_HaloDisplay_Colour(HaloDisplay_Colour colour)
{
	int rtn = 0;
	IO_GPIO_Pin_State pe_open_halodisplay = IO_GPIO_PIN_RESET;

	switch (colour)
    {
    case Green:
        PE_HaloDisplay_Green(pe_open_halodisplay);
        break;
    case Red:
        PE_HaloDisplay_Red(pe_open_halodisplay);
        break;
    case Blue:
        PE_HaloDisplay_Blue(pe_open_halodisplay);
        break;
    case Purple:
        PE_HaloDisplay_Purple(pe_open_halodisplay);
        break;
	case Yellow:
        PE_HaloDisplay_Yellow(pe_open_halodisplay);
        break;
	case Close:
        PE_HaloDisplay_Close();
        break;
		
	default:
        // 处理未知的 IO_GPIOx
        return IO_PARAMETER_ERROR; 
	}

	return rtn;

}















