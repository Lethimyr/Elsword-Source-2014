/********************************************************************
	created:	2006/10/11
	created:	11:10:2006   16:36
	filename: 	d:\projectX2\X2Project\TestEmpty\AITest.h
	file path:	d:\projectX2\X2Project\TestEmpty
	file base:	AITest
	file ext:	h
	author:		dmlee
	
	purpose:	몬스터 AI 만들고 테스트 해보려고
	note:		CMapGraph 구현 완료

	todo:		vecNodeList, vecEdgeList는 vector가 아닌 map이 되어야?
				
*********************************************************************/
#pragma once


#if 0 //////////////////////////////////////////////////////////////////////////



class CAITest : public CX2Main
{
public:
	class CX2NPCUnit;
	class CCollision
	{
	public:
		static float PointToSegment( float& fLerpCoef, const D3DXVECTOR3* pvPoint, const D3DXVECTOR3* pvSegmentStart, const D3DXVECTOR3* pvSegmentEnd )
		{
			D3DXVECTOR3 vSegmentDir = *pvSegmentEnd - *pvSegmentStart;
			D3DXVECTOR3 vSegmentStartToPoint = *pvPoint - *pvSegmentStart;

			fLerpCoef = D3DXVec3Dot( &vSegmentDir, &vSegmentStartToPoint ) / D3DXVec3Dot( &vSegmentDir, &vSegmentDir );

			if( fLerpCoef < 0.f )
			{
				float fDistance = D3DXVec3Length( &(*pvSegmentStart - *pvPoint) );
				return fDistance;
			}
			else if( fLerpCoef > 1.f )
			{
				float fDistance = D3DXVec3Length( &(*pvSegmentEnd - *pvPoint) );
				return fDistance;
			}
			else
			{
				D3DXVECTOR3 vPointOnSegment = *pvSegmentStart + vSegmentDir * fLerpCoef;
				float fDistance = D3DXVec3Length( &(vPointOnSegment - *pvPoint) );
				return fDistance;
			}
		}


		static bool RayToPlane( float& fDistance, D3DXVECTOR3& vIntersection, 
			const D3DXVECTOR3& vRayOrigin, const D3DXVECTOR3& vRayDirection, 
			const D3DXVECTOR3& vPlaneVertex, const D3DXVECTOR3& vPlaneNormal )
		{
			// ray: P = P0 + t*V
			// plane: P.N + d = 0
			float VdotN = D3DXVec3Dot( &vRayDirection, &vPlaneNormal );
			float P0dotN = D3DXVec3Dot( &vRayOrigin, &vPlaneNormal );
			float d = -D3DXVec3Dot( &vPlaneVertex, &vPlaneNormal );
			if( VdotN == 0.f )
				return false;

			float t = -(P0dotN + d) / VdotN;
			fDistance = t;

			if( t < 0.f ) // no intersection
			{				
				return false;
			}
			else
			{				
				vIntersection = vRayOrigin + vRayDirection * t;
				return true;
			}
		}

		static bool RayToRect( float& fDistance, D3DXVECTOR3& vIntersection, 
			const D3DXVECTOR3& vRayOrigin, const D3DXVECTOR3& vRayDirection, 
			const D3DXVECTOR3& vRectVertex1, const D3DXVECTOR3& vRectVertex2, const D3DXVECTOR3& vRectNormal )
		{
			bool bResult = RayToPlane( fDistance, vIntersection, vRayOrigin, vRayDirection, vRectVertex1, vRectNormal);
			if( bResult == true )
			{
				D3DXVECTOR3 vTemp1 = vIntersection - vRectVertex1;
				D3DXVECTOR3 vTemp2 = vIntersection - vRectVertex2;
				float fIsInside = D3DXVec3Dot( &vTemp1, &vTemp2 );
				if( fIsInside < 0.f )
				{
					return true; 					
				}
			}
			return false;
		}		


		/*
		http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline3d/
		Calculate the line segment PaPb that is the shortest route between
		two lines P1P2 and P3P4. Calculate also the values of mua and mub where
		Pa = P1 + mua (P2 - P1)
		Pb = P3 + mub (P4 - P3)
		Return FALSE if no solution exists.
		*/
		static bool LineToLine( D3DXVECTOR3& pa, D3DXVECTOR3& pb, 
			float& mua, float& mub,
			const D3DXVECTOR3& p1, const D3DXVECTOR3& p2,
			const D3DXVECTOR3& p3, const D3DXVECTOR3& p4,
			const float EPS = 0.000001f )
		{
			D3DXVECTOR3 p13, p43, p21;	
			float d1343,d4321,d1321,d4343,d2121; // dot products
			float numer,denom;

			p43 = p4 - p3;			
			if( fabs(p43.x) < EPS && fabs(p43.y) < EPS && fabs(p43.z) < EPS )
				return(false);			
			p21 = p2 - p1;
			if( fabs(p21.x) < EPS && fabs(p21.y) < EPS && fabs(p21.z) < EPS)
				return(false);
			p13 = p1 - p3;

			d1343 = D3DXVec3Dot(&p13, &p43);
			d4321 = D3DXVec3Dot(&p43, &p21);
			d1321 = D3DXVec3Dot(&p13, &p21);
			d4343 = D3DXVec3Dot(&p43, &p43);
			d2121 = D3DXVec3Dot(&p21, &p21);

			denom = d2121 * d4343 - d4321 * d4321;
			if( fabs(denom) < EPS)
				return(false);
			numer = d1343 * d4321 - d1321 * d4343;

			mua = numer / denom;
			mub = (d1343 + d4321 * mua) / d4343;

			pa = p1 + p21 * mua;
			pb = p3 + p43 * mub;

			return(true);
		}

	};

