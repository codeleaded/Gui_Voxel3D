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
	Menu_Set(0);

	RGA_Set(Time_Nano());
	RGA_Get(6969);
	PerlinNoise_Permutations_Init();

	voxelworld = VoxelWorld_New(
		Figure3D_New(
			Vec3D_New(10.0f,100.0f,10.0f),
			Vec3D_New(0.5f,1.4f,0.5f),
			Vec3D_New(0.0f,-10.0f,0.0f),
			1
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
	
			Figure3D* fig = VoxelWorld_GetFigure(&voxelworld);
			fig->a.y += a.x;
			fig->a.x += a.y;
	
			SetMouse((Vec2){ GetWidth() / 2,GetHeight() / 2 });
			voxelworld.mousebefore = GetMouse();
		}
	}
	
	if(Stroke(ALX_KEY_ESC).PRESSED)
		Menu_Set(!voxelworld.menu);

	if(Stroke(ALX_KEY_Z).PRESSED)
		voxelworld.mode = voxelworld.mode < 3 ? voxelworld.mode + 1 : 0;

	if(Stroke(ALX_KEY_C).PRESSED){
		Figure3D* fig = VoxelWorld_GetFigure(&voxelworld);
		fig->OnView = fig->OnView < 3 ? fig->OnView + 1 : 0;
	}
	if(Stroke(ALX_KEY_B).PRESSED){
		Figure3D* fig = VoxelWorld_GetFigure(&voxelworld);
		fig->OnCollision = fig->OnCollision < 1 ? fig->OnCollision + 1 : 0;
	}

	if(Stroke(ALX_KEY_R).DOWN) 		Figure3D_AccUp(VoxelWorld_GetFigure(&voxelworld),5.0f);
	else if(Stroke(ALX_KEY_F).DOWN) Figure3D_AccUp(VoxelWorld_GetFigure(&voxelworld),-5.0f);
	else							Figure3D_AccUp(VoxelWorld_GetFigure(&voxelworld),0.0f);
	
	if(Stroke(ALX_KEY_W).DOWN) 	Figure3D_AccForward(VoxelWorld_GetFigure(&voxelworld),20.0f * w->ElapsedTime);
	if(Stroke(ALX_KEY_S).DOWN) 	Figure3D_AccForward(VoxelWorld_GetFigure(&voxelworld),-20.0f * w->ElapsedTime);
	if(Stroke(ALX_KEY_A).DOWN) 	Figure3D_AccLeft(VoxelWorld_GetFigure(&voxelworld),20.0f * w->ElapsedTime);
	if(Stroke(ALX_KEY_D).DOWN)  Figure3D_AccLeft(VoxelWorld_GetFigure(&voxelworld),-20.0f * w->ElapsedTime);

	if(Stroke(ALX_MOUSE_L).PRESSED) VoxelWorld_Place(&voxelworld,BLOCK_VOID,4.0f);
	if(Stroke(ALX_MOUSE_R).PRESSED) VoxelWorld_PlaceN(&voxelworld,BLOCK_TORCH,4.0f);

	VoxelWorld_Update(&voxelworld,w->ElapsedTime);

	Clear(LIGHT_BLUE);
	VoxelWorld_Render(WINDOW_STD_ARGS,&voxelworld);

	String str = String_Format("X: %f, Y: %f, Z: %f, Size: %d",VoxelWorld_GetFigure(&voxelworld)->p.x,VoxelWorld_GetFigure(&voxelworld)->p.y,VoxelWorld_GetFigure(&voxelworld)->p.z,voxelworld.meshselected->size);
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