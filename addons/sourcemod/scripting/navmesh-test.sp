#include <sourcemod>
#include <sdktools>
#include <profiler>
#include <navmesh>

#define PLUGIN_VERSION "1.0.1"

int g_iPathLaserModelIndex = -1;

public Plugin myinfo = 
{
    name = "SP-Readable Navigation Mesh Test",
    author	= "KitRifty",
    description	= "Testing plugin of the SP-Readable Navigation Mesh plugin.",
    version = PLUGIN_VERSION,
    url = ""
}

public void OnPluginStart()
{
	RegConsoleCmd("sm_navmesh_collectsurroundingareas", Command_NavMeshCollectSurroundingAreas);
	RegConsoleCmd("sm_navmesh_buildpath", Command_NavMeshBuildPath);
	RegConsoleCmd("sm_navmesh_worldtogridx", Command_NavMeshWorldToGridX);
	RegConsoleCmd("sm_navmesh_worldtogridy", Command_NavMeshWorldToGridY);
	RegConsoleCmd("sm_navmesh_getareasongrid", Command_GetNavAreasOnGrid);
	RegConsoleCmd("sm_navmesh_getarea", Command_GetArea);
	RegConsoleCmd("sm_navmesh_getnearestarea", Command_GetNearestArea);
	RegConsoleCmd("sm_navmesh_getadjacentareas", Command_GetAdjacentNavAreas);
}

public void OnMapStart()
{
	g_iPathLaserModelIndex = PrecacheModel("materials/sprites/laserbeam.vmt");
}

public Action Command_GetArea(int client, int args)
{
	if (!NavMesh_Exists()) return Plugin_Handled;

	float flEyePos[3]; float flEyeDir[3]; float flEndPos[3];
	GetClientEyePosition(client, flEyePos);
	GetClientEyeAngles(client, flEyeDir);
	GetAngleVectors(flEyeDir, flEyeDir, NULL_VECTOR, NULL_VECTOR);
	NormalizeVector(flEyeDir, flEyeDir);
	ScaleVector(flEyeDir, 1000.0);
	AddVectors(flEyePos, flEyeDir, flEndPos);
	
	Handle hTrace = TR_TraceRayFilterEx(flEyePos,
		flEndPos,
		MASK_PLAYERSOLID_BRUSHONLY,
		RayType_EndPoint,
		TraceRayDontHitEntity,
		client);
	
	TR_GetEndPosition(flEndPos, hTrace);
	delete hTrace;
	
	CNavArea area = NavMesh_GetArea(flEndPos);
	PrintToChat(client, "Nearest area ID: %d", area.ID);
	
	return Plugin_Handled;
}

public Action Command_GetNearestArea(int client, int args)
{
	if (!NavMesh_Exists()) return Plugin_Handled;

	float flEyePos[3]; float flEyeDir[3]; float flEndPos[3];
	GetClientEyePosition(client, flEyePos);
	GetClientEyeAngles(client, flEyeDir);
	GetAngleVectors(flEyeDir, flEyeDir, NULL_VECTOR, NULL_VECTOR);
	NormalizeVector(flEyeDir, flEyeDir);
	ScaleVector(flEyeDir, 1000.0);
	AddVectors(flEyePos, flEyeDir, flEndPos);
	
	Handle hTrace = TR_TraceRayFilterEx(flEyePos,
		flEndPos,
		MASK_PLAYERSOLID_BRUSHONLY,
		RayType_EndPoint,
		TraceRayDontHitEntity,
		client);
	
	TR_GetEndPosition(flEndPos, hTrace);
	CloseHandle(hTrace);
	
	int x = NavMesh_WorldToGridX(flEndPos[0]);
	int y = NavMesh_WorldToGridY(flEndPos[1]);
	int iGridIndex = x + y * NavMesh_GetGridSizeX();
	
	CNavArea area = NavMesh_GetNearestArea(flEndPos);
	if (area != INVALID_NAV_AREA)
	{
		PrintToChat(client, "Nearest area ID found from spiral out of %d: %d", iGridIndex, area.ID);
	}
	else
	{
		PrintToChat(client, "Could not find nearest area in spiral out of %d!", iGridIndex);
	}
	
	return Plugin_Handled;
}