	class CMapGraph
	{
	public:
		struct MapGraphNode;
		struct MapGraphEdge					// directed edge
		{
			MapGraphNode* pDestNode;
			float fCost;					// 우선 line segment 사이의 최단 거리를 저장해두자.
			D3DXVECTOR3 vSrcNearestPoint;	// out node의 line segment상의 최단 거리 지점
			D3DXVECTOR3 vDestNearestPoint;	// in node의 line segment상의 최단 거리 지점

			~MapGraphEdge()
			{
				pDestNode = NULL;
			}

			MapGraphEdge()
			{
				pDestNode = NULL;
				fCost = 0.f;
			}

			MapGraphEdge(MapGraphNode* pNode)
			{
				MapGraphEdge();
				pDestNode = pNode;
			}
		};

		struct MapGraphNode
		{
			CKTDGLineMap::LineData* pLineData;
			vector<MapGraphEdge*> vecEdgeList;

			~MapGraphNode()
			{
				pLineData = NULL;
				for( unsigned i=0; i<vecEdgeList.size(); i++ )
				{
					SAFE_DELETE( vecEdgeList[i] );
				}
				vecEdgeList.clear();				
			}

			MapGraphNode()
			{
				pLineData = NULL;
			}	

			MapGraphNode(CKTDGLineMap::LineData* pdata )
			{
				MapGraphNode();
				pLineData = pdata;
			}
		};

	public:
		~CMapGraph()
		{ 
			ReleaseMapGraph();
		};

		CMapGraph()
		{

		};

		MapGraphNode* GetNode(int index) { return vecNodeList[index]; }
		void ReleaseMapGraph()
		{
			for( unsigned i=0; i<vecNodeList.size(); i++ )
			{
				SAFE_DELETE( vecNodeList[i] );
			}
			vecNodeList.clear();
		}

		void ReBuildGraphFromLineMap( CKTDGLineMap* pLineMap )
		{
			if( NULL == pLineMap )
				return;

			int nNodes = pLineMap->GetNumLineData();
			if( nNodes <= 0 )
				return;

			ReleaseMapGraph();

			for( int i=0; i<nNodes; i++ )
			{
				CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData(i);
				MapGraphNode* pNode = new MapGraphNode(pLineData);
				vecNodeList.push_back( pNode );
			}

			for( int i=0; i<nNodes; i++ )
			{
				MapGraphNode* pSrcNode = vecNodeList[i];
				CKTDGLineMap::LineData* pSrcLine = pSrcNode->pLineData;

				for( int j=0; j<nNodes; j++ )
				{					
					if( i==j )
						continue;

					MapGraphNode* pDestNode = vecNodeList[j];
					CKTDGLineMap::LineData* pDestLine = pDestNode->pLineData;
					D3DXVECTOR3 vNearestPointOnSrcLine, vNearestPointOnDestLine;
					float mua, mub;
					// fix! KTDX에 있는 linetoline함수로 교체
					CCollision::LineToLine( vNearestPointOnSrcLine, vNearestPointOnDestLine, mua, mub, 
						pSrcLine->startPos, pSrcLine->endPos, 
						pDestLine->startPos, pDestLine->endPos );

					MapGraphEdge* pEdge = new MapGraphEdge(pDestNode);
					pEdge->fCost = D3DXVec3Length( &(vNearestPointOnSrcLine - vNearestPointOnDestLine) );
					pEdge->vSrcNearestPoint = vNearestPointOnSrcLine;
					pEdge->vDestNearestPoint = vNearestPointOnDestLine;
					pSrcNode->vecEdgeList.push_back(pEdge);
				}				
			}

#if 0// debug output
			map<MapGraphNode*, int> mapNodeIndex;
			for( int i=0; i<nNodes; i++ )
			{
				MapGraphNode* pNode = vecNodeList[i];
				mapNodeIndex.insert( std::make_pair(pNode, i) );
			}


			for( int i=0; i<nNodes; i++ )
			{
				MapGraphNode* pSrcNode = vecNodeList[i];
				CKTDGLineMap::LineData* pSrcLine = pSrcNode->pLineData;

				for( unsigned j=0; j<pSrcNode->vecEdgeList.size(); j++ )
				{
					gamelog << mapNodeIndex.find( pSrcNode )->second << "-";

					MapGraphNode* pDestNode = pSrcNode->vecEdgeList.at(j)->pDestNode;
					gamelog << mapNodeIndex.find( pDestNode )->second << "(";
					gamelog << pSrcNode->vecEdgeList.at(j)->fCost << ")" << fileout;
				}

				gamelog << fileout;
			}
#endif			
		}

	private:
		vector<MapGraphNode*> vecNodeList;

	};

