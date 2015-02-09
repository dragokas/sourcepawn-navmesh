#if defined _navmesh_included
 #endinput
#endif
#define _navmesh_included

#define HalfHumanHeight 35.5

#define StepHeight 18.0

#define NAV_MAGIC_NUMBER 0xFEEDFACE

enum 
{
	NAV_DIR_NORTH = 0,
	NAV_DIR_EAST,
	NAV_DIR_SOUTH,
	NAV_DIR_WEST,
	NAV_DIR_COUNT
};

enum NavCornerType
{
	NAV_CORNER_NORTH_WEST = 0,
	NAV_CORNER_NORTH_EAST,
	NAV_CORNER_SOUTH_EAST,
	NAV_CORNER_SOUTH_WEST,
	NAV_CORNER_COUNT
};

enum
{
	NAV_LADDER_DIR_UP = 0,
	NAV_LADDER_DIR_DOWN,
	NAV_LADDER_DIR_COUNT
};

enum
{
	NAV_MESH_CROUCH = 0x0001, 
	NAV_MESH_JUMP = 0x0002, 
	NAV_MESH_PRECISE = 0x0004, 
	NAV_MESH_NO_JUMP = 0x0008, 
	NAV_MESH_STOP = 0x0010, 
	NAV_MESH_RUN = 0x0020, 
	NAV_MESH_WALK = 0x0040, 
	NAV_MESH_AVOID = 0x0080, 
	NAV_MESH_TRANSIENT = 0x0100, 
	NAV_MESH_DONT_HIDE = 0x0200, 
	NAV_MESH_STAND = 0x0400, 
	NAV_MESH_NO_HOSTAGES = 0x0800
};

enum
{
	GO_NORTH = 0,
	GO_EAST,
	GO_SOUTH,
	GO_WEST,
	GO_LADDER_UP,
	GO_LADDER_DOWN,
	GO_JUMP,
	NUM_TRAVERSE_TYPES
};

enum
{
	NavMeshArea_ID = 0,
	NavMeshArea_Flags,
	NavMeshArea_PlaceID,
	NavMeshArea_X1,
	NavMeshArea_Y1,
	NavMeshArea_Z1,
	NavMeshArea_X2,
	NavMeshArea_Y2,
	NavMeshArea_Z2,
	NavMeshArea_CenterX,
	NavMeshArea_CenterY,
	NavMeshArea_CenterZ,
	NavMeshArea_InvDxCorners,
	NavMeshArea_InvDyCorners,
	NavMeshArea_NECornerZ,
	NavMeshArea_SWCornerZ,
	
//	NavMeshArea_Connections,
	NavMeshArea_ConnectionsStartIndex,
	NavMeshArea_ConnectionsEndIndex,
	
//	NavMeshArea_HidingSpots,
	NavMeshArea_HidingSpotsStartIndex,
	NavMeshArea_HidingSpotsEndIndex,
	
//	NavMeshArea_EncounterPaths,
	NavMeshArea_EncounterPathsStartIndex,
	NavMeshArea_EncounterPathsEndIndex,
	
//	NavMeshArea_LadderConnections,
	NavMeshArea_LadderConnectionsStartIndex,
	NavMeshArea_LadderConnectionsEndIndex,
	
	NavMeshArea_CornerLightIntensityNW,
	NavMeshArea_CornerLightIntensityNE,
	NavMeshArea_CornerLightIntensitySE,
	NavMeshArea_CornerLightIntensitySW,
	
//	NavMeshArea_VisibleAreas,
	NavMeshArea_VisibleAreasStartIndex,
	NavMeshArea_VisibleAreasEndIndex,
	
	NavMeshArea_InheritVisibilityFrom,
	NavMeshArea_EarliestOccupyTimeFirstTeam,
	NavMeshArea_EarliestOccupyTimeSecondTeam,
	NavMeshArea_unk01,
	NavMeshArea_Blocked,
	
// 	A* pathfinding
	NavMeshArea_Parent,
	NavMeshArea_ParentHow,
	NavMeshArea_CostSoFar,
	NavMeshArea_TotalCost,
	NavMeshArea_Marker,
	NavMeshArea_OpenMarker,
	NavMeshArea_PrevOpenIndex,
	NavMeshArea_NextOpenIndex,
	NavMeshArea_PathLengthSoFar,
	