public Action Command_GetAdjacentNavAreas(int client, int args)
{
	if (!NavMesh_Exists()) return Plugin_Handled;
	
	if (args < 2)
	{
		ReplyToCommand(client, "Usage: sm_navmesh_getadjacentareas <area ID>");
		return Plugin_Handled;
	}
	
	ArrayList hAreas = NavMesh_GetAreas();
	if (hAreas == null) return Plugin_Handled;
	
	char sAreaID[64];
	GetCmdArg(1, sAreaID, sizeof(sAreaID));
	
	int iAreaID = StringToInt(sAreaID);
	
	CNavArea startArea = view_as<CNavArea>hAreas.FindValue(iAreaID);
	if (startArea == INVALID_NAV_AREA) return Plugin_Handled;
	
	char sNavDirection[64];
	GetCmdArg(2, sNavDirection, sizeof(sNavDirection));
	
	int iNavDirection = StringToInt(sNavDirection);
	if (iNavDirection >= NAV_DIR_COUNT)
	{
		ReplyToCommand(client, "Invalid direction! Direction cannot reach %d!", NAV_DIR_COUNT);
		return Plugin_Handled;
	}
	
	ArrayStack hAdjacentAreas = new ArrayStack();
	startArea.GetAdjacentAreas(iNavDirection, hAdjacentAreas);
	
	if (!hAdjacentAreas.Empty)
	{
		while (!hAdjacentAreas.Empty)
		{
			CNavArea area = INVALID_NAV_AREA;
			hAdjacentAreas.Pop(view_as<int>area);
			PrintToChat(client, "Found adjacent area (ID: %d) for area ID %d", area.ID, startArea.ID);
		}
		
		delete hAdjacentAreas;
	}
	else
	{
		PrintToChat(client, "Found no adjacent areas for area ID %d", startArea.ID);
	}
	
	delete hAdjacentAreas;
	
	return Plugin_Handled;
}

public Action Command_NavMeshCollectSurroundingAreas(int client, int args)
{
	if (args < 2)
	{
		ReplyToCommand(client, "Usage: sm_navmesh_collectsurroundingareas <area ID> <max dist>");
		return Plugin_Handled;
	}
	
	if (!NavMesh_Exists()) return Plugin_Handled;
	
	ArrayList hAreas = NavMesh_GetAreas();
	if (hAreas == null) return Plugin_Handled;
	
	char sAreaID[64];
	GetCmdArg(1, sAreaID, sizeof(sAreaID));
	
	CNavArea area = view_as<CNavArea>hAreas.FindValue(StringToInt(sAreaID));
	
	if (area == INVALID_NAV_AREA) return Plugin_Handled;
	
	char sMaxDist[64];
	GetCmdArg(2, sMaxDist, sizeof(sMaxDist));
	
	float flMaxDist = StringToFloat(sMaxDist);
	
	Handle hProfiler = CreateProfiler();
	StartProfiling(hProfiler);
	
	ArrayStack hNearAreas = new ArrayStack();
	NavMesh_CollectSurroundingAreas(hNearAreas, area, flMaxDist);
	
	StopProfiling(hProfiler);
	float flProfileTime = GetProfilerTime(hProfiler);
	delete hProfiler;
	
	if (!hNearAreas.Empty)
	{
		int iAreaCount = 0;
		while (!hNearAreas.Empty)
		{
			int iSomething;
			hNearAreas.Pop(iSomething);
			iAreaCount++;
		}
		
		if (client > 0) 
		{
			PrintToChat(client, "Collected %d areas in %f seconds.", iAreaCount, flProfileTime);
		}
		else
		{
			PrintToServer("Collected %d areas in %f seconds.", iAreaCount, flProfileTime);
		}
	}
	
	delete hNearAreas;
	
	return Plugin_Handled;
}

public Action Command_NavMeshWorldToGridX(int client, int args)
{
	if (args < 1) return Plugin_Handled;
	
	char arg1[32];
	GetCmdArg(1, arg1, sizeof(arg1));
	
	float flpl = StringToFloat(arg1);
	
	ReplyToCommand(client, "Grid x: %d", NavMesh_WorldToGridX(flpl));
	
	return Plugin_Handled;
}

public Action Command_NavMeshWorldToGridY(int client, int args)
{
	if (args < 1) return Plugin_Handled;
	
	char arg1[32];
	GetCmdArg(1, arg1, sizeof(arg1));
	
	float flpl = StringToFloat(arg1);
	
	ReplyToCommand(client, "Grid y: %d", NavMesh_WorldToGridY(flpl));
	
	return Plugin_Handled;
}

public Action Command_GetNavAreasOnGrid(int client, int args)
{
	if (args < 2) return Plugin_Handled;
	
	char arg1[32];
	GetCmdArg(1, arg1, sizeof(arg1));
	
	int x = StringToInt(arg1);
	
	decl String:arg2[32];
	GetCmdArg(2, arg2, sizeof(arg2));
	
	int y = StringToInt(arg2);
	
	ArrayStack hAreas = new ArrayStack();
	NavMesh_GetAreasOnGrid(hAreas, x, y);
	
	if (!hAreas.Empty)
	{
		while (!hAreas.Empty)
		{
			CNavArea area = INVALID_NAV_AREA;
			hAreas.Pop(view_as<int>area);
			ReplyToCommand(client, "%d", area.Index);
		}
	}
	
	delete hAreas;
	
	return Plugin_Handled;
}

