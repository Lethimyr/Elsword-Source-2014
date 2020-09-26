//--------------------------------------------------------------------------------------
// File: ShadowVolume.fx
//
// The effect file for the MultiAnimation sample.  
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#ifndef MATRIX_PALETTE_SIZE_DEFAULT
#define MATRIX_PALETTE_SIZE_DEFAULT 26
#endif

const int MATRIX_PALETTE_SIZE = MATRIX_PALETTE_SIZE_DEFAULT;
float4x4 amPalette[ MATRIX_PALETTE_SIZE_DEFAULT ];


//----------------------------------------------------------------------------
// Shader body - VS_ Skin
//----------------------------------------------------------------------------

// define the inputs -- caller must fill this, usually right from the VB
struct VS_SKIN_INPUT
{
    float4      vPos;
    float3      vBlendWeights;
    float4      vBlendIndices;
    float3      vNor;
};

// return skinned position and normal
struct VS_SKIN_OUTPUT
{
    float4 vPos;
    float4 vNor;
};

// call this function to skin VB position and normal
VS_SKIN_OUTPUT VS_Skin( const VS_SKIN_INPUT vInput, int iNumBones )
{
    VS_SKIN_OUTPUT vOutput = (VS_SKIN_OUTPUT) 0;

    float fLastWeight = 1.0;
    float fWeight;
    float afBlendWeights[ 3 ] = (float[ 3 ]) vInput.vBlendWeights;
    int aiIndices[ 4 ] = (int[ 4 ]) D3DCOLORtoUBYTE4( vInput.vBlendIndices );
    
    for( int iBone = 0; (iBone < 3) && (iBone < iNumBones - 1); ++ iBone )
    {
        fWeight = afBlendWeights[ iBone ];
        fLastWeight -= fWeight;
        vOutput.vPos.xyz += mul( vInput.vPos, amPalette[ aiIndices[ iBone ] ] ) * fWeight;
        vOutput.vNor     += mul( vInput.vNor, amPalette[ aiIndices[ iBone ] ] ) * fWeight;
    }
    
    vOutput.vPos.xyz += mul( vInput.vPos, amPalette[ aiIndices[ iNumBones - 1 ] ] ) * fLastWeight;
    vOutput.vNor     += mul( vInput.vNor, amPalette[ aiIndices[ iNumBones - 1 ] ] ) * fLastWeight;

    return vOutput;
}

int	CurNumBones = 3;

float4x4 g_mWorld			: WORLD;
float4x4 g_mViewProj		: VIEWPROJECTION;
float4x4 g_mWorldViewProj	: WORLDVIEWPROJECTION;
float4	 g_Color;
float4   g_LightDir;
float4	 g_OutLineColor;
float	 g_fOutLineWide;
float	 g_fLightFlowImpact;
float4	 g_LightFlowPoint;
float	 g_fLightFlowWide;
float4	 g_TexOffSet01;
float	 g_fTexOffSet2X;
float	 g_fTexOffSet2Y;


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4  Pos             : POSITION;
    float3  BlendWeights    : BLENDWEIGHT;
    float4  BlendIndices    : BLENDINDICES;
    float3  Normal          : NORMAL;
    float3  Tex0            : TEXCOORD0;
};

struct VS_REAL_COLOR_OUTPUT
{
    float4  Pos     : POSITION;
    float4  Diffuse	: COLOR;
    float2  Tex0    : TEXCOORD0;
    float2  Tex1    : TEXCOORD1;
    float2  Tex2    : TEXCOORD2;
};

struct VS_CARTOON_OUTPUT
{
    float4  Pos     : POSITION;
    float4  Diffuse	: COLOR;
    float2  Tex0    : TEXCOORD0;
    float2  Tex1    : TEXCOORD1;
    float2  Tex2    : TEXCOORD2;
};

struct VS_BLACK_EDGE_OUTPUT
{
    float4  Pos     : POSITION;
};

struct VS_COLOR_EDGE_OUTPUT
{
    float4  Pos     : POSITION;
    float4  Diffuse : COLOR;
};

struct VS_LIGHT_FLOW_OUTPUT
{
    float4  Pos     : POSITION;
    float4  Diffuse : COLOR;
};


