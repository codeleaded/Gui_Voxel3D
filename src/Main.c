//#include "F:/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
//#include "F:/home/codeleaded/System/Static/Library/Random.h"
//#include "F:/home/codeleaded/System/Static/Library/RayCast.h"
//#include "F:/home/codeleaded/System/Static/Library/Thread.h"
//#include "F:/home/codeleaded/System/Static/Library/ImageFilter.h"

#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/RayCast.h"
#include "/home/codeleaded/System/Static/Library/Thread.h"
#include "/home/codeleaded/System/Static/Library/ImageFilter.h"
#include "/home/codeleaded/System/Static/Library/VoxelWorld.h"


VoxelWorld voxelworld;

void Menu_Set(int m){
	if(voxelworld.menu==0 && m==1){
		AlxWindow_Mouse_SetInvisible(&window);
		SetMouse((Vec2){ GetWidth() / 2,GetHeight() / 2 });
	}
	if(voxelworld.menu==1 && m==0){
		AlxWindow_Mouse_SetVisible(&window);
	}
	
	voxelworld.mousebefore = GetMouse();
	voxelworld.menu = m;
}

void Setup(AlxWindow* w){
	Menu_Set(1);

	RGA_Set(Time_Nano());
	RGA_Get(6969);
	PerlinNoise_Permutations_Init();

	voxelworld = VoxelWorld_New(
		Camera_Make(
			Vec3D_New(0.0f,0.0f,0.0f),
			Vec3D_New(0.5f,1.8f,0.5f),
			Vec3D_New(0.0f,0.0f,0.0f),
			90.0f
		),
		"../data"
	);
	VoxelWorld_Start(&voxelworld);
}

void Update(AlxWindow* w){
	if(voxelworld.menu==1){
		if(GetMouse().x!=voxelworld.mousebefore.x || GetMouse().y!=voxelworld.mousebefore.y){
			Vec2 d = Vec2_Sub(GetMouse(),voxelworld.mousebefore);
			Vec2 a = Vec2_Mulf(Vec2_Div(d,(Vec2){ window.Width,window.Height }),F32_PI2);
	
			Camera* cam = VoxelWorld_GetCam(&voxelworld);
			cam->a.y += a.x;
			cam->a.x += a.y;
	
			SetMouse((Vec2){ GetWidth() / 2,GetHeight() / 2 });
			voxelworld.mousebefore = GetMouse();
		}
	}
	
	if(Stroke(ALX_KEY_ESC).PRESSED)
		Menu_Set(!voxelworld.menu);

	if(Stroke(ALX_KEY_Z).PRESSED)
		voxelworld.mode = voxelworld.mode < 3 ? voxelworld.mode + 1 : 0;

	if(Stroke(ALX_KEY_R).DOWN) Camera_AccUp(VoxelWorld_GetCam(&voxelworld),5.0f);
	if(Stroke(ALX_KEY_F).DOWN) Camera_AccDown(VoxelWorld_GetCam(&voxelworld),5.0f);
	
	if(Stroke(ALX_KEY_W).DOWN) 	Camera_AccForward(VoxelWorld_GetCam(&voxelworld),20.0f * w->ElapsedTime);
	if(Stroke(ALX_KEY_S).DOWN) 	Camera_AccForward(VoxelWorld_GetCam(&voxelworld),-20.0f * w->ElapsedTime);
	if(Stroke(ALX_KEY_A).DOWN) 	Camera_AccLeft(VoxelWorld_GetCam(&voxelworld),20.0f * w->ElapsedTime);
	if(Stroke(ALX_KEY_D).DOWN)  Camera_AccLeft(VoxelWorld_GetCam(&voxelworld),-20.0f * w->ElapsedTime);


	if(Stroke(ALX_MOUSE_L).PRESSED) VoxelWorld_Camera_Place(&voxelworld,BLOCK_VOID,4.0f);
	if(Stroke(ALX_MOUSE_R).PRESSED) VoxelWorld_Camera_PlaceN(&voxelworld,BLOCK_TORCH,4.0f);

	VoxelWorld_Update(&voxelworld);

	Clear(LIGHT_BLUE);
	VoxelWorld_Render(WINDOW_STD_ARGS,&voxelworld);

	String str = String_Format("X: %f, Y: %f, Z: %f, Size: %d",VoxelWorld_GetCam(&voxelworld)->p.x,VoxelWorld_GetCam(&voxelworld)->p.y,VoxelWorld_GetCam(&voxelworld)->p.z,voxelworld.meshselected->size);
	RenderCStrSize(str.Memory,str.size,0,0,RED);
	String_Free(&str);
}

void Delete(AlxWindow* w){
	VoxelWorld_Free(&voxelworld);
	AlxWindow_Mouse_SetVisible(&window);
}

int main(){
    if(Create("Voxel3D",2500,1200,1,1,Setup,Update,Delete))
        Start();
    return 0;
}