	template <class UserState> class CAStarSearch // The AStar search class. UserState is the users state space type
	{
	public: // data
		enum
		{
			SEARCH_STATE_NOT_INITIALISED,
			SEARCH_STATE_SEARCHING,
			SEARCH_STATE_SUCCEEDED,
			SEARCH_STATE_FAILED,
			SEARCH_STATE_OUT_OF_MEMORY,
			SEARCH_STATE_INVALID
		};


		// A node represents a possible state in the search
		// The user provided state type is included inside this type

	public:
		class Node
		{
		public:

			Node *parent; // used during the search to record the parent of successor nodes
			Node *child; // used after the search for the application to view the search in reverse

			float g; // cost of this node + it's predecessors
			float h; // heuristic estimate of distance to goal
			float f; // sum of cumulative cost of predecessors and self and heuristic

			Node() :
			parent( 0 ),
				child( 0 ),
				g( 0.0f ),
				h( 0.0f ),
				f( 0.0f )
			{			
			}

			UserState m_UserState;
		};


		// For sorting the heap the STL needs compare function that lets us compare
		// the f value of two nodes

		class HeapCompare_f 
		{
		public:

			bool operator() ( const Node *x, const Node *y ) const
			{
				return x->f > y->f;
			}
		};


	public: // methods


		// constructor just initialises private data
		CAStarSearch( int MaxNodes = 1000 ) :
			m_AllocateNodeCount(0),
				m_FreeNodeCount(0),
				m_State( SEARCH_STATE_NOT_INITIALISED ),
				m_CurrentSolutionNode( NULL ),
				m_CancelRequest( false )
			{
			}

			// call at any time to cancel the search and free up all the memory
			void CancelSearch()
			{
				m_CancelRequest = true;
			}

			// Set Start and goal states
			void SetStartAndGoalStates( UserState &Start, UserState &Goal )
			{
				m_CancelRequest = false;

				m_Start = AllocateNode();
				m_Goal = AllocateNode();

				m_Start->m_UserState = Start;
				m_Goal->m_UserState = Goal;

				m_State = SEARCH_STATE_SEARCHING;

				// Initialise the AStar specific parts of the Start Node
				// The user only needs fill out the state information

				m_Start->g = 0; 
				m_Start->h = m_Start->m_UserState.GoalDistanceEstimate( m_Goal->m_UserState );
				m_Start->f = m_Start->g + m_Start->h;
				m_Start->parent = 0;

				// Push the start node on the Open list

				m_OpenList.push_back( m_Start ); // heap now unsorted

				// Sort back element into heap
				push_heap( m_OpenList.begin(), m_OpenList.end(), HeapCompare_f() );

				// Initialise counter for search steps
				m_Steps = 0;
			}

			// Advances search one step 
			unsigned int SearchStep()
			{
				// Firstly break if the user has not initialised the search
				assert( (m_State > SEARCH_STATE_NOT_INITIALISED) &&
					(m_State < SEARCH_STATE_INVALID) );

				// Next I want it to be safe to do a searchstep once the search has succeeded...
				if( (m_State == SEARCH_STATE_SUCCEEDED) ||
					(m_State == SEARCH_STATE_FAILED) 
					)
				{
					return m_State; 
				}

				// Failure is defined as emptying the open list as there is nothing left to 
				// search...
				// New: Allow user abort
				if( m_OpenList.empty() || m_CancelRequest )
				{
					FreeAllNodes();
					m_State = SEARCH_STATE_FAILED;
					return m_State;
				}

				// Incremement step count
				m_Steps ++;

				// Pop the best node (the one with the lowest f) 
				Node *n = m_OpenList.front(); // get pointer to the node
				pop_heap( m_OpenList.begin(), m_OpenList.end(), HeapCompare_f() );
				m_OpenList.pop_back();

				// Check for the goal, once we pop that we're done
				if( n->m_UserState.IsGoal( m_Goal->m_UserState ) )
				{
					// The user is going to use the Goal Node he passed in 
					// so copy the parent pointer of n 
					m_Goal->parent = n->parent;

					// A special case is that the goal was passed in as the start state
					// so handle that here
					if( n != m_Start )
					{
						//delete n;
						FreeNode( n );

						// set the child pointers in each node (except Goal which has no child)
						Node *nodeChild = m_Goal;
						Node *nodeParent = m_Goal->parent;

						do 
						{
							nodeParent->child = nodeChild;

							nodeChild = nodeParent;
							nodeParent = nodeParent->parent;

						} 
						while( nodeChild != m_Start ); // Start is always the first node by definition

					}

					// delete nodes that aren't needed for the solution
					FreeUnusedNodes();

					m_State = SEARCH_STATE_SUCCEEDED;

					return m_State;
				}
				else // not goal
				{

					// We now need to generate the successors of this node
					// The user helps us to do this, and we keep the new nodes in
					// m_Successors ...

					m_Successors.clear(); // empty vector of successor nodes to n

					// User provides this functions and uses AddSuccessor to add each successor of
					// node 'n' to m_Successors
					bool ret = n->m_UserState.GetSuccessors( this, n->parent ? &n->parent->m_UserState : NULL ); 

					if( !ret )
					{

						typename vector< Node * >::iterator successor;

						// free the nodes that may previously have been added 
						for( successor = m_Successors.begin(); successor != m_Successors.end(); successor ++ )
						{
							FreeNode( (*successor) );
						}

						m_Successors.clear(); // empty vector of successor nodes to n

						// free up everything else we allocated
						FreeAllNodes();

						m_State = SEARCH_STATE_OUT_OF_MEMORY;
						return m_State;
					}

					// Now handle each successor to the current node ...
					for( typename vector< Node * >::iterator successor = m_Successors.begin(); successor != m_Successors.end(); successor ++ )
					{

						// 	The g value for this successor ...
						float newg = n->g + n->m_UserState.GetCost( (*successor)->m_UserState );

						// Now we need to find whether the node is on the open or closed lists
						// If it is but the node that is already on them is better (lower g)
						// then we can forget about this successor

						// First linear search of open list to find node

						typename vector< Node * >::iterator openlist_result;

						for( openlist_result = m_OpenList.begin(); openlist_result != m_OpenList.end(); openlist_result ++ )
						{
							if( (*openlist_result)->m_UserState.IsSameState( (*successor)->m_UserState ) )
							{
								break;					
							}
						}

						if( openlist_result != m_OpenList.end() )
						{

							// we found this state on open

							if( (*openlist_result)->g <= newg )
							{
								FreeNode( (*successor) );

								// the one on Open is cheaper than this one
								continue;
							}
						}

						typename vector< Node * >::iterator closedlist_result;

						for( closedlist_result = m_ClosedList.begin(); closedlist_result != m_ClosedList.end(); closedlist_result ++ )
						{
							if( (*closedlist_result)->m_UserState.IsSameState( (*successor)->m_UserState ) )
							{
								break;					
							}
						}

						if( closedlist_result != m_ClosedList.end() )
						{

							// we found this state on closed

							if( (*closedlist_result)->g <= newg )
							{
								// the one on Closed is cheaper than this one
								FreeNode( (*successor) );

								continue;
							}
						}

						// This node is the best node so far with this particular state
						// so lets keep it and set up its AStar specific data ...

						(*successor)->parent = n;
						(*successor)->g = newg;
						(*successor)->h = (*successor)->m_UserState.GoalDistanceEstimate( m_Goal->m_UserState );
						(*successor)->f = (*successor)->g + (*successor)->h;

						// Remove successor from closed if it was on it

						if( closedlist_result != m_ClosedList.end() )
						{
							// remove it from Closed
							FreeNode(  (*closedlist_result) ); 
							m_ClosedList.erase( closedlist_result );
						}

						// Update old version of this node
						if( openlist_result != m_OpenList.end() )
						{	   

							FreeNode( (*openlist_result) ); 
							m_OpenList.erase( openlist_result );

							// re-make the heap 
							make_heap( m_OpenList.begin(), m_OpenList.end(), HeapCompare_f() );

							// make_heap rather than sort_heap is an essential bug fix
							// thanks to Mike Ryynanen for pointing this out and then explaining
							// it in detail. sort_heap called on an invalid heap does not work

							//					sort_heap( m_OpenList.begin(), m_OpenList.end(), HeapCompare_f() );

							//					assert( is_heap( m_OpenList.begin(), m_OpenList.end(), HeapCompare_f() ) );

						}

						// heap now unsorted
						m_OpenList.push_back( (*successor) );

						// sort back element into heap
						push_heap( m_OpenList.begin(), m_OpenList.end(), HeapCompare_f() );

					}

					// push n onto Closed, as we have expanded it now

					m_ClosedList.push_back( n );

				} // end else (not goal so expand)

				return m_State; // Succeeded bool is false at this point. 

			}

			// User calls this to add a successor to a list of successors
			// when expanding the search frontier
			bool AddSuccessor( UserState &State )
			{
				Node *node = AllocateNode();

				if( node )
				{
					node->m_UserState = State;

					m_Successors.push_back( node );

					return true;
				}

				return false;
			}

			// Free the solution nodes
			// This is done to clean up all used Node memory when you are done with the
			// search
			void FreeSolutionNodes()
			{
				Node *n = m_Start;

				if( m_Start->child )
				{
					do
					{
						Node *del = n;
						n = n->child;
						FreeNode( del );

						del = NULL;

					} while( n != m_Goal );

					FreeNode( n ); // Delete the goal

				}
				else
				{
					// if the start node is the solution we need to just delete the start and goal
					// nodes
					FreeNode( m_Start );
					FreeNode( m_Goal );
				}

			}

			// Functions for traversing the solution

			// Get start node
			UserState *GetSolutionStart()
			{
				m_CurrentSolutionNode = m_Start;
				if( m_Start )
				{
					return &m_Start->m_UserState;
				}
				else
				{
					return NULL;
				}
			}

			// Get next node
			UserState *GetSolutionNext()
			{
				if( m_CurrentSolutionNode )
				{
					if( m_CurrentSolutionNode->child )
					{

						Node *child = m_CurrentSolutionNode->child;

						m_CurrentSolutionNode = m_CurrentSolutionNode->child;

						return &child->m_UserState;
					}
				}

				return NULL;
			}

			// Get end node
			UserState *GetSolutionEnd()
			{
				m_CurrentSolutionNode = m_Goal;
				if( m_Goal )
				{
					return &m_Goal->m_UserState;
				}
				else
				{
					return NULL;
				}
			}

			// Step solution iterator backwards
			UserState *GetSolutionPrev()
			{
				if( m_CurrentSolutionNode )
				{
					if( m_CurrentSolutionNode->parent )
					{

						Node *parent = m_CurrentSolutionNode->parent;

						m_CurrentSolutionNode = m_CurrentSolutionNode->parent;

						return &parent->m_UserState;
					}
				}

				return NULL;
			}

			// For educational use and debugging it is useful to be able to view
			// the open and closed list at each step, here are two functions to allow that.

			UserState *GetOpenListStart()
			{
				float f,g,h;
				return GetOpenListStart( f,g,h );
			}

			UserState *GetOpenListStart( float &f, float &g, float &h )
			{
				iterDbgOpen = m_OpenList.begin();
				if( iterDbgOpen != m_OpenList.end() )
				{
					f = (*iterDbgOpen)->f;
					g = (*iterDbgOpen)->g;
					h = (*iterDbgOpen)->h;
					return &(*iterDbgOpen)->m_UserState;
				}

				return NULL;
			}

			UserState *GetOpenListNext()
			{
				float f,g,h;
				return GetOpenListNext( f,g,h );
			}

			UserState *GetOpenListNext( float &f, float &g, float &h )
			{
				iterDbgOpen++;
				if( iterDbgOpen != m_OpenList.end() )
				{
					f = (*iterDbgOpen)->f;
					g = (*iterDbgOpen)->g;
					h = (*iterDbgOpen)->h;
					return &(*iterDbgOpen)->m_UserState;
				}

				return NULL;
			}

			UserState *GetClosedListStart()
			{
				float f,g,h;
				return GetClosedListStart( f,g,h );
			}

			UserState *GetClosedListStart( float &f, float &g, float &h )
			{
				iterDbgClosed = m_ClosedList.begin();
				if( iterDbgClosed != m_ClosedList.end() )
				{
					f = (*iterDbgClosed)->f;
					g = (*iterDbgClosed)->g;
					h = (*iterDbgClosed)->h;

					return &(*iterDbgClosed)->m_UserState;
				}

				return NULL;
			}

			UserState *GetClosedListNext()
			{
				float f,g,h;
				return GetClosedListNext( f,g,h );
			}

			UserState *GetClosedListNext( float &f, float &g, float &h )
			{
				iterDbgClosed++;
				if( iterDbgClosed != m_ClosedList.end() )
				{
					f = (*iterDbgClosed)->f;
					g = (*iterDbgClosed)->g;
					h = (*iterDbgClosed)->h;

					return &(*iterDbgClosed)->m_UserState;
				}

				return NULL;
			}

			// Get the number of steps

			int GetStepCount() { return m_Steps; }

	private: // methods

		// This is called when a search fails or is cancelled to free all used
		// memory 
		void FreeAllNodes()
		{
			// iterate open list and delete all nodes
			typename vector< Node * >::iterator iterOpen = m_OpenList.begin();

			while( iterOpen != m_OpenList.end() )
			{
				Node *n = (*iterOpen);
				FreeNode( n );

				iterOpen ++;
			}

			m_OpenList.clear();

			// iterate closed list and delete unused nodes
			typename vector< Node * >::iterator iterClosed;

			for( iterClosed = m_ClosedList.begin(); iterClosed != m_ClosedList.end(); iterClosed ++ )
			{
				Node *n = (*iterClosed);
				FreeNode( n );
			}

			m_ClosedList.clear();
		}


		// This call is made by the search class when the search ends. A lot of nodes may be
		// created that are still present when the search ends. They will be deleted by this 
		// routine once the search ends
		void FreeUnusedNodes()
		{
			// iterate open list and delete unused nodes
			typename vector< Node * >::iterator iterOpen = m_OpenList.begin();

			while( iterOpen != m_OpenList.end() )
			{
				Node *n = (*iterOpen);

				if( !n->child )
				{
					FreeNode( n );

					n = NULL;
				}

				iterOpen ++;
			}

			m_OpenList.clear();

			// iterate closed list and delete unused nodes
			typename vector< Node * >::iterator iterClosed;

			for( iterClosed = m_ClosedList.begin(); iterClosed != m_ClosedList.end(); iterClosed ++ )
			{
				Node *n = (*iterClosed);

				if( !n->child )
				{
					FreeNode( n );
					n = NULL;

				}
			}

			m_ClosedList.clear();

		}

		// Node memory management
		Node *AllocateNode()
		{
			Node *p = new Node;
			return p;
		}

		void FreeNode( Node *node )
		{
			m_FreeNodeCount ++;
			delete node;
		}

	private: // data

		// Heap (simple vector but used as a heap, cf. Steve Rabin's game gems article)
		vector< Node *> m_OpenList;

		// Closed list is a vector.
		vector< Node * > m_ClosedList; 

		// Successors is a vector filled out by the user each type successors to a node
		// are generated
		vector< Node * > m_Successors;

		// State
		unsigned int m_State;

		// Counts steps
		int m_Steps;

		// Start and goal state pointers
		Node *m_Start;
		Node *m_Goal;

		Node *m_CurrentSolutionNode;

		// Memory
		//FixedSizeAllocator<Node> m_FixedSizeAllocator;

		//Debug : need to keep these two iterators around
		// for the user Dbg functions
		typename vector< Node * >::iterator iterDbgOpen;
		typename vector< Node * >::iterator iterDbgClosed;

		// debugging : count memory allocation and free's
		int m_AllocateNodeCount;
		int m_FreeNodeCount;

		bool m_CancelRequest;

	};