VS_REAL_COLOR_OUTPUT VertRealColor( VS_INPUT i )
{
    VS_REAL_COLOR_OUTPUT o = (VS_REAL_COLOR_OUTPUT) 0;
        
    o.Pos	= mul( i.Pos, g_mWorldViewProj );

    // copy the input texture coordinate through
    o.Tex0		= i.Tex0;
    o.Tex0.x	+= g_TexOffSet01.x;
    o.Tex0.y	+= g_TexOffSet01.y;
    
    o.Tex1		= i.Tex0;
    o.Tex1.x	+= g_TexOffSet01.z;
    o.Tex1.y	+= g_TexOffSet01.w;
    
    o.Tex2		= i.Tex0;
    o.Tex2.x	+= g_fTexOffSet2X;
    o.Tex2.y	+= g_fTexOffSet2Y;
    
    o.Diffuse = g_Color;

    return o;
}

VS_REAL_COLOR_OUTPUT VertSkinningRealColor( VS_INPUT i, uniform int iNumBones )
{
    VS_REAL_COLOR_OUTPUT o = (VS_REAL_COLOR_OUTPUT) 0;
    
     // skin VB inputs
    VS_SKIN_INPUT vsi	= { i.Pos, i.BlendWeights, i.BlendIndices, i.Normal };
    VS_SKIN_OUTPUT vso	= VS_Skin( vsi, iNumBones );
        
    // transform position from world space into view and then projection space
    o.Pos	= mul( float4( vso.vPos.xyz, 1.0f ), g_mViewProj );
    
    // copy the input texture coordinate through
    o.Tex0		= i.Tex0;
    o.Tex0.x	+= g_TexOffSet01.x;
    o.Tex0.y	+= g_TexOffSet01.y;
    
    o.Tex1		= i.Tex0;
    o.Tex1.x	+= g_TexOffSet01.z;
    o.Tex1.y	+= g_TexOffSet01.w;
    
    o.Tex2		= i.Tex0;
    o.Tex2.x	+= g_fTexOffSet2X;
    o.Tex2.y	+= g_fTexOffSet2Y;
    
    o.Diffuse = g_Color;  

    return o;
}

VS_CARTOON_OUTPUT VertCartoon( VS_INPUT i )
{
    VS_CARTOON_OUTPUT o = (VS_CARTOON_OUTPUT) 0;
        
    o.Pos	= mul( i.Pos, g_mWorldViewProj );
    float4 vNor = mul( i.Normal, g_mWorld );
    
    // normalize normals
    vNor = normalize( vNor );

    // copy the input texture coordinate through
    o.Tex0		= i.Tex0;
    o.Tex0.x	+= g_TexOffSet01.x;
    o.Tex0.y	+= g_TexOffSet01.y;
    
    o.Tex1		= dot( vNor, g_LightDir );
    
    o.Tex2		= i.Tex0;
    o.Tex2.x	+= g_fTexOffSet2X;
    o.Tex2.y	+= g_fTexOffSet2Y;

    o.Diffuse = g_Color;

    return o;
}

VS_CARTOON_OUTPUT VertSkinningCartoon( VS_INPUT i, uniform int iNumBones )
{
    VS_CARTOON_OUTPUT   o;
    
    // skin VB inputs
    VS_SKIN_INPUT vsi = { i.Pos, i.BlendWeights, i.BlendIndices, i.Normal };
    VS_SKIN_OUTPUT vso = VS_Skin( vsi, iNumBones );

    // transform position from world space into view and then projection space
    o.Pos = mul( float4( vso.vPos.xyz, 1.0f ), g_mViewProj );

    // normalize normals
    float4 vNor = normalize( vso.vNor );

    // copy the input texture coordinate through
    o.Tex0		= i.Tex0;
    o.Tex0.x	+= g_TexOffSet01.x;
    o.Tex0.y	+= g_TexOffSet01.y;
    
    o.Tex1		= dot( vNor, g_LightDir );
    
    o.Tex2		= i.Tex0;
    o.Tex2.x	+= g_fTexOffSet2X;
    o.Tex2.y	+= g_fTexOffSet2Y;
    
    o.Diffuse = g_Color;

    return o;
}

VS_BLACK_EDGE_OUTPUT VertBlackEdge( VS_INPUT i )
{
    VS_BLACK_EDGE_OUTPUT o = (VS_BLACK_EDGE_OUTPUT) 0;
    
    o.Pos = mul( i.Pos, g_mWorld );
    float4 vNor = mul( i.Normal, g_mWorld );
    
    o.Pos = o.Pos + vNor * g_fOutLineWide;
    o.Pos = mul( o.Pos, g_mViewProj );

    return o;
}

