// Shader Model 5.0

Texture2D tex : register(t0); // Texture
SamplerState texSampler : register(s0); // Sampler state

cbuffer Constants : register(b0)
{
  float texelHeight;
  float3 offsets = { 0.0f, 1.3846153846f, 3.2307692308f };
  float3 weights = { 0.2270270270f, 0.3162162162f, 0.0702702703f };
}

float4 main(float2 uv : TEXCOORD0) : SV_TARGET
{
  float4 color = tex.Sample(texSampler, uv);
  color.rgb *= weights[0];

  for (int i = 1; i < 3; ++i)
  {
    color.rgb += tex.Sample(texSampler, float2(uv.x, uv.y - texelHeight * offsets[i])).rgb * weights[i];
    color.rgb += tex.Sample(texSampler, float2(uv.x, uv.y + texelHeight * offsets[i])).rgb * weights[i];
  }

  return color;
}
