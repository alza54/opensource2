// CopyPS.hlsl
Texture2D backBufferTex : register(t0);
SamplerState sampler0 : register(s0);

float4 main(float2 uv : TEXCOORD0) : SV_TARGET
{
  return backBufferTex.Sample(sampler0, uv);
}