VS_BLACK_EDGE_OUTPUT VertSkinningBlackEdge( VS_INPUT i, uniform int iNumBones )
{
    VS_BLACK_EDGE_OUTPUT	o;
    
    // skin VB inputs
    VS_SKIN_INPUT vsi = { i.Pos, i.BlendWeights, i.BlendIndices, i.Normal };
    VS_SKIN_OUTPUT vso = VS_Skin( vsi, iNumBones );

    // transform position from world space into view and then projection space
    o.Pos = float4( vso.vPos.xyz, 1.0f );
    
    float4 vNor = vso.vNor;    
    o.Pos = o.Pos + vNor * g_fOutLineWide;
    o.Pos = mul( o.Pos, g_mViewProj );

    return o;
}

VS_COLOR_EDGE_OUTPUT VertColorEdge( VS_INPUT i )
{
    VS_COLOR_EDGE_OUTPUT o = (VS_COLOR_EDGE_OUTPUT) 0;
    
    o.Pos = mul( i.Pos, g_mWorld );
    float4 vNor = mul( i.Normal, g_mWorld );
    
    o.Pos = o.Pos + vNor * g_fOutLineWide;
    o.Pos = mul( o.Pos, g_mViewProj );
    o.Diffuse = g_OutLineColor;

    return o;
}

VS_COLOR_EDGE_OUTPUT VertSkinningColorEdge( VS_INPUT i, uniform int iNumBones )
{
    VS_COLOR_EDGE_OUTPUT	o;
    
    // skin VB inputs
    VS_SKIN_INPUT vsi = { i.Pos, i.BlendWeights, i.BlendIndices, i.Normal };
    VS_SKIN_OUTPUT vso = VS_Skin( vsi, iNumBones );

    // transform position from world space into view and then projection space
    o.Pos = float4( vso.vPos.xyz, 1.0f );
    
    float4 vNor = vso.vNor;    
    o.Pos = o.Pos + vNor * g_fOutLineWide;
    o.Pos = mul( o.Pos, g_mViewProj );
    o.Diffuse = g_OutLineColor;

    return o;
}

VS_LIGHT_FLOW_OUTPUT VertXFlowWorld( VS_INPUT i )
{
    VS_LIGHT_FLOW_OUTPUT o = (VS_LIGHT_FLOW_OUTPUT) 0;
        
    o.Pos = mul( i.Pos, g_mWorld );
    o.Diffuse = g_Color;    
    o.Diffuse.w = o.Diffuse.w / (0.00001 + (abs(o.Pos.x - g_fLightFlowImpact) / g_fLightFlowWide));
    o.Pos = mul( o.Pos, g_mViewProj );

    return o;
}

VS_LIGHT_FLOW_OUTPUT VertSkinningXFlowWorld( VS_INPUT i, uniform int iNumBones )
{
    VS_LIGHT_FLOW_OUTPUT	o;
    
    // skin VB inputs
    VS_SKIN_INPUT vsi = { i.Pos, i.BlendWeights, i.BlendIndices, i.Normal };
    VS_SKIN_OUTPUT vso = VS_Skin( vsi, iNumBones );

    // transform position from world space into view and then projection space
    o.Pos = float4( vso.vPos.xyz, 1.0f );    
    o.Diffuse = g_Color; 
    o.Diffuse.w = o.Diffuse.w / (0.00001 + (abs(o.Pos.x - g_fLightFlowImpact) / g_fLightFlowWide));
    o.Pos = mul( o.Pos, g_mViewProj );

    return o;
}

VS_LIGHT_FLOW_OUTPUT VertYFlowWorld( VS_INPUT i )
{
    VS_LIGHT_FLOW_OUTPUT o = (VS_LIGHT_FLOW_OUTPUT) 0;
        
    o.Pos = mul( i.Pos, g_mWorld );
    o.Diffuse = g_Color;    
    o.Diffuse.w = o.Diffuse.w / (0.00001 + (abs(o.Pos.y - g_fLightFlowImpact) / g_fLightFlowWide));
    o.Pos = mul( o.Pos, g_mViewProj );

    return o;
}