	NavMeshArea_NearSearchMarker,
	
	NavMeshArea_MaxStats
};

enum
{
	NavMeshConnection_AreaIndex = 0,
	NavMeshConnection_Direction,
	NavMeshConnection_MaxStats
};

enum
{
	NavMeshHidingSpot_ID = 0,
	NavMeshHidingSpot_X,
	NavMeshHidingSpot_Y,
	NavMeshHidingSpot_Z,
	NavMeshHidingSpot_Flags,
	NavMeshHidingSpot_MaxStats
};

enum
{
	NavMeshEncounterPath_FromID = 0,
	NavMeshEncounterPath_FromDirection,
	NavMeshEncounterPath_ToID,
	NavMeshEncounterPath_ToDirection,
	NavMeshEncounterPath_SpotsStartIndex,
	NavMeshEncounterPath_SpotsEndIndex,
	NavMeshEncounterPath_MaxStats
};

enum
{
	NavMeshEncounterSpot_OrderID = 0,
	NavMeshEncounterSpot_ParametricDistance,
	NavMeshEncounterSpot_MaxStats
};

enum
{
	NavMeshLadderConnection_LadderIndex = 0,
	NavMeshLadderConnection_Direction,
	NavMeshLadderConnection_MaxStats
};

enum
{
	NavMeshVisibleArea_Index = 0,
	NavMeshVisibleArea_Attributes,
	NavMeshVisibleArea_MaxStats
};

enum
{
	NavMeshLadder_ID = 0,
	NavMeshLadder_Width,
	NavMeshLadder_Length,
	NavMeshLadder_TopX,
	NavMeshLadder_TopY,
	NavMeshLadder_TopZ,
	NavMeshLadder_BottomX,
	NavMeshLadder_BottomY,
	NavMeshLadder_BottomZ,
	NavMeshLadder_Direction,
	NavMeshLadder_TopForwardAreaIndex,
	NavMeshLadder_TopLeftAreaIndex,
	NavMeshLadder_TopRightAreaIndex,
	NavMeshLadder_TopBehindAreaIndex,
	NavMeshLadder_BottomAreaIndex,
	NavMeshLadder_MaxStats
};

enum
{
	NavMeshGrid_ListStartIndex = 0,
	NavMeshGrid_ListEndIndex,
	NavMeshGrid_MaxStats
};

enum
{
	NavMeshGridList_AreaIndex = 0,
	NavMeshGridList_Owner,
	NavMeshGridList_MaxStats
};

const CNavArea INVALID_NAV_AREA = view_as<CNavArea>-1;

typedef NavPathCostFunctor = function int(CNavArea area, CNavArea from, int ladderIndex, any data);

// NavMesh

native bool NavMesh_Exists();

native int NavMesh_GetMagicNumber();

native int NavMesh_GetVersion();

native int NavMesh_GetSubVersion();

native int NavMesh_GetSaveBSPSize();

native bool NavMesh_IsAnalyzed();

native ArrayList NavMesh_GetPlaces();

native ArrayList NavMesh_GetAreas();

native ArrayList NavMesh_GetLadders();

native void NavMesh_CollectSurroundingAreas(ArrayStack hStack, CNavArea startArea, float travelDistLimit=1500.0, float maxStepUpLimit=StepHeight, float maxDropDownLimit=100.0);

native bool NavMesh_BuildPath(CNavArea startArea, CNavArea goalArea, const float goalPos[3], NavPathCostFunctor costFunctor, any costData=0, CNavArea &closestArea=INVALID_NAV_AREA, float maxPathLength=0.0);

native CNavArea NavMesh_FindAreaByID( int areaID );

native CNavArea NavMesh_GetArea(const float pos[3], float beneathLimit=120.0);

native CNavArea NavMesh_GetNearestArea(const float pos[3], bool anyZ=false, float maxDist=10000.0, bool checkLOS=false, bool checkGround=true, int team=-2);

native int NavMesh_WorldToGridX(float wx);

native int NavMesh_WorldToGridY(float wy);

native void NavMesh_GetAreasOnGrid(ArrayStack hStack, int x, int y);

native int NavMesh_GetGridSizeX();