public Action Command_NavMeshBuildPath(int client, int args)
{
	if (args < 2)
	{
		ReplyToCommand(client, "Usage: sm_navmesh_buildpath <start area ID> <goal area ID>");
		return Plugin_Handled;
	}
	
	if (!NavMesh_Exists()) return Plugin_Handled;
	
	ArrayList hAreas = NavMesh_GetAreas();
	if (hAreas == null) return Plugin_Handled;
	
	char sStartAreaID[64]; char sGoalAreaID[64];
	GetCmdArg(1, sStartAreaID, sizeof(sStartAreaID));
	GetCmdArg(2, sGoalAreaID, sizeof(sGoalAreaID));
	
	CNavArea startArea = view_as<CNavArea>hAreas.FindValue(StringToInt(sStartAreaID));
	CNavArea goalArea = view_as<CNavArea>hAreas.FindValue(StringToInt(sGoalAreaID));
	
	if (startArea == INVALID_NAV_AREA || goalArea == INVALID_NAV_AREA) return Plugin_Handled;
	
	float flGoalPos[3];
	goalArea.GetCenter(flGoalPos);
	
	new iColor[4] = { 0, 255, 0, 255 };
	
	new Float:flMaxPathLength = 0.0;
	if (args > 2)
	{
		decl String:sMaxPathLength[64];
		GetCmdArg(3, sMaxPathLength, sizeof(sMaxPathLength));
		flMaxPathLength = StringToFloat(sMaxPathLength);
		
		if (flMaxPathLength < 0.0) return Plugin_Handled;
	}
	
	CNavArea closestArea = CNavArea(0);
	
	Handle hProfiler = CreateProfiler();
	StartProfiling(hProfiler);
	
	bool bBuiltPath = NavMesh_BuildPath(startArea, 
		goalArea,
		flGoalPos,
		NavMeshShortestPathCost,
		_,
		closestArea,
		flMaxPathLength);
	
	StopProfiling(hProfiler);
	
	float flProfileTime = GetProfilerTime(hProfiler);
	
	delete hProfiler;
	
	if (client > 0) 
	{
		PrintToChat(client, "Path built!\nBuild path time: %f\nReached goal: %d", flProfileTime, bBuiltPath);
		
		CNavArea tempArea = closestArea;
		CNavArea parentArea = tempArea.Parent;
		int dir;
		float halfWidth;
		
		float centerPortal[3]; float closestPoint[3];
		
		ArrayList hPositions = CreateArray(3);
		hPositions.PushArray(flGoalPos, 3);
		
		while (parentArea != INVALID_NAV_AREA)
		{
			float tempAreaCenter[3]; float parentAreaCenter[3];
			tempArea.GetCenter(tempAreaCenter);
			parentArea.GetCenter(parentAreaCenter);
			
			dir = tempArea.ComputeDirection(parentAreaCenter);
			tempArea.ComputePortal(parentArea, dir, centerPortal, halfWidth);
			tempArea.ComputeClosestPointInPortal(parentArea, dir, centerPortal, closestPoint);
			
			closestPoint[2] = tempArea.GetZ(closestPoint);
			
			hPositions.PushArray(closestPoint, 3);
			
			tempArea = parentArea;
			parentArea = tempArea.Parent;
		}
		
		float startPos[3];
		startArea.GetCenter(startPos);
		hPositions.PushArray(startPos, 3);
		
		for (int i = hPositions.Length - 1; i > 0; i--)
		{
			float flFromPos[3]; float flToPos[3];
			hPositions.GetArray(i, flFromPos, 3);
			hPositions.GetArray(i - 1, flToPos, 3);
			
			TE_SetupBeamPoints(flFromPos,
				flToPos,
				g_iPathLaserModelIndex,
				g_iPathLaserModelIndex,
				0,
				30,
				5.0,
				5.0,
				5.0,
				5, 
				0.0,
				iColor,
				30);
				
			TE_SendToClient(client);
		}
	}
	else 
	{
		PrintToServer("Path built!\nBuild path time: %f\nReached goal: %d", flProfileTime, bBuiltPath);
	}
	
	return Plugin_Handled;
}

public bool TraceRayDontHitEntity(int entity, int mask, any data)
{
	if (entity == data) return false;
	return true;
}