VS_LIGHT_FLOW_OUTPUT VertSkinningYFlowWorld( VS_INPUT i, uniform int iNumBones )
{
    VS_LIGHT_FLOW_OUTPUT	o;
    
    // skin VB inputs
    VS_SKIN_INPUT vsi = { i.Pos, i.BlendWeights, i.BlendIndices, i.Normal };
    VS_SKIN_OUTPUT vso = VS_Skin( vsi, iNumBones );

    // transform position from world space into view and then projection space
    o.Pos = float4( vso.vPos.xyz, 1.0f );    
    o.Diffuse = g_Color; 
    o.Diffuse.w = o.Diffuse.w / (0.00001 + (abs(o.Pos.y - g_fLightFlowImpact) / g_fLightFlowWide));
    o.Pos = mul( o.Pos, g_mViewProj );

    return o;
}

VS_LIGHT_FLOW_OUTPUT VertZFlowWorld( VS_INPUT i )
{
    VS_LIGHT_FLOW_OUTPUT o = (VS_LIGHT_FLOW_OUTPUT) 0;
        
    o.Pos = mul( i.Pos, g_mWorld );
    o.Diffuse = g_Color;    
    o.Diffuse.w = o.Diffuse.w / (0.00001 + (abs(o.Pos.z - g_fLightFlowImpact) / g_fLightFlowWide));
    o.Pos = mul( o.Pos, g_mViewProj );

    return o;
}

VS_LIGHT_FLOW_OUTPUT VertSkinningZFlowWorld( VS_INPUT i, uniform int iNumBones )
{
    VS_LIGHT_FLOW_OUTPUT	o;
    
    // skin VB inputs
    VS_SKIN_INPUT vsi = { i.Pos, i.BlendWeights, i.BlendIndices, i.Normal };
    VS_SKIN_OUTPUT vso = VS_Skin( vsi, iNumBones );

    // transform position from world space into view and then projection space
    o.Pos = float4( vso.vPos.xyz, 1.0f );    
    o.Diffuse = g_Color; 
    o.Diffuse.w = o.Diffuse.w / (0.00001 + (abs(o.Pos.z - g_fLightFlowImpact) / g_fLightFlowWide));
    o.Pos = mul( o.Pos, g_mViewProj );

    return o;
}

VS_LIGHT_FLOW_OUTPUT VertPointFlowWorld( VS_INPUT i )
{
    VS_LIGHT_FLOW_OUTPUT o = (VS_LIGHT_FLOW_OUTPUT) 0;
        
    o.Pos = mul( i.Pos, g_mWorld );
    o.Diffuse = g_Color;    
    o.Diffuse.w = o.Diffuse.w / (0.00001 + (distance(o.Pos,g_LightFlowPoint) / g_fLightFlowWide));
    o.Pos = mul( o.Pos, g_mViewProj );

    return o;
}

VS_LIGHT_FLOW_OUTPUT VertSkinningPointFlowWorld( VS_INPUT i, uniform int iNumBones )
{
    VS_LIGHT_FLOW_OUTPUT	o;
    
    // skin VB inputs
    VS_SKIN_INPUT vsi = { i.Pos, i.BlendWeights, i.BlendIndices, i.Normal };
    VS_SKIN_OUTPUT vso = VS_Skin( vsi, iNumBones );
    
    // transform position from world space into view and then projection space
    o.Pos = float4( vso.vPos.xyz, 1.0f );    
    o.Diffuse = g_Color; 
    o.Diffuse.w = o.Diffuse.w / (0.00001 + (distance(o.Pos,g_LightFlowPoint) / g_fLightFlowWide));
    o.Pos = mul( o.Pos, g_mViewProj );

    return o;
}

VS_LIGHT_FLOW_OUTPUT VertXFlowLocal( VS_INPUT i )
{
    VS_LIGHT_FLOW_OUTPUT o = (VS_LIGHT_FLOW_OUTPUT) 0;
        
    o.Diffuse = g_Color;    
    o.Diffuse.w = o.Diffuse.w / (0.00001 + (abs(i.Pos.x - g_fLightFlowImpact) / g_fLightFlowWide));
    o.Pos = mul( i.Pos, g_mWorldViewProj );

    return o;
}

VS_LIGHT_FLOW_OUTPUT VertSkinningXFlowLocal( VS_INPUT i, uniform int iNumBones )
{
    VS_LIGHT_FLOW_OUTPUT o = (VS_LIGHT_FLOW_OUTPUT) 0;
        
    o.Diffuse = g_Color;    
    o.Diffuse.w = o.Diffuse.w / (0.00001 + (abs(i.Pos.x - g_fLightFlowImpact) / g_fLightFlowWide));
    
    // skin VB inputs
    VS_SKIN_INPUT vsi = { i.Pos, i.BlendWeights, i.BlendIndices, i.Normal };
    VS_SKIN_OUTPUT vso = VS_Skin( vsi, iNumBones );
    
    // transform position from world space into view and then projection space
    o.Pos = float4( vso.vPos.xyz, 1.0f );  
    o.Pos = mul( o.Pos, g_mViewProj );

    return o;
}