native int NavMesh_GetGridSizeY();

native bool NavMesh_GetGroundHeight(const float pos[3], float &height=-1.0, float normal[3]=NULL_VECTOR);


// CNavArea

native int NavMeshArea_GetID(int areaIndex);

native int NavMeshArea_GetFlags(int areaIndex);

native bool NavMeshArea_GetCenter(int areaIndex, float buffer[3]);

native void NavMeshArea_GetAdjacentList(ArrayStack buffer, int areaIndex, int dir);

native void NavMeshArea_GetLadderList(ArrayStack buffer, int areaIndex, int dir);

native void NavMeshArea_GetClosestPointOnArea(int areaIndex, const float pos[3], float closePos[3]);

native int NavMeshArea_GetTotalCost(int areaIndex);

native int NavMeshArea_GetCostSoFar(int areaIndex);

native int NavMeshArea_GetParent(int areaIndex);

native int NavMeshArea_GetParentHow(int areaIndex);

native void NavMeshArea_SetParent(int areaIndex, int parentAreaIndex);

native void NavMeshArea_SetParentHow(int areaIndex, int parentHow);

native bool NavMeshArea_GetExtentLow(int areaIndex, float buffer[3]);

native bool NavMeshArea_GetExtentHigh(int areaIndex, float buffer[3]);

native bool NavMeshArea_IsOverlappingPoint(int areaIndex, const float pos[3], float tolerance);

native bool NavMeshArea_IsOverlappingArea(int areaIndex, int targetAreaIndex);

native float NavMeshArea_GetNECornerZ(int areaIndex);

native float NavMeshArea_GetSWCornerZ(int areaIndex);

native void NavMeshArea_GetCorner(int areaIndex, NavCornerType corner, float buffer[3]);

native float NavMeshArea_GetZ(int areaIndex, const float pos[3]);

native float NavMeshArea_GetZFromXAndY(int areaIndex, Float:x, Float:y);

native bool NavMeshArea_IsEdge(int areaIndex, int dir);

native bool NavMeshArea_Contains(int areaIndex, const float pos[3]);

native void NavMeshArea_GetRandomPoint(int areaIndex, float buffer[3]);

native bool NavMeshArea_ComputePortal(int areaIndex, int areaToIndex, int dir, float center[3], float &halfWidth);

native bool NavMeshArea_ComputeClosestPointInPortal(int areaIndex, int areaToIndex, int dir, const float fromPos[3], float closePos[3]);

native int NavMeshArea_ComputeDirection(int areaIndex, const float pos[3]);

native float NavMeshArea_GetLightIntensity(int areaIndex, const float pos[3]);

