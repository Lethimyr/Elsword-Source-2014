//그랜드체이스 스키닝을 위한 FX파일

/////////////////////////////////////////////////////////////////////////////
//타입 선언
/////////////////////////////////////////////////////////////////////////////

struct VS_INPUT
{
    float4 vPosition        : POSITION;
    int    vBlendIndices    : BLENDINDICES;
    float3 vNormal          : NORMAL;
    float2 vTexCoords0       : TEXCOORD0;
    float  vTexCoords1       : TEXCOORD1;  
};

struct VS_OUTPUT
{
    float4 Pos : POSITION;
    float2 Tex0 : TEXCOORD0;
    float  Tex1 : TEXCOORD1;
    float  Tex2 : TEXCOORD2;
};




/////////////////////////////////////////////////////////////////////////////
//변수 선언
/////////////////////////////////////////////////////////////////////////////

float4x4    mViewProj;				//뷰매트릭스 * 프로젝션매트릭스
float4x4    mBoneList[15];			//입력상수 본 매트릭스 팔레트
float4x4    mInvViewBoneList[15];   //입력상수 본 매트릭스 팔레트
float4      v4LightDir;				//vecor4 라이트 방향




/////////////////////////////////////////////////////////////////////////////
//버텍스 쉐이더 함수
/////////////////////////////////////////////////////////////////////////////

VS_OUTPUT GC_SKIN_VS( const VS_INPUT v )
{
    VS_OUTPUT o = (VS_OUTPUT) 0;

    o.Pos = mul( v.vPosition, mBoneList[v.vBlendIndices] );
    o.Pos = mul( o.Pos, mViewProj );
    float4 vNor = mul( v.vNormal, mBoneList[v.vBlendIndices] );

    vNor = normalize( vNor ) - 0.4f;
    o.Tex0 = v.vTexCoords0;
    o.Tex1 = dot( vNor, v4LightDir );

    vNor = vNor + 0.5f;
    o.Tex2 = dot( vNor, v4LightDir );

    return o;
}

VS_OUTPUT GC_OUTLINE_VS( const VS_INPUT v )
{
    VS_OUTPUT o = (VS_OUTPUT) 0;

    o.Pos = mul( v.vPosition, mBoneList[v.vBlendIndices] );
    float4 vNor = mul( v.vNormal, mBoneList[v.vBlendIndices] );
    o.Pos = o.Pos + vNor * 0.005;
    o.Pos = mul( o.Pos, mViewProj );


    return o;
}



/////////////////////////////////////////////////////////////////////////////
//테크닉
/////////////////////////////////////////////////////////////////////////////

technique GC_SKIN_TECH
{
    pass P0
    {   
        Lighting                    = False;
        SpecularEnable              = False;
        
        IndexedVertexBlendEnable    = True;
        VertexBlend                 = 0Weights;
        //AlphaBlendEnable          = True;
        //SrcBlend                  = Zero;
        //DestBlend                 = Zero;

        // Stage0
        ColorOp[0]   = SelectArg1;
        ColorArg1[0] = Texture;

        MinFilter[0] = Linear;
        MagFilter[0] = Linear;
        MipFilter[0] = Linear;

        // Stage1
        ColorOp[1]      = AddSigned;
        ColorArg1[1]    = Current;
        ColorArg2[1]    = Texture;
        AddressU[1]     = Clamp;

        // Stage2
        ColorOp[2]      = Modulate;
        ColorArg1[2]    = Current;
        ColorArg2[2]    = Texture;
        AddressU[2]     = Clamp;
        
        VertexShader = compile vs_1_1 GC_SKIN_VS();
    }
}

technique GC_OUTLINE_TECH
{
    pass P0
    {   
        Lighting                    = False;
        SpecularEnable              = False;
        
        IndexedVertexBlendEnable    = True;
        VertexBlend                 = 0Weights;
        AlphaBlendEnable            = True;
        SrcBlend                    = Zero;
        DestBlend                   = Zero;
        CullMode                    = CW;

        // Stage0

        MinFilter[0] = Linear;
        MagFilter[0] = Linear;
        MipFilter[0] = Linear;

        VertexShader = compile vs_1_1 GC_OUTLINE_VS();
    }
}