VS_LIGHT_FLOW_OUTPUT VertYFlowLocal( VS_INPUT i )
{
    VS_LIGHT_FLOW_OUTPUT o = (VS_LIGHT_FLOW_OUTPUT) 0;
        
    o.Diffuse = g_Color;    
    o.Diffuse.w = o.Diffuse.w / (0.00001 + (abs(i.Pos.y - g_fLightFlowImpact) / g_fLightFlowWide));
    o.Pos = mul( i.Pos, g_mWorldViewProj );

    return o;
}

VS_LIGHT_FLOW_OUTPUT VertSkinningYFlowLocal( VS_INPUT i, uniform int iNumBones )
{
    VS_LIGHT_FLOW_OUTPUT o = (VS_LIGHT_FLOW_OUTPUT) 0;
        
    o.Diffuse = g_Color;    
    o.Diffuse.w = o.Diffuse.w / (0.00001 + (abs(i.Pos.y - g_fLightFlowImpact) / g_fLightFlowWide));
    
    // skin VB inputs
    VS_SKIN_INPUT vsi = { i.Pos, i.BlendWeights, i.BlendIndices, i.Normal };
    VS_SKIN_OUTPUT vso = VS_Skin( vsi, iNumBones );
    
    // transform position from world space into view and then projection space
    o.Pos = float4( vso.vPos.xyz, 1.0f );  
    o.Pos = mul( o.Pos, g_mViewProj );

    return o;
}

VS_LIGHT_FLOW_OUTPUT VertZFlowLocal( VS_INPUT i )
{
    VS_LIGHT_FLOW_OUTPUT o = (VS_LIGHT_FLOW_OUTPUT) 0;
        
    o.Diffuse = g_Color;    
    o.Diffuse.w = o.Diffuse.w / (0.00001 + (abs(i.Pos.z - g_fLightFlowImpact) / g_fLightFlowWide));
    o.Pos = mul( i.Pos, g_mWorldViewProj );

    return o;
}

VS_LIGHT_FLOW_OUTPUT VertSkinningZFlowLocal( VS_INPUT i, uniform int iNumBones )
{
    VS_LIGHT_FLOW_OUTPUT o = (VS_LIGHT_FLOW_OUTPUT) 0;
        
    o.Diffuse = g_Color;    
    o.Diffuse.w = o.Diffuse.w / (0.00001 + (abs(i.Pos.z - g_fLightFlowImpact) / g_fLightFlowWide));
    
    // skin VB inputs
    VS_SKIN_INPUT vsi = { i.Pos, i.BlendWeights, i.BlendIndices, i.Normal };
    VS_SKIN_OUTPUT vso = VS_Skin( vsi, iNumBones );
    
    // transform position from world space into view and then projection space
    o.Pos = float4( vso.vPos.xyz, 1.0f );  
    o.Pos = mul( o.Pos, g_mViewProj );

    return o;
}

VS_LIGHT_FLOW_OUTPUT VertPointFlowLocal( VS_INPUT i )
{
    VS_LIGHT_FLOW_OUTPUT o = (VS_LIGHT_FLOW_OUTPUT) 0;
        
    o.Diffuse = g_Color;    
    o.Diffuse.w = o.Diffuse.w / (0.00001 + (distance(i.Pos,g_LightFlowPoint) / g_fLightFlowWide));
    o.Pos = mul( i.Pos, g_mWorldViewProj );

    return o;
}

VS_LIGHT_FLOW_OUTPUT VertSkinningPointFlowLocal( VS_INPUT i, uniform int iNumBones )
{
    VS_LIGHT_FLOW_OUTPUT	o;
          
    o.Diffuse = g_Color; 
    o.Diffuse.w = o.Diffuse.w / (0.00001 + (distance(i.Pos,g_LightFlowPoint) / g_fLightFlowWide));
    
    // skin VB inputs
    VS_SKIN_INPUT vsi = { i.Pos, i.BlendWeights, i.BlendIndices, i.Normal };
    VS_SKIN_OUTPUT vso = VS_Skin( vsi, iNumBones );
    
    // transform position from world space into view and then projection space
    o.Pos = float4( vso.vPos.xyz, 1.0f );  
    o.Pos = mul( o.Pos, g_mViewProj );

    return o;
}




