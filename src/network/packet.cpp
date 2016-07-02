#include "Sharun.hpp"

packet::packet()
{
	id = 0;
	size = 4;
	if (Sharun->Settings.Packets_Fill_Size)
		size_max = PACKET_MAX_SIZE;
	else
		size_max = 4;
	pos = 4;
	pos_switch = 0;
	raw = (byte*)malloc(sizeof(byte) * size_max);
	memset(raw, 0, size);
}

packet::~packet()
{
	delete raw;
}

int packet::seek(int offset, int origin)
{
	ushort pos_t = pos;
	switch (origin)
	{
		case SEEK_END:
			pos_t = size + offset;
			break;
		case SEEK_SET:
			pos_t = offset;
			break;
		default:
			pos_t += offset;
	}
	if (pos_t < 4 || pos_t > size)
		return 1;
	pos = pos_t;
	return 0;
}

bool packet::eof()
{
	if (pos >= size)
		return true;
	return false;
}

bool packet::swap()
{
	if (pos_switch < 4)
		return false;
	pos = pos_switch;
	return true;
}

bool packet::swap(ushort pos_n)
{
	if (pos_n > size)
		return false;
	pos_switch = pos;
	pos = pos_n;
	return true;
}

packet* packet::clone()
{
	packet* packet_t = new packet();
	packet_t->id = id;
	packet_t->size = size;
	packet_t->pos = pos_switch;
	packet_t->swap(pos);
	byte *raw_n = (byte*)realloc((void*)packet_t->raw, sizeof(byte) * size_max);
	if (raw_n)
		packet_t->raw = raw_n;
	else
		resize(-1);
	memcpy(packet_t->raw, raw, size);
	return packet_t;
}

void packet::resize(int size_f)
{
	byte *raw_n = (byte*)malloc(sizeof(byte) * size_max);
	if (size_f >= 0)
		memcpy(raw_n, raw, size_f ? size_f : size);
	if (raw)
		free(raw);
	raw = raw_n;
}

ushort packet::full_size()
{
	size_max = PACKET_MAX_SIZE;
	resize(0);
	return 0;
}

void packet::get_header()
{
	memcpy(&size, &raw[0], 2);
	memcpy(&id, &raw[2], 2);
	size_max = size;
	resize(size);
}

void packet::set_header()
{
	memcpy(&raw[0], &size, 2);
	memcpy(&raw[2], &id, 2);
}

void packet::set_header(ushort id_n)
{
	id = id_n;
	set_header();
}

void packet::write_len(ushort len)
{
	if (size_max < size + len) {
		size_max = size + len;
		byte *raw_n = (byte*)realloc((void*)raw, sizeof(byte) * size_max);
		if (!raw_n)
			resize(0);
		else
			raw = raw_n;
	}
	size += len;
}

ushort packet::write(void* src, ushort len)
{
	write_len(len);
	memcpy(&raw[pos], src, len);
	pos += len;
	return len;
}

ushort packet::write(char* src)
{
	size_t len = 0;
	void *src_t = str_str(src, &len);
	len = write(src_t, len);
	free(src_t);
	pos += len;
	return len;
}

ushort packet::write(char16_t* src)
{
	size_t len = strlen16(src);
	len = write(src, len+1);
	pos += len;
	return len;
}

ushort packet::read(void* dst, ushort len)
{
	if (pos + len > size)
		return 0;
	memcpy(dst, &raw[pos], len);
	pos += len;
	return len;
}

byte* packet::read(ushort len)
{
	byte *dst_t = new byte[len];
	read((void*)dst_t, len);
	return dst_t;
}

ushort packet::read(char* dst)
{
	ushort len = 0;
	len = strcpy16(dst, (char16_t*)&raw[pos]);
	pos += len;
	return len;
}

char* packet::read_Str()
{
	ushort len = 0;
	len = str_strlen((char16_t*)&raw[pos]);
	char *dst_t = new char[len + 1];
	len = strcpy16(dst_t, (char16_t*)&raw[pos]);
	pos += len;
	return dst_t;
}

char16_t* packet::read_S()
{
	ushort len = strlen16((char16_t*)&raw[pos]);
	char16_t *dst_t = new char16_t[len + 1];
	len = strcpy16(dst_t, (char16_t*)&raw[pos]);
	pos += len;
	return dst_t;
}

uchar packet::read_1()
{
	ushort len = 0;
	uchar dst_t = 0;
	len = read((void*)&dst_t, sizeof(dst_t));
	pos += len;
	return dst_t;
}

ushort packet::read_2()
{
	ushort len = 0;
	ushort dst_t = 0;
	len = read((void*)&dst_t, sizeof(dst_t));
	pos += len;
	return dst_t;
}

uint packet::read_4()
{
	ushort len = 0;
	uint dst_t = 0;
	len = read((void*)&dst_t, sizeof(dst_t));
	pos += len;
	return dst_t;
}

ulong packet::read_8()
{
	ushort len = 0;
	ulong dst_t = 0;
	len = read((void*)&dst_t, sizeof(dst_t));
	pos += len;
	return dst_t;
}

float packet::read_F()
{
	ushort len = 0;
	float dst_t = 0;
	len = read((void*)&dst_t, sizeof(dst_t));
	pos += len;
	return dst_t;
}


/**
 * Easy-Life part...
 **/
ushort packet::write(uchar src)
{
	return write(&src, sizeof(src));
}
ushort packet::write(ushort src)
{
	return write(&src, sizeof(src));
}
ushort packet::write(short src)
{
	return write(&src, sizeof(src));
}
ushort packet::write(uint src)
{
	return write(&src, sizeof(src));
}
ushort packet::write(int src)
{
	return write(&src, sizeof(src));
}
ushort packet::write(ulong src)
{
	return write(&src, sizeof(src));
}
ushort packet::write(long src)
{
	return write(&src, sizeof(src));
}
ushort packet::write(float src)
{
	return write(&src, sizeof(src));
}

ushort packet::read(uchar* dst)
{
	return read((void*)dst, 1);
}
ushort packet::read(ushort* dst)
{
	return read((void*)dst, 2);
}
ushort packet::read(short* dst)
{
	return read((void*)dst, 2);
}
ushort packet::read(uint* dst)
{
	return read((void*)dst, 4);
}
ushort packet::read(int* dst)
{
	return read((void*)dst, 4);
}
ushort packet::read(ulong* dst)
{
	return read((void*)dst, 8);
}
ushort packet::read(long* dst)
{
	return read((void*)dst, 8);
}
ushort packet::read(float* dst)
{
	return read((void*)dst, 4);
}