	class CAStarSearchNode
	{
	public:
		~CAStarSearchNode()
		{
		}
		CAStarSearchNode()
		{
			m_pUnit			= NULL;
			m_pMapGraph		= NULL;
			m_pMapGraphNode = NULL;
			m_vUnitEnterPos = D3DXVECTOR3(0,0,0);
		}
		CAStarSearchNode( CMapGraph::MapGraphNode* pNode, CMapGraph* pGraph )
		{
			m_pUnit			= NULL; 
			m_pMapGraphNode = pNode;
			m_pMapGraph		= pGraph;
			m_vUnitEnterPos = D3DXVECTOR3(0,0,0);			
		}

		bool  GetSuccessors( CAStarSearch<CAStarSearchNode> *astarsearch, CAStarSearchNode *parent_node )
		{
			if( NULL == parent_node )
				parent_node = this;

			if( parent_node->m_pMapGraphNode->vecEdgeList.size() <= 0 )
				return false;


			CAStarSearchNode NewSearchNode;
			for( unsigned i=0; i<parent_node->m_pMapGraphNode->vecEdgeList.size(); i++ )
			{
				CMapGraph::MapGraphEdge *pEdge = parent_node->m_pMapGraphNode->vecEdgeList.at(i);
				NewSearchNode.m_pMapGraphNode	= pEdge->pDestNode;
				NewSearchNode.m_pMapGraph		= this->m_pMapGraph;
				NewSearchNode.m_pUnit			= this->m_pUnit;
				NewSearchNode.m_vUnitEnterPos	= pEdge->vDestNearestPoint;
				astarsearch->AddSuccessor( NewSearchNode );
			}

			return true;
		}