VertexShader vsArrayRealColor11[ 5 ] = { compile vs_1_1 VertRealColor(),
										compile vs_1_1 VertSkinningRealColor( 1 ),
										compile vs_1_1 VertSkinningRealColor( 2 ),
										compile vs_1_1 VertSkinningRealColor( 3 ),
										compile vs_1_1 VertSkinningRealColor( 4 ) };

VertexShader vsArrayCartoon11[ 5 ] = { compile vs_1_1 VertCartoon(),
										compile vs_1_1 VertSkinningCartoon( 1 ),
										compile vs_1_1 VertSkinningCartoon( 2 ),
										compile vs_1_1 VertSkinningCartoon( 3 ),
										compile vs_1_1 VertSkinningCartoon( 4 ) };
										
VertexShader vsArrayBlackEdge11[ 5 ] = { compile vs_1_1 VertBlackEdge(),
										compile vs_1_1 VertSkinningBlackEdge( 1 ),
										compile vs_1_1 VertSkinningBlackEdge( 2 ),
										compile vs_1_1 VertSkinningBlackEdge( 3 ),
										compile vs_1_1 VertSkinningBlackEdge( 4 ) };
										
VertexShader vsArrayColorEdge11[ 5 ] = { compile vs_1_1 VertColorEdge(),
										compile vs_1_1 VertSkinningColorEdge( 1 ),
										compile vs_1_1 VertSkinningColorEdge( 2 ),
										compile vs_1_1 VertSkinningColorEdge( 3 ),
										compile vs_1_1 VertSkinningColorEdge( 4 ) };
										
VertexShader vsArrayXFlowWorld11[ 5 ] = { compile vs_1_1 VertXFlowWorld(),
										compile vs_1_1 VertSkinningXFlowWorld( 1 ),
										compile vs_1_1 VertSkinningXFlowWorld( 2 ),
										compile vs_1_1 VertSkinningXFlowWorld( 3 ),
										compile vs_1_1 VertSkinningXFlowWorld( 4 ) };
										
VertexShader vsArrayYFlowWorld11[ 5 ] = { compile vs_1_1 VertYFlowWorld(),
										compile vs_1_1 VertSkinningYFlowWorld( 1 ),
										compile vs_1_1 VertSkinningYFlowWorld( 2 ),
										compile vs_1_1 VertSkinningYFlowWorld( 3 ),
										compile vs_1_1 VertSkinningYFlowWorld( 4 ) };
										
VertexShader vsArrayZFlowWorld11[ 5 ] = { compile vs_1_1 VertZFlowWorld(),
										compile vs_1_1 VertSkinningZFlowWorld( 1 ),
										compile vs_1_1 VertSkinningZFlowWorld( 2 ),
										compile vs_1_1 VertSkinningZFlowWorld( 3 ),
										compile vs_1_1 VertSkinningZFlowWorld( 4 ) };
										
VertexShader vsArrayPointFlowWorld11[ 5 ] = { compile vs_1_1 VertPointFlowWorld(),
										compile vs_1_1 VertSkinningPointFlowWorld( 1 ),
										compile vs_1_1 VertSkinningPointFlowWorld( 2 ),
										compile vs_1_1 VertSkinningPointFlowWorld( 3 ),
										compile vs_1_1 VertSkinningPointFlowWorld( 4 ) };
										
										
VertexShader vsArrayXFlowLocal11[ 5 ] = { compile vs_1_1 VertXFlowLocal(),
										compile vs_1_1 VertSkinningXFlowLocal( 1 ),
										compile vs_1_1 VertSkinningXFlowLocal( 2 ),
										compile vs_1_1 VertSkinningXFlowLocal( 3 ),
										compile vs_1_1 VertSkinningXFlowLocal( 4 ) };
										
VertexShader vsArrayYFlowLocal11[ 5 ] = { compile vs_1_1 VertYFlowLocal(),
										compile vs_1_1 VertSkinningYFlowLocal( 1 ),
										compile vs_1_1 VertSkinningYFlowLocal( 2 ),
										compile vs_1_1 VertSkinningYFlowLocal( 3 ),
										compile vs_1_1 VertSkinningYFlowLocal( 4 ) };
										
VertexShader vsArrayZFlowLocal11[ 5 ] = { compile vs_1_1 VertZFlowLocal(),
										compile vs_1_1 VertSkinningZFlowLocal( 1 ),
										compile vs_1_1 VertSkinningZFlowLocal( 2 ),
										compile vs_1_1 VertSkinningZFlowLocal( 3 ),
										compile vs_1_1 VertSkinningZFlowLocal( 4 ) };

