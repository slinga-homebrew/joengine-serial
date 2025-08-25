/*
** Jo Sega Saturn Engine
** Copyright (c) 2012-2020, Johannes Fetz (johannesfetz@gmail.com)
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the Johannes Fetz nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL Johannes Fetz BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <jo/jo.h>
#include "serial.h"
#include <string.h>

unsigned char recvBuffer[10];
unsigned char sendBuffer[10];

unsigned int g_Frame = 0;

void			my_draw(void)
{
	unsigned int i = 0;

	jo_printf(4, 2, "Serial Link (Taisen Cable) Demo");

	jo_printf(1, 6, "Recv:");
	for(i = 0; i < sizeof(recvBuffer); i++)
	{
		jo_printf(7 + (i * 2), 6, "%c", recvBuffer[i]);
	}	
	
	jo_printf(1, 8, "Send:");
	for(i = 0; i < sizeof(sendBuffer); i++)
	{
		jo_printf(7 + (i * 2), 8, "%c", sendBuffer[i]);
	}
}

void			my_gamepad(void)
{

}

void			my_frame_counter(void)
{
	g_Frame++;

}

void			my_serial_send(void)
{
	unsigned char data = 'H';
	int result = 0;

	if ((g_Frame & 127) != 0)
	{
		return;
	}

	result = jo_serial_send_byte(data);
	if(result != 0)
	{
		jo_printf(2, 12, "Send failed %d", result);
		return;
	}

	jo_printf(2, 13, "Sent data %d (%x)", data);

	memmove(sendBuffer + 1, sendBuffer, sizeof(sendBuffer) -1);
	sendBuffer[0] = data;
	sendBuffer[1] = data;
}

void			my_serial_recv(void)
{
	unsigned char data = 0;
	int result = 0;

	result = jo_serial_recv_byte(&data);
	if(result != 0)
	{
		jo_printf(2, 10, "Recv failed %d", result);
		return;
	}

	jo_printf(2, 11, "Recived data %d (%x)", data);

	memmove(recvBuffer + 1, recvBuffer, sizeof(recvBuffer) -1);
	recvBuffer[0] = data;
}

void			jo_main(void)
{
	jo_core_init(JO_COLOR_Black);
	//gamepad_sprite_id = jo_sprite_add_tga(JO_ROOT_DIR, "PAD.TGA", JO_COLOR_Green);
	//arrow_sprite_id = jo_sprite_add_tga(JO_ROOT_DIR, "ARW.TGA", JO_COLOR_Green);
	//x_sprite_id = jo_sprite_add_tga(JO_ROOT_DIR, "X.TGA", JO_COLOR_Green);
	//jo_core_add_callback(my_gamepad);

	jo_serial_init();

	memset(recvBuffer, ' ', sizeof(recvBuffer));
	memset(recvBuffer, ' ', sizeof(recvBuffer));

	jo_core_add_callback(my_draw);
	jo_core_add_callback(my_frame_counter);
	jo_core_add_callback(my_serial_send);
	jo_core_add_callback(my_serial_recv);
	jo_core_run();
}

/*
** END OF FILE
*/
