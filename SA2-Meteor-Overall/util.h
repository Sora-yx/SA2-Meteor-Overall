#pragma once

#pragma pack(push, 1)
struct cartStruct
{
	NJS_POINT3 Velocity;
	Float idk;
	Float idk2;
	NJS_VECTOR Position;
	Rotation Forward;
	Float DistanceWall;
	Angle DistanceIdk;
	Float idk3;
	Float SpeedY;
	Float SpeedX;
	Float Accel;
	Float idk5;
};
#pragma pack(pop)


FastcallFunctionPointer(int, sub_621380, (int a1, EntityData1* a2), 0x621380);
DataArray(NJS_VECTOR, ChaoSpawnpoints, 0x01366260, 16 * 3);