VertexShader vsArrayPointFlowLocal11[ 5 ] = { compile vs_1_1 VertPointFlowLocal(),
										compile vs_1_1 VertSkinningPointFlowLocal( 1 ),
										compile vs_1_1 VertSkinningPointFlowLocal( 2 ),
										compile vs_1_1 VertSkinningPointFlowLocal( 3 ),
										compile vs_1_1 VertSkinningPointFlowLocal( 4 ) };




//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------

technique REAL_COLOR
{
    pass p0
    {
        AlphaOp[0]      = Modulate;
        AlphaArg1[0]    = Texture;
        AlphaArg2[0]    = Diffuse;
        
        ColorOp[0]      = Modulate;
        ColorArg1[0]    = Texture;
        ColorArg2[0]    = Diffuse;
        
        AlphaOp[1]      = SelectArg1;
        AlphaArg1[1]    = Current;
        
        ColorArg1[1]    = Current;
        ColorArg2[1]    = Texture;
        
        AlphaOp[2]      = SelectArg1;
        AlphaArg1[2]    = Current;
        
        ColorArg1[2]    = Current;
        ColorArg2[2]    = Texture;
        
        VertexShader = ( vsArrayRealColor11[ CurNumBones ] );
    }
}

technique CARTOON_BLACK_EDGE
{
	pass p0
    {        	
		AlphaOp[0]      = Modulate;
        AlphaArg1[0]    = Texture;
        AlphaArg2[0]    = Diffuse;
        
        AlphaOp[1]      = SelectArg1;
        AlphaArg1[1]    = Current;
        
		// Stage0
		ColorOp[0]		= Modulate;
		ColorArg1[0]	= Texture;
		ColorArg2[0]	= Diffuse;
		
		// Stage1
        ColorOp[1]      = Modulate;
        ColorArg1[1]    = Current;
        ColorArg2[1]    = Texture;
        AddressU[1]     = Clamp;
        
        AlphaOp[2]      = SelectArg1;
        AlphaArg1[2]    = Current;
        
        ColorArg1[2]    = Current;
        ColorArg2[2]    = Texture;
		
        VertexShader = ( vsArrayCartoon11[ CurNumBones ] );
    }
    
    pass p1//¿Ü°û¼±
    {    
		AlphaBlendEnable	= True;
		
        SrcBlend			= Zero;
        DestBlend			= Zero;
        CullMode			= CW;
		
        VertexShader = ( vsArrayBlackEdge11[ CurNumBones ] );
    }
	
}

technique CARTOON_COLOR_EDGE
{
	pass p0
    {        	
		AlphaOp[0]      = Modulate;
        AlphaArg1[0]    = Texture;
        AlphaArg2[0]    = Diffuse;
        
        AlphaOp[1]      = SelectArg1;
        AlphaArg1[1]    = Current;
        
		// Stage0
		ColorOp[0]		= Modulate;
		ColorArg1[0]	= Texture;
		ColorArg2[0]	= Diffuse;
		
		// Stage1
        ColorOp[1]      = Modulate;
        ColorArg1[1]    = Current;
        ColorArg2[1]    = Texture;
        AddressU[1]     = Clamp;
        
        ColorArg1[2]    = Current;
        ColorArg2[2]    = Texture;
		
        VertexShader = ( vsArrayCartoon11[ CurNumBones ] );
    }
    
    pass p1//¿Ü°û¼±
    {    
		AlphaBlendEnable	= True;
		CullMode			= CW;
        
		ColorOp[0]			= SelectArg1;
		ColorArg1[0]		= Diffuse;
		
        VertexShader = ( vsArrayColorEdge11[ CurNumBones ] );
    }	
}

technique CARTOON
{
	pass p0
    {        	
		AlphaOp[0]      = Modulate;
        AlphaArg1[0]    = Texture;
        AlphaArg2[0]    = Diffuse;
        
        AlphaOp[1]      = SelectArg1;
        AlphaArg1[1]    = Current;
        
		// Stage0
		ColorOp[0]		= Modulate;
		ColorArg1[0]	= Texture;
		ColorArg2[0]	= Diffuse;
		
		// Stage1
        ColorOp[1]      = Modulate;
        ColorArg1[1]    = Current;
        ColorArg2[1]    = Texture;
        AddressU[1]     = Clamp;
        
        AlphaOp[2]      = SelectArg1;
        AlphaArg1[2]    = Current;
        
        ColorArg1[2]    = Current;
        ColorArg2[2]    = Texture;
		
        VertexShader = ( vsArrayCartoon11[ CurNumBones ] );
    }	
}

