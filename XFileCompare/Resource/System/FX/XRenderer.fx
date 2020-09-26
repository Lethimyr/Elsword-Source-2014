
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
    float3  Normal          : NORMAL;
    float4  Diffuse			: COLOR;
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

VS_BLACK_EDGE_OUTPUT VertBlackEdge( VS_INPUT i )
{
    VS_BLACK_EDGE_OUTPUT o = (VS_BLACK_EDGE_OUTPUT) 0;
    
    o.Pos = mul( i.Pos, g_mWorld );
    float4 vNor = mul( i.Normal, g_mWorld );
    
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

VS_LIGHT_FLOW_OUTPUT VertXFlowWorld( VS_INPUT i )
{
    VS_LIGHT_FLOW_OUTPUT o = (VS_LIGHT_FLOW_OUTPUT) 0;
        
    o.Pos = mul( i.Pos, g_mWorld );
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

VS_LIGHT_FLOW_OUTPUT VertZFlowWorld( VS_INPUT i )
{
    VS_LIGHT_FLOW_OUTPUT o = (VS_LIGHT_FLOW_OUTPUT) 0;
        
    o.Pos = mul( i.Pos, g_mWorld );
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

VS_LIGHT_FLOW_OUTPUT VertXFlowLocal( VS_INPUT i )
{
    VS_LIGHT_FLOW_OUTPUT o = (VS_LIGHT_FLOW_OUTPUT) 0;
        
    o.Diffuse = g_Color;    
    o.Diffuse.w = o.Diffuse.w / (0.00001 + (abs(i.Pos.x - g_fLightFlowImpact) / g_fLightFlowWide));
    o.Pos = mul( i.Pos, g_mWorldViewProj );

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

VS_LIGHT_FLOW_OUTPUT VertZFlowLocal( VS_INPUT i )
{
    VS_LIGHT_FLOW_OUTPUT o = (VS_LIGHT_FLOW_OUTPUT) 0;
        
    o.Diffuse = g_Color;    
    o.Diffuse.w = o.Diffuse.w / (0.00001 + (abs(i.Pos.z - g_fLightFlowImpact) / g_fLightFlowWide));
    o.Pos = mul( i.Pos, g_mWorldViewProj );

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
        
        VertexShader = ( compile vs_1_1 VertRealColor() );
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
		
        VertexShader = ( compile vs_1_1 VertCartoon() );
    }
	pass p1 //¿Ü°û¼±
    {
		AlphaBlendEnable	= True;
        SrcBlend			= Zero;
        DestBlend			= Zero;
        CullMode			= CW;
		
        VertexShader = ( compile vs_1_1 VertBlackEdge() );
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
        
        AlphaOp[2]      = SelectArg1;
        AlphaArg1[2]    = Current;
        
        ColorArg1[2]    = Current;
        ColorArg2[2]    = Texture;
		
        VertexShader = ( compile vs_1_1 VertCartoon() );
    }
	pass p1 //¿Ü°û¼±
    {
		AlphaBlendEnable	= True;
        CullMode			= CW;
        
        ColorOp[0]			= SelectArg1;
		ColorArg1[0]		= Diffuse;
		
        VertexShader = ( compile vs_1_1 VertColorEdge() );
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
		
        VertexShader = ( compile vs_1_1 VertCartoon() );
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
                
        VertexShader		= ( compile vs_1_1 VertXFlowWorld() );
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
                
        VertexShader		= ( compile vs_1_1 VertYFlowWorld() );
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
                
        VertexShader		= ( compile vs_1_1 VertZFlowWorld() );
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
                
        VertexShader		= ( compile vs_1_1 VertPointFlowWorld() );
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
                
        VertexShader		= ( compile vs_1_1 VertXFlowLocal() );
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
                
        VertexShader		= ( compile vs_1_1 VertYFlowLocal() );
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
                
        VertexShader		= ( compile vs_1_1 VertZFlowLocal() );
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
                
        VertexShader		= ( compile vs_1_1 VertPointFlowLocal() );
    }
}