		float GoalDistanceEstimate( CAStarSearchNode &nodeGoal )
		{
			return 1000.f;
		}

		float GetCost( CAStarSearchNode &successor )
		{
			const float MAGIC_BIG_FLOAT = 1000000000000000000.f;

			if( NULL == m_pMapGraphNode )
				return -1;

			// 현재 노드에서 successor 노드로 이동하는 비용 계산
			//successor->m_pMapGraphNode->vecEdgeList.at
			for( unsigned i=0; i<m_pMapGraphNode->vecEdgeList.size(); i++ )
			{			
				CMapGraph::MapGraphEdge *pEdge = this->m_pMapGraphNode->vecEdgeList.at(i);
				if( pEdge->pDestNode == successor.m_pMapGraphNode )
				{
					float fCost;
					D3DXVECTOR3 vTemp = m_vUnitEnterPos - pEdge->vSrcNearestPoint;
					fCost = D3DXVec3Length( &vTemp );
					vTemp = pEdge->vSrcNearestPoint - pEdge->vDestNearestPoint;
					fCost += D3DXVec3Length( &vTemp );
					return fCost;
				}
			}

			return MAGIC_BIG_FLOAT;		
		};
		
		bool  IsGoal( CAStarSearchNode &nodeGoal )
		{
			if( nodeGoal.m_pMapGraphNode == this->m_pMapGraphNode )
			{
				return true;
			}
			else
			{
				return false;
			}
			
		}
		bool  IsSameState( CAStarSearchNode &rhs )
		{
			if( rhs.m_pMapGraphNode == this->m_pMapGraphNode )
			{
				return true;
			}
			else
			{
				return false;
			}
		}