technique LIGHT_FLOW_WORLD_X
{	       
    pass p0
    {	    
		AlphaBlendEnable    = True;
				
		SrcBlend            = SrcAlpha;
        DestBlend           = DestAlpha;
		
		AlphaOp[0]			= Add;
        AlphaArg1[0]		= Current;
        AlphaArg2[0]		= Diffuse;
        
        ColorArg1[0]		= Diffuse;
                
        VertexShader		= ( vsArrayXFlowWorld11[ CurNumBones ] );
    }
}

technique LIGHT_FLOW_WORLD_Y
{	       
    pass p0
    {	    
		AlphaBlendEnable    = True;
				
		SrcBlend            = SrcAlpha;
        DestBlend           = DestAlpha;
		
		AlphaOp[0]			= Add;
        AlphaArg1[0]		= Current;
        AlphaArg2[0]		= Diffuse;
        
        ColorArg1[0]		= Diffuse;
                
        VertexShader		= ( vsArrayYFlowWorld11[ CurNumBones ] );
    }
}

technique LIGHT_FLOW_WORLD_Z
{	       
    pass p0
    {	    
		AlphaBlendEnable    = True;
				
		SrcBlend            = SrcAlpha;
        DestBlend           = DestAlpha;
		
		AlphaOp[0]			= Add;
        AlphaArg1[0]		= Current;
        AlphaArg2[0]		= Diffuse;
        
        ColorArg1[0]		= Diffuse;
                
        VertexShader		= ( vsArrayZFlowWorld11[ CurNumBones ] );
    }
}

technique LIGHT_FLOW_WORLD_POINT
{	       
    pass p0
    {	    
		AlphaBlendEnable    = True;
				
		SrcBlend            = SrcAlpha;
        DestBlend           = DestAlpha;
		
		AlphaOp[0]			= Add;
        AlphaArg1[0]		= Current;
        AlphaArg2[0]		= Diffuse;
        
        ColorArg1[0]		= Diffuse;
                
        VertexShader		= ( vsArrayPointFlowWorld11[ CurNumBones ] );
    }
}


technique LIGHT_FLOW_LOCAL_X
{	       
    pass p0
    {	    
		AlphaBlendEnable    = True;
				
		SrcBlend            = SrcAlpha;
        DestBlend           = DestAlpha;
		
		AlphaOp[0]			= Add;
        AlphaArg1[0]		= Current;
        AlphaArg2[0]		= Diffuse;
        
        ColorArg1[0]		= Diffuse;
                
        VertexShader		= ( vsArrayXFlowLocal11[ CurNumBones ] );
    }
}

technique LIGHT_FLOW_LOCAL_Y
{	       
    pass p0
    {	    
		AlphaBlendEnable    = True;
				
		SrcBlend            = SrcAlpha;
        DestBlend           = DestAlpha;
		
		AlphaOp[0]			= Add;
        AlphaArg1[0]		= Current;
        AlphaArg2[0]		= Diffuse;
        
        ColorArg1[0]		= Diffuse;
                
        VertexShader		= ( vsArrayYFlowLocal11[ CurNumBones ] );
    }
}

technique LIGHT_FLOW_LOCAL_Z
{	       
    pass p0
    {	    
		AlphaBlendEnable    = True;
				
		SrcBlend            = SrcAlpha;
        DestBlend           = DestAlpha;
		
		AlphaOp[0]			= Add;
        AlphaArg1[0]		= Current;
        AlphaArg2[0]		= Diffuse;
        
        ColorArg1[0]		= Diffuse;
                
        VertexShader		= ( vsArrayZFlowLocal11[ CurNumBones ] );
    }
}

technique LIGHT_FLOW_LOCAL_POINT
{	       
    pass p0
    {	    
		AlphaBlendEnable    = True;
				
		SrcBlend            = SrcAlpha;
        DestBlend           = DestAlpha;
		
		AlphaOp[0]			= Add;
        AlphaArg1[0]		= Current;
        AlphaArg2[0]		= Diffuse;
        
        ColorArg1[0]		= Diffuse;
                
        VertexShader		= ( vsArrayPointFlowLocal11[ CurNumBones ] );
    }
}