methodmap CNavArea
{
	public CNavArea( int index )
	{
		return view_as<CNavArea>index;
	}

	property int Index
	{
		public get() { return view_as<int>this; }
	}
	
	property int ID
	{
		public get() { return NavMeshArea_GetID(this.Index); }
	}
	
	property int Attributes
	{
		public get() { return NavMeshArea_GetFlags(this.Index); }
	}
	
	property CNavArea Parent
	{
		public get() { return CNavArea(NavMeshArea_GetParent(this.Index)); }
		public set( CNavArea area ) { NavMeshArea_SetParent(this.Index, area.Index); }
	}
	
	property int ParentHow
	{
		public get() { return NavMeshArea_GetParentHow(this.Index); }
		public set( int how ) { NavMeshArea_SetParentHow(this.Index, how); }
	}
	
	property int TotalCost
	{
		public get() { return NavMeshArea_GetTotalCost(this.Index); }
	}
	
	property int CostSoFar
	{
		public get() { return NavMeshArea_GetCostSoFar(this.Index); }
	}
	
	property float NECornerZ
	{
		public get() { return NavMeshArea_GetNECornerZ(this.Index); }
	}
	
	property float SWCornerZ
	{
		public get() { return NavMeshArea_GetSWCornerZ(this.Index); }
	}
	
	public bool GetCenter( float buffer[3] )
	{
		return NavMeshArea_GetCenter(this.Index, buffer);
	}
	
	public bool GetExtentLow( float buffer[3] )
	{
		return NavMeshArea_GetExtentLow(this.Index, buffer);
	}
	
	public bool GetExtentHigh( float buffer[3] )
	{
		return NavMeshArea_GetExtentHigh(this.Index, buffer);
	}
	
	public void GetCorner( NavCornerType corner, float buffer[3] )
	{
		NavMeshArea_GetCorner( this.Index, corner, buffer );
	}
	
	public float GetZ( const float point[3] )
	{
		return NavMeshArea_GetZ(this.Index, point);
	}
	
	public float GetZFromXAndY( float x, float y )
	{
		return NavMeshArea_GetZFromXAndY(this.Index, x, y);
	}
	
	public bool IsOverlappingPoint( const float point[3], float tolerance=0.0 )
	{
		return NavMeshArea_IsOverlappingPoint(this.Index, point, tolerance);
	}
	
	public bool IsOverlappingArea( CNavArea area )
	{
		return NavMeshArea_IsOverlappingArea(this.Index, area.Index);
	}
	
	public bool Contains( const float point[3] )
	{
		return NavMeshArea_Contains(this.Index, point);
	}
	
	public bool IsEdge( int dir )
	{
		return NavMeshArea_IsEdge(this.Index, dir);
	}
	
	public void GetRandomPoint( float buffer[3] )
	{
		NavMeshArea_GetRandomPoint(this.Index, buffer);
	}
	
	public void GetClosestPointOnArea( const float pos[3], float buffer[3] )
	{
		NavMeshArea_GetClosestPointOnArea(this.Index, pos, buffer);
	}
	
	public void GetAdjacentAreas( int dir, ArrayStack buffer )
	{
		NavMeshArea_GetAdjacentList( buffer, this.Index, dir );
	}
	
	public int GetAdjacentCount( int dir )
	{
		ArrayStack buffer = new ArrayStack();
		this.GetAdjacentAreas( dir, buffer );
		int count = 0;
		while (!buffer.Empty)
		{
			int value;
			PopStackCell(buffer, value);
			count++;
		}
		delete buffer;
		return count;
	}
	
	public CNavArea GetRandomAdjacentArea( int dir )
	{
		ArrayStack buffer = new ArrayStack();
		this.GetAdjacentAreas( dir, buffer );
		
		if (buffer.Empty)
		{
			delete buffer;
			return INVALID_NAV_AREA;
		}
		
		ArrayList list = new ArrayList();
		while (!buffer.Empty)
		{
			int value;
			PopStackCell(buffer, value);
			list.Push(value);
		}
		
		delete buffer;
		CNavArea area = CNavArea( list.Get( GetRandomInt(0, list.Length - 1) ) );
		delete list;
		return area;
	}
	
	public void GetLadders( int dir, ArrayStack buffer )
	{
		NavMeshArea_GetLadderList( buffer, this.Index, dir );
	}
	
	public void ComputePortal( CNavArea to, int dir, float center[3], float &halfWidth )
	{
		NavMeshArea_ComputePortal(this.Index, to.Index, dir, center, halfWidth);
	}
	
	public void ComputeClosestPointInPortal( CNavArea to, int dir, const float fromPos[3], float closePos[3] )
	{
		NavMeshArea_ComputeClosestPointInPortal(this.Index, to.Index, dir, fromPos, closePos);
	}
	
	public int ComputeDirection( const float point[3] )
	{
		return NavMeshArea_ComputeDirection(this.Index, point);
	}
	
	public float GetLightIntensity( const float point[3] )
	{
		return NavMeshArea_GetLightIntensity(this.Index, point);
	}
}

// CNavLadder

native float NavMeshLadder_GetLength(int iLadderID);


// Shortest-path cost function for NavMesh_BuildPath.
public int NavMeshShortestPathCost(CNavArea area, CNavArea from, int ladderIndex, any data)
{
	if (from == INVALID_NAV_AREA)
	{
		return 0;
	}
	else
	{
		int iDist = 0;
		if (ladderIndex != -1)
		{
			iDist = RoundFloat(NavMeshLadder_GetLength(ladderIndex));
		}
		else
		{
			float flAreaCenter[3]; float flFromAreaCenter[3];
			area.GetCenter(flAreaCenter);
			from.GetCenter(flFromAreaCenter);
			
			iDist = RoundFloat(GetVectorDistance(flAreaCenter, flFromAreaCenter));
		}
		
		int iCost = iDist + from.CostSoFar;
		int iAreaFlags = area.Attributes;
		if (iAreaFlags & NAV_MESH_CROUCH) iCost += 20;
		if (iAreaFlags & NAV_MESH_JUMP) iCost += (5 * iDist);
		return iCost;
	}
}