	public:
		CMapGraph* m_pMapGraph;
		CMapGraph::MapGraphNode* m_pMapGraphNode;
		D3DXVECTOR3 m_vUnitEnterPos;
		CX2NPCUnit* m_pUnit;
	};
	class CX2NPCUnit //: public CX2GameUnit
	{
	public:
		struct Command
		{
			enum COMMAND_TYPE
			{
				CMD_WALK,
				CMD_DASH,
				CMD_JUMPUP,
				CMD_JUMPDOWN,
			};

			struct LineArea
			{
				D3DXVECTOR3 vLineStart;
				D3DXVECTOR3 vLineEnd;
			};

			COMMAND_TYPE	cmdType;
			LineArea		destArea;
		};

	public:
		~CX2NPCUnit()
		{
		}

		CX2NPCUnit()
		{
		}

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime )
		{
			
		}
	private:
		CMapGraph m_MapGraph;
		vector<Command> m_vecCommandList;

	};

	class CX2SoloGame : public CX2PVPGame
	{
	public:
		virtual ~CX2SoloGame(void) 
		{
		}

		CX2SoloGame(void)
		{
		}

		virtual HRESULT OnLostDevice()
		{
			HRESULT hr = S_OK;
			hr = CX2PVPGame::OnLostDevice();
			return hr;
		}

		virtual HRESULT OnResetDevice()
		{
			HRESULT hr = S_OK;
			hr = CX2PVPGame::OnResetDevice();
			return hr;
		}

		//		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime ) 
		//		{
		//			HRESULT hr = S_OK;
		//			hr = CX2PVPGame::OnFrameMove(fTime, fElapsedTime);
		//			return hr;		
		//		}

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime )
		{
			m_fTime			= (float)fTime;
			m_fElapsedTime	= fElapsedTime;

			if( m_pTransfer != NULL )
				m_pTransfer->OnFrameMove( fTime, fElapsedTime );
			if( m_GameState == GS_PLAY )
			{
				UpdateUnitPointer();
				//KeyProcess();					
				MyKeyProcess( (float)fTime);

				for( UINT i = 0; i < m_UnitList.size(); i++ )
				{
					CX2GameUnit* pCX2GameUnit = m_UnitList[i];
					pCX2GameUnit->OnFrameMove( fTime, fElapsedTime );
				}
				m_pWorld->OnFrameMove( fTime, fElapsedTime );

				m_pMajorXMeshPlayer->OnFrameMove( fTime, fElapsedTime );
				m_pMinorXMeshPlayer->OnFrameMove( fTime, fElapsedTime );

				m_pDamageEffect->OnFrameMove( fTime, fElapsedTime );

				m_pMajorParticle->OnFrameMove( fTime, fElapsedTime );
				m_pMinorParticle->OnFrameMove( fTime, fElapsedTime );		

				m_pFocusUnit->OnCameraMove();
				m_pCamera->OnFrameMove( fTime, fElapsedTime );
				g_pKTDXApp->GetDSManager()->SetListenerData( m_pCamera->GetCamera().GetEye(), m_pCamera->GetCamera().GetLookVec(), m_pCamera->GetCamera().GetUpVec() );

				DetermineLastKill();
			}

			return S_OK;
		}

		virtual HRESULT OnFrameRender() 
		{
			HRESULT hr = S_OK;
			hr = CX2PVPGame::OnFrameRender();
			return hr;
		}

		virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
		{
			bool bNoFurtherProcessing = false;

			switch( wParam )
			{
			case CX2Packet::XPT_UNIT_FULL_SYNC_7:
				return P2PUnitFullSync( hWnd, uMsg, wParam, lParam );
				break;
			}

			return bNoFurtherProcessing;
		}

		//void SetWorld( CX2World* pWorld ) { m_pWorld = pWorld; }

	protected:
		bool P2PUnitFullSync( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
		{

			//if( (long)lParam < 0 ) 
			//	return false;

			//CKTDNPacket::PacketData* pPacketData = (CKTDNPacket::PacketData*)lParam;
			//if( NULL == pPacketData->pPacket || pPacketData->pPacket <= 0 )
			//	return false;

			//CX2Packet::SXPT_UNIT_FULL_SYNC_7* pSXPT_UNIT_FULL_SYNC = (CX2Packet::SXPT_UNIT_FULL_SYNC*)pPacketData->pPacket;
			//if( NULL == pSXPT_UNIT_FULL_SYNC )
			//	return false;

			//CX2GameUnit* pCX2GameUnit = GetUnitByUID( pSXPT_UNIT_FULL_SYNC->unitUID );
			//if( pCX2GameUnit != g_pX2Game->GetMyUnit() )
			//	return false;

			//if ( pCX2GameUnit != NULL )
			//	pCX2GameUnit->ReceiveFrameData( pPacketData );

			//SAFE_DELETE( pPacketData );
			//return true;

			if( (long)lParam < 0 ) 
				return false;

			CKTDNPacket::PacketData* pPacketData = (CKTDNPacket::PacketData*)lParam;
			if( NULL == pPacketData->pPacket || pPacketData->pPacket <= 0 )
				return false;

			if( g_pMain->GetFrameBufferNum() == 7 )
			{
				CX2Packet::SXPT_UNIT_FULL_SYNC_7* pSXPT_UNIT_FULL_SYNC = (CX2Packet::SXPT_UNIT_FULL_SYNC_7*)pPacketData->pPacket;
				if( NULL == pSXPT_UNIT_FULL_SYNC )
					return false;

				CX2GameUnit* pCX2GameUnit = GetUnitByUID( pSXPT_UNIT_FULL_SYNC->unitUID );
				if( pCX2GameUnit != g_pX2Game->GetMyUnit() )
					return false;

				if ( pCX2GameUnit != NULL )
					pCX2GameUnit->ReceiveFrameData( pPacketData );
			}
			else if( g_pMain->GetFrameBufferNum() == 8 )
			{
				CX2Packet::SXPT_UNIT_FULL_SYNC_8* pSXPT_UNIT_FULL_SYNC = (CX2Packet::SXPT_UNIT_FULL_SYNC_8*)pPacketData->pPacket;
				if( NULL == pSXPT_UNIT_FULL_SYNC )
					return false;

				CX2GameUnit* pCX2GameUnit = GetUnitByUID( pSXPT_UNIT_FULL_SYNC->unitUID );
				if( pCX2GameUnit != g_pX2Game->GetMyUnit() )
					return false;

				if ( pCX2GameUnit != NULL )
					pCX2GameUnit->ReceiveFrameData( pPacketData );
			}

			SAFE_DELETE( pPacketData );
			return true;

		}


		void MyKeyProcess(float fTime) 
		{
			/*
			int x = (int)fTime;
			if( x%5 == 0 )
			{			
				CX2GameUnit* pCX2GameUnit = GetMyUnit();
				m_InputData.Init();

				int y = rand()%5;
				switch(y)
				{
				case 0:
					m_InputData.pureLeft = true;
					break;
				case 1:
					m_InputData.pureRight = true;
					break;
				case 2:
					m_InputData.pureUp = true;
					break;
				case 3:
					m_InputData.pureDown = true;
					break;
				case 4:
					y = rand()%10;
					if( y == 0 )					
						m_InputData.oneZ = true;
					break;
				}
			}
			pCX2GameUnit->SetInputData( m_InputData );
			*/

			//#if 0 // open device 테스트
			//			if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_3) == TRUE )
			//			{
			//				g_pKTDXApp->GetDeviceManager()->OpenDeviceGroup( 0 );
			//			}
			//
			//			if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_4) == TRUE )
			//			{
			//				g_pKTDXApp->GetDeviceManager()->CloseDeviceGroup( 0 );
			//			}
			//
			//			if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_5) == TRUE )
			//			{
			//				g_pKTDXApp->GetDeviceManager()->OpenDeviceDirectory( L"D:\\projectX2\\X2Project\\X2\\dat\\Model\\World\\ElTreeDown\\" );
			//			}
			//
			//			if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_6) == TRUE )
			//			{
			//				g_pKTDXApp->GetDeviceManager()->CloseDeviceDirectory( L"D:\\projectX2\\X2Project\\X2\\dat\\Model\\World\\ElTreeDown\\" );
			//			}
			//#endif


			CX2GameUnit* pCX2GameUnit = GetMyUnit();


			if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_1) == TRUE )
			{
				m_MapGraph.ReBuildGraphFromLineMap( this->GetWorld()->GetLineMap() );
			}

			if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_2) == TRUE )
			{
		
				const float MAGIC_LINE_RADIUS = 30.f;
				int iLineIndex=54;
				D3DXVECTOR3 vOutPos;

				CAStarSearchNode NodeStart;
				NodeStart.m_vUnitEnterPos = this->GetMyUnit()->GetPos();				
				NodeStart.m_pMapGraph	  = &(this->m_MapGraph);
				this->GetWorld()->GetLineMap()->GetLandPosition( NodeStart.m_vUnitEnterPos, MAGIC_LINE_RADIUS, &iLineIndex );
				NodeStart.m_pMapGraphNode = m_MapGraph.GetNode(iLineIndex);
				
				CAStarSearchNode NodeGoal;
				NodeGoal.m_vUnitEnterPos = this->GetMyUnit()->GetPos();
				NodeGoal.m_pMapGraph	  = &(this->m_MapGraph);
				NodeGoal.m_pMapGraphNode = m_MapGraph.GetNode( 70 );

				m_PathFinder.SetStartAndGoalStates( NodeStart, NodeGoal );
				unsigned int SearchState;
				do
				{
					SearchState = m_PathFinder.SearchStep();
				}
				while( SearchState == CAStarSearch<CAStarSearchNode>::SEARCH_STATE_SEARCHING );

                
				if( SearchState == CAStarSearch<CAStarSearchNode>::SEARCH_STATE_SUCCEEDED )
				{
					m_vecCommandList.clear();
					
					CAStarSearchNode *pNodePrev = m_PathFinder.GetSolutionStart();
					for( ;; )
					{
						CAStarSearchNode* pNode = m_PathFinder.GetSolutionNext();
						if( NULL == pNode )
						{
							break;
						}
					
						CX2NPCUnit::Command NewCmd;

						// 이전 line segment와 지금 line segment가 연결되어 있으면 dash 나 walk
						if( pNode->m_pMapGraphNode->pLineData->startPos == pNodePrev->m_pMapGraphNode->pLineData->endPos ||
							pNode->m_pMapGraphNode->pLineData->endPos == pNodePrev->m_pMapGraphNode->pLineData->startPos ) 
						{
							if( 0 == rand()%1 )
							{
								NewCmd.cmdType = CX2NPCUnit::Command::CMD_DASH;
							}
							else
							{
								NewCmd.cmdType = CX2NPCUnit::Command::CMD_WALK;
							}
						}

						//if( ) // 떨어져 있는데 위쪽에 있으면 jump
						//if( ) // 위 앞쪽에 있으면 dash jump
						//if( ) // 아래에 있으면 down
						//if( ) // 아래 앞쪽에 있으면 dash down
						//
						//NewCmd.cmdType = CX2NPCUnit::Command::CMD_WALK;						
						NewCmd.destArea.vLineStart = pNode->m_pMapGraphNode->pLineData->startPos;
						NewCmd.destArea.vLineEnd   = pNode->m_pMapGraphNode->pLineData->endPos;
						m_vecCommandList.push_back( NewCmd );

						pNodePrev = pNode;
					}

					// Once you're done with the solution you can free the nodes up
					m_PathFinder.FreeSolutionNodes();
				}
				else if( SearchState == CAStarSearch<CAStarSearchNode>::SEARCH_STATE_FAILED ) 
				{
					// do nothing
				}
			}
	

			m_InputData.Init();
			if( m_vecCommandList.size() > 0 )
			{				
				CX2NPCUnit::Command cmd = m_vecCommandList[0];				
				bool bCommandAchieved = false;
				D3DXVECTOR3 vUnitPos = GetMyUnit()->GetPos();

				float fLerp;
				CCollision::PointToSegment( fLerp, &vUnitPos, &cmd.destArea.vLineStart, &cmd.destArea.vLineEnd );
				if( fLerp > 0.f && fLerp < 1.f ) 
				{
					D3DXVECTOR3 vOnLinePos = cmd.destArea.vLineStart*(1.f-fLerp) + cmd.destArea.vLineEnd*fLerp;
					vOnLinePos -= vUnitPos;
					if( D3DXVec3Length( &vOnLinePos ) < 4.f )						
					{				
						bCommandAchieved = true;
						m_vecCommandList.erase( m_vecCommandList.begin() );
					}
				}

				cmd = m_vecCommandList[0];
				switch( cmd.cmdType )
				{
				case CX2NPCUnit::Command::CMD_WALK:
					{
						D3DXVECTOR3 vDestPos = cmd.destArea.vLineStart + cmd.destArea.vLineEnd;
						if( vDestPos.x > vUnitPos.x )
						{
							m_InputData.pureRight = true;
						}
						else
						{
							m_InputData.pureLeft = true;
						}
					} break;

				case CX2NPCUnit::Command::CMD_DASH:
					{
						D3DXVECTOR3 vDestPos = cmd.destArea.vLineStart + cmd.destArea.vLineEnd;
						if( vDestPos.x > vUnitPos.x )
						{
							m_InputData.pureDoubleRight = true;
							m_InputData.oneDoubleRight  = true;
						}
						else
						{
							m_InputData.pureDoubleLeft = true;
							m_InputData.oneDoubleLeft  = true;
						}
					} break;

				case CX2NPCUnit::Command::CMD_JUMPUP:
					{
						D3DXVECTOR3 vDestPos = cmd.destArea.vLineStart + cmd.destArea.vLineEnd;
						//if( vDestPos.x > vUnitPos.x )
						//{
						//	m_InputData.oneRight = true;
						//}
						//else
						//{
						//	m_InputData.oneLeft = true;
						//}

						m_InputData.pureUp = true;
						m_InputData.oneUp = true;
						m_InputData.pureDoubleUp = true;
						m_InputData.oneDoubleUp = true;
					} break;

				case CX2NPCUnit::Command::CMD_JUMPDOWN:
					{
						D3DXVECTOR3 vDestPos = cmd.destArea.vLineStart + cmd.destArea.vLineEnd;
						if( vDestPos.x > vUnitPos.x )
						{
							m_InputData.pureRight = true;
						}
						else
						{
							m_InputData.pureLeft = true;
						}
						m_InputData.pureDown = true;
					} break;				

				}
			}
			else
			{
				m_InputData.Init();
			}

			pCX2GameUnit->SetInputData( m_InputData );
		}

	private:
		CAStarSearch<CAStarSearchNode> m_PathFinder;
		CMapGraph m_MapGraph;
		vector<CX2NPCUnit::Command> m_vecCommandList;
	};

public:
	virtual ~CAITest(void);
	CAITest(void);

	virtual HRESULT OnLostDevice();
	virtual HRESULT OnResetDevice();
	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();
	virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

protected:
	void CreateNewGame();

private:
	CX2SoloGame* m_pGame;

};




#endif //////////////////////////////////////////////////////////////////////////
