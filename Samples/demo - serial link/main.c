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

unsigned char recvBuffer[16];
unsigned char sendBuffer[16];

unsigned int frame = 0;
unsigned int serial_sprite_id = 0;

void prepend_array(unsigned char* array, unsigned int array_size, unsigned char val);
char convert_button_to_char(void);

void			my_draw(void)
{
	unsigned int i = 0;

	jo_printf(4, 2, "Serial Link (Taisen Cable) Demo");
	jo_sprite_draw3D(serial_sprite_id, 0,-40, 500);

	jo_printf(1, 18, "Send:");
	for(i = 0; i < sizeof(sendBuffer); i++)
	{
		jo_printf(7 + (i * 2), 18, "%c", sendBuffer[i]);
	}

	jo_printf(1, 20, "Recv:");
	for(i = 0; i < sizeof(recvBuffer); i++)
	{
		jo_printf(7 + (i * 2), 20, "%c", recvBuffer[i]);
	}

	jo_printf(1, 22, "Press buttons to send data");
	jo_printf(1, 23, "H is a periodic heartbeat. 1800 B 8N1");
}

void			my_gamepad(void)
{
	unsigned char data = 0;
	int result = 0;

	if(!jo_is_input_available(0))
	{
		return;
	}

	data = convert_button_to_char();
	if(!data)
	{
		return;
	}

	result = jo_serial_send_byte(data);
	if(result != 0)
	{
		//jo_printf(2, 12, "Send failed %d", result);
		return;
	}

	prepend_array(sendBuffer, sizeof(sendBuffer), data);
}

void			my_frame_counter(void)
{
	frame++;
}

void			my_serial_send(void)
{
	unsigned char data = 'H';
	int result = 0;

	if ((frame & 255) != 0)
	{
		return;
	}

	result = jo_serial_send_byte(data);
	if(result != 0)
	{
		//jo_printf(2, 12, "Send failed %d", result);
		return;
	}

	// jo_printf(2, 13, "Sent data %d (%x)", data);

	prepend_array(sendBuffer, sizeof(sendBuffer), data);

	jo_printf(1, 25, "              ");
	jo_printf(1, 26, "              ");
	jo_printf(1, 27, "              ");
	jo_printf(1, 28, "              ");
}

void			my_serial_recv(void)
{
	unsigned char data = 0;
	int result = 0;

	result = jo_serial_recv_byte(&data);
	if(result != 0)
	{
		//jo_printf(2, 10, "Recv failed %d", result);
		return;
	}

	//jo_printf(2, 11, "Recived data %d (%x)", data);

	prepend_array(recvBuffer, sizeof(recvBuffer), data);
}

void			jo_main(void)
{
	memset(recvBuffer, ' ', sizeof(recvBuffer));
	memset(sendBuffer, ' ', sizeof(sendBuffer));

	jo_core_init(JO_COLOR_Black);
	jo_serial_init();

	serial_sprite_id = jo_sprite_add_tga(JO_ROOT_DIR, "SERIAL.TGA", JO_COLOR_Green);

	jo_core_add_callback(my_draw);
	jo_core_add_callback(my_frame_counter);
	jo_core_add_callback(my_serial_send);
	jo_core_add_callback(my_serial_recv);
	jo_core_add_callback(my_gamepad);

	jo_core_run();
}

void prepend_array(unsigned char* array, unsigned int array_size, unsigned char val)
{
	memmove(array + 1, array, array_size -1);
	array[0] = val;
}

char convert_button_to_char(void)
{
	if(jo_is_input_key_down(0, JO_KEY_RIGHT))
	{
		return 'r';
	}
	else if(jo_is_input_key_down(0, JO_KEY_LEFT))
	{
		return 'l';
	}
	else if(jo_is_input_key_down(0, JO_KEY_DOWN))
	{
		return 'd';
	}
	else if(jo_is_input_key_down(0, JO_KEY_UP))
	{
		return 'u';
	}
	else if(jo_is_input_key_down(0, JO_KEY_START))
	{
		return 'S';
	}
	else if(jo_is_input_key_down(0, JO_KEY_A))
	{
		return 'A';
	}
	else if(jo_is_input_key_down(0, JO_KEY_B))
	{
		return 'B';
	}
	else if(jo_is_input_key_down(0, JO_KEY_C))
	{
		return 'C';
	}
	else if(jo_is_input_key_down(0, JO_KEY_X))
	{
		return 'X';
	}
	else if(jo_is_input_key_down(0, JO_KEY_Y))
	{
		return 'Y';
	}
	else if(jo_is_input_key_down(0, JO_KEY_Z))
	{
		return 'Z';
	}
	else if(jo_is_input_key_down(0, JO_KEY_LEFT))
	{
		return 'L';
	}
	else if(jo_is_input_key_down(0, JO_KEY_RIGHT))
	{
		return 'R';
	}

	return 0;
}

/*
** END OF FILE
*/
