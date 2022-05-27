#version 450
#extension GL_ARB_separate_shader_objects : enable
#include "hg_sdf.frag"

layout(location = 0) out vec4 OutColor;

layout(set = 0, binding = 0) uniform UniformBufferObject
{
	vec2 Resolution;
	uint Ticks;
} UBO;

const float FOV = 1.0;
const int MAX_STEPS = 256;
const float MAX_DIST = 500;
const float EPSILON = 0.001;

vec2 fOpUnionID(vec2 Res1, vec2 Res2) 
{
    return (Res1.x < Res2.x) ? Res1 : Res2;
}

vec2 fOpDifferenceID(vec2 Res1, vec2 Res2) 
{
    return (Res1.x > -Res2.x) ? Res1 : vec2(-Res2.x, Res2.y);
}

vec2 fOpDifferenceColumnsID(vec2 Res1, vec2 Res2, float R, float N) 
{
    float dist = fOpDifferenceColumns(Res1.x, Res2.x, R, N);
    return (Res1.x > -Res2.x) ? vec2(dist, Res1.y) : vec2(dist, Res2.y);
}

vec2 fOpUnionStairsID(vec2 Res1, vec2 Res2, float R, float N) 
{
    float dist = fOpUnionStairs(Res1.x, Res2.x, R, N);
    return (Res1.x < Res2.x) ? vec2(dist, Res1.y) : vec2(dist, Res2.y);
}

vec2 fOpUnionChamferID(vec2 Res1, vec2 Res2, float R) 
{
    float dist = fOpUnionChamfer(Res1.x, Res2.x, R);
    return (Res1.x < Res2.x) ? vec2(dist, Res1.y) : vec2(dist, Res2.y);
}

vec2 Map(vec3 P)
{
	float PlaneDist = fPlane(P, vec3(0, 1, 0), 14.0);
	float PlaneID = 2.0;
	vec2 Plane = vec2(PlaneDist, PlaneID);

	float SphereDist = fSphere(P, 1.0);
	float SphereID = 1.0;
	vec2 Sphere = vec2(SphereDist, SphereID);

	pMirrorOctant(P.xz, vec2(50, 50));
	P.x = -abs(P.x) + 20;
	pMod1(P.z, 15);

	vec3 PR = P;
	PR.y -= 15.5;
	pR(PR.xy, 0.6);
	PR.x -= 18.0;
	float RoofDist = fBox2(PR.xy, vec2(20, 0.3));
	float RoofID = 1.0;
	vec2 Roof = vec2(RoofDist, RoofID);

	float BoxDist = fBox(P, vec3(3, 9, 4));
	float BoxID = 1.0;
	vec2 Box = vec2(BoxDist, BoxID);

	vec3 PC = P;
	PC.y -= 9.0;
	float CylinderDist = fCylinder(PC.yxz, 4, 3);
	float CylinderID = 1.0;
	vec2 Cylinder = vec2(CylinderDist, CylinderID);

	float WallDist = fBox2(P.xy, vec2(1, 15));
	float WallID = 1.0;
	vec2 Wall = vec2(WallDist, WallID);

	vec2 Result;
	Result = fOpUnionID(Box, Cylinder);
	Result = fOpDifferenceColumnsID(Wall, Result, 0.6, 3.0);
	Result = fOpUnionChamferID(Result, Roof, 0.9);
	Result = fOpUnionStairsID(Result, Plane, 4.0, 5.0);
	return Result;
}

vec2 RayMarch(vec3 RayOrigin, vec3 RayDirection)
{
	vec2 Hit = vec2(0.0);
	vec2 Object = vec2(0.0);

	for (int i = 0; i < MAX_STEPS; i++)
	{
		vec3 P = RayOrigin + Object.x * RayDirection;
		Hit = Map(P);
		Object.x += Hit.x;
		Object.y = Hit.y;
		if (abs(Hit.x) < EPSILON || Object.x > MAX_DIST)
			break;
	}

	return Object;
}

vec3 GetNormal(vec3 P)
{
	vec2 E = vec2(EPSILON, 0.0);
	vec3 N = vec3(Map(P).x) - vec3(Map(P - E.xyy).x, Map(P - E.yxy).x, Map(P - E.yyx).x);
	return normalize(N);
}

vec3 GetLight(vec3 P, vec3 RayDirection, vec3 Color)
{
	vec3 LightPos = vec3(20.0, 40.0, -30.0);
	vec3 L = normalize(LightPos - P);
	vec3 N = GetNormal(P);
	vec3 V = -RayDirection;
	vec3 R = reflect(-L, N);

	vec3 SpecularColor = vec3(0.5);
	vec3 Specular = SpecularColor * pow(clamp(dot(R, V), 0.0, 1.0), 10.0);
	vec3 Diffuse = Color * clamp(dot(L, N), 0.0, 1.0);
	vec3 Ambient = Color * 0.05;

	//Shadows
	float D = RayMarch(P + N * 0.02, normalize(LightPos)).x;
	if (D < length(LightPos - P))
		return Ambient;
	return Diffuse + Ambient + Specular;
}

vec3 GetMaterial(vec3 P, float ID)
{
	vec3 Material;
	switch (int(ID))
	{
		case 1:
			Material = vec3(0.2, 0.4, 0.8);
			break;

		case 2:
			Material = vec3(0.2 + 0.4 * mod(floor(P.x) + floor(P.z), 2.0));
			break;
	}

	return Material;
}

mat3 GetCamera(vec3 RayOrigin, vec3 LookAt)
{
	vec3 CameraFront = normalize(vec3(LookAt - RayOrigin));
	vec3 CameraRight = normalize(cross(vec3(0.0, 1.0, 0.0), CameraFront));
	vec3 CameraUp = -cross(CameraFront, CameraRight);
	return mat3(CameraRight, CameraUp, CameraFront);
}

void Render(inout vec3 Color, in vec2 UV)
{
	vec3 RayOrigin = vec3(16.0 * sin(UBO.Ticks * 0.001), 9.0, 16.0 * cos(UBO.Ticks * 0.001));
	vec3 LookAt = vec3(0.0, 0.0, 0.0);
	vec3 RayDirection = GetCamera(RayOrigin, LookAt) * normalize(vec3(UV, FOV));

	vec3 Background = vec3(0.5, 0.8, 0.9);

	vec2 Object = RayMarch(RayOrigin, RayDirection);
	if (Object.x < MAX_DIST)
	{
		vec3 P = RayOrigin + Object.x * RayDirection;
		vec3 Material = GetMaterial(P, Object.y);
		Color += GetLight(P, RayDirection, Material);
		//Fog
		Color = mix(Color, Background, 1.0 - exp(-0.00002 * Object.x * Object.x));
	}
	else
	{
		Color += Background - max(0.95 * RayDirection.y, 0.0);
	}
	
}

void main()
{
	vec2 UV = (2.0 * gl_FragCoord.xy - UBO.Resolution.xy) / UBO.Resolution.y;

	vec3 Color = vec3(0.0);
	Render(Color, UV);

	Color = pow(Color, vec3(0.4545));
	OutColor = vec4(Color, 1.0);
}