#define WALK_THRU_PROP_DOORS		0x01
#define WALK_THRU_FUNC_DOORS		0x02
#define WALK_THRU_DOORS 			(WALK_THRU_PROP_DOORS | WALK_THRU_FUNC_DOORS)
#define WALK_THRU_BREAKABLES		0x04
#define WALK_THRU_TOGGLE_BRUSHES	0x08
#define WALK_THRU_EVERYTHING		(WALK_THRU_DOORS | WALK_THRU_BREAKABLES | WALK_THRU_TOGGLE_BRUSHES)


stock bool IsEntityWalkable(int iEnt, int iFlags)
{
	char sClass[64];
	GetEntityClassname(iEnt, sClass, sizeof(sClass));
	
	if (!strcmp(sClass, "worldspawn")) return false;
	if (!strcmp(sClass, "player")) return false;
	
	if (!strcmp(sClass, "func_door*"))
	{
		return (iFlags & WALK_THRU_PROP_DOORS) ? true : false;
	}
	
	if (!strcmp(sClass, "prop_door*"))
	{
		return (iFlags & WALK_THRU_PROP_DOORS) ? true : false;
	}
	
	if (!strcmp(sClass, "func_brush"))
	{
		int iSolidity = GetEntProp(iEnt, Prop_Data, "m_iSolidity");
		
		switch (iSolidity)
		{
			case 2: // BRUSHSOLID_ALWAYS
			{
				return false;
			}
			case 1: // BRUSHSOLID_NEVER
			{
				return true;
			}
			case 0: // BRUSHSOLID_TOGGLE
			{
				return (iFlags & WALK_THRU_TOGGLE_BRUSHES) ? true : false;
			}
		}
		
		return (iFlags & WALK_THRU_PROP_DOORS) ? true : false;
	}
	
	if (!strcmp(sClass, "func_breakable") && GetEntProp(iEnt, Prop_Data, "m_iHealth") && GetEntProp(iEnt, Prop_Data, "m_takedamage") == 2) // DAMAGE_YES
	{
		return (iFlags & WALK_THRU_BREAKABLES) ? true : false;
	}
	
	if (!strcmp(sClass, "func_playerinfected_clip"))
	{
		return true;
	}
	
	return false;
}

stock bool IsWalkableTraceLineClear(const float flFrom[3], const float flTo[3], int iFlags)
{
	float flUseFrom[3];
	flUseFrom[0] = flFrom[0];
	flUseFrom[1] = flFrom[1];
	flUseFrom[2] = flFrom[2];
	
	Handle hTrace;
	
	float flFraction = 0.0;
	
	for (int t = 0; t < 50; t++)
	{
		hTrace = TR_TraceRayFilterEx(flUseFrom, flTo, MASK_NPCSOLID, RayType_EndPoint, TraceFilterWalkableEntities, iFlags);
		flFraction = TR_GetFraction(hTrace);
		int iResEnt = TR_GetEntityIndex(hTrace);
		float flResPos[3];
		TR_GetEndPosition(flResPos, hTrace);
		delete hTrace;
		
		if (flFraction != 1.0 && IsEntityWalkable(iResEnt, iFlags))
		{
			float flDir[3];
			SubtractVectors(flTo, flFrom, flDir);
			NormalizeVector(flDir, flDir);
			ScaleVector(flDir, 5.0);
			AddVectors(flResPos, flDir, flUseFrom);
		}
		else
		{
			break;
		}
	}
	
	if (flFraction == 1.0) return true;
	
	return false;
}

public bool TraceFilterWalkableEntities(int entity, int mask, any iFlags)
{
	return !IsEntityWalkable(entity, iFlags);
}


public SharedPlugin __pl_navmesh = 
{
	name = "navmesh",
	file = "navmesh.smx",
#if defined REQUIRE_PLUGIN
	required = 1,
#else
	required = 0,
